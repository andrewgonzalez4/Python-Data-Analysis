#include "main.h"
#include "json.hpp"
#include "odometry.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace okapi;
using nlohmann::json;

#define MAX_SPEED 100
#define LEFT_WHEEL_DOWN 4
#define RIGHT_WHEEL_DOWN 2
#define LEFT_WHEEL_UP 3
#define RIGHT_WHEEL_UP 1
#define ROTATION_ARM 10

#define ULTRA_PING_PORT 1
#define ULTRA_ECHO_PORT 2
#define ULTRA_PING_PORT2 7
#define ULTRA_ECHO_PORT2 8

#define VISION_PORT 21

#define NUM_VISION_OBJECTS 3
//Author: andrew.gonzalez4 on gitlab and andrewgonzalez4 on github.

pros::Motor left_wheels_down (LEFT_WHEEL_DOWN);
pros::Motor right_wheels_down (RIGHT_WHEEL_DOWN, true);//true means it goes reversed.
pros::Motor left_wheels_up (LEFT_WHEEL_UP);
pros::Motor right_wheels_up (RIGHT_WHEEL_UP, true);//true means it goes reversed.
pros::Motor rotation_arm (ROTATION_ARM);

pros::ADIUltrasonic ultrasonic (ULTRA_PING_PORT, ULTRA_ECHO_PORT);
pros::ADIUltrasonic ultrasonic2 (ULTRA_PING_PORT2, ULTRA_ECHO_PORT2);

pros::Vision vision_sensor (VISION_PORT);

inline okapi::AverageFilter<5> avgFilter;
inline okapi::DemaFilter demaFilter(0.2, 0.05);

struct motorData{

  std::vector<int> speeds;
  std::vector<int> x;
  std::vector<int> y;
  std::vector<int> theta;

}mtr1,mtr2,mtr3;

void moveMotors(){
   left_wheels_up.move_voltage(12000);
   right_wheels_up.move_voltage(10000);
   rotation_arm.move_voltage(1000);
 }

 void stopMotors(){

   right_wheels_down.move_voltage(0);
   left_wheels_down.move_voltage(0);
   left_wheels_up.move_voltage(0);
   right_wheels_up.move_voltage(0);
   rotation_arm.move_voltage(0);
 }

 void convert(){
   std::string filename;
   //Dictionaries
   std::vector<std::vector<std::vector<int>>> motors;
   std::vector<std::vector<std::vector<int>>> sensors;
   //Individual Components
   std::vector<std::vector<int>> mtr_1;
   std::vector<std::vector<int>> mtr_2;
   std::vector<std::vector<int>> mtr_3;
   std::vector<std::vector<int>> ultsonic;
   std::vector<std::vector<int>> ultsonic2;
   //Unique Values
   //MTR_1
   std::vector<int> speeds;
   std::vector<std::vector<float>> pos_1;
   std::vector<float> x_1;
   std::vector<float> y_1;
   std::vector<float> theta_1;
   //MTR_2
   std::vector<int> speeds2;
   std::vector<int> pos_2;
   std::vector<float> x_2;
   std::vector<float> y_2;
   std::vector<float> theta_2;
   //MTR_3
   std::vector<int> speeds3;
   std::vector<int> pos_3;
   std::vector<float> x_3;
   std::vector<float> y_3;
   std::vector<float> theta_3;
   //ULT_SONIC
   std::vector<int> ult_1;
   std::vector<int> ult_2;

  Timer timer;
  timer.placeMark();
  json j;
  json i;

  while(timer.getDtFromMark() < 10000_ms){
    //moveMotors();

    speeds.push_back(left_wheels_up.get_actual_velocity());
    speeds2.push_back(right_wheels_up.get_actual_velocity());
    speeds3.push_back(rotation_arm.get_actual_velocity());
    ult_1.push_back((int)demaFilter.filter(ultrasonic.get_value()));
    ult_2.push_back((int)demaFilter.filter(ultrasonic2.get_value()));
    // std::cout <<  right_wheels_down.get_actual_velocity() << "Motor right down";
    // std::cout <<  right_wheels_up.get_actual_velocity() << "Motor right up";
    // std::cout <<  rotation_arm.get_actual_velocity() << "Motor arm";
    //std::printf("%f", right_wheels_down.get_actual_velocity());
    pros::delay(30);
  }
  stopMotors();

  //Pushing Individual Values
  //Motor 1
  pos_1.push_back(x_1);
  pos_1.push_back(y_1);
  pos_1.push_back(theta_1);
  mtr_1.push_back(speeds);
  mtr_1.push_back(pos_1);
  //Motor 2
  pos_2.push_back(x_2);
  pos_2.push_back(y_2);
  pos_2.push_back(theta_2);
  mtr_2.push_back(pos_2);
  mtr_2.push_back(speeds2);


  mtr_3.push_back(speeds3);
  ultsonic.push_back(ult_1);
  ultsonic2.push_back(ult_2);
  //Pushing Individual Components
  motors.push_back(mtr_1);
  motors.push_back(mtr_2);
  motors.push_back(mtr_3);
  sensors.push_back(ultsonic);
  sensors.push_back(ultsonic2);

  //Creating dicitionaries
  j["Motors"] = motors;
  j["Sensors"] = sensors;

  std::vector<std::string> toSD;

  filename = "j.json";

  std::string wri = j.dump();

  toSD.push_back(wri);


  char cstr[wri.size() +1];
  strcpy(cstr, wri.c_str());

  std::ofstream out(filename);
  out << j.dump();
  out.close();

FILE* usd_file_write = fopen("/usd/j.json", "w");//open file
fputs(cstr, usd_file_write);//write in the file
fclose(usd_file_write);//close the file

//Gotten from https://pros.cs.purdue.edu/v5/tutorials/topical/filesystem.html
//and https://www.techiedelight.com/convert-string-char-array-cpp/

}

//ULTRASONIC
void SonicAdjustment(int voltage){
  Timer timer;
  timer.placeMark();
  //Attemp for 10 seconds
   //while(timer.getDtFromMark() < 10000_ms){
    //Constantly calculate difference and acquire values
    int difference = (int)ultrasonic.get_value() - (int)ultrasonic2.get_value();

    //Minimum value of 15 for difference
    while(abs(difference) >= 15 &&
    ((int)ultrasonic.get_value() >= 30 && (int)ultrasonic2.get_value()  >= 30)){
      //
      if((int)ultrasonic2.get_value() > (int)ultrasonic.get_value()){
        right_wheels_up.move_voltage(-voltage);
        left_wheels_up.move_voltage(voltage);
        right_wheels_down.move_voltage(-voltage);
        left_wheels_down.move_voltage(voltage);
      }
      //
      else if((int)ultrasonic.get_value() > (int)ultrasonic2.get_value()){
        right_wheels_up.move_voltage(voltage);
        left_wheels_up.move_voltage(-voltage);
        right_wheels_down.move_voltage(voltage);
        left_wheels_down.move_voltage(-voltage);
      }
      //Update Info
      difference = (int)ultrasonic.get_value() - (int)ultrasonic2.get_value();
    }
    stopMotors();
  //}
}

//VISION SENSOR
void LargestColorAcquisition(){
  std::string outt = "";
  //vision_sensor.set_zero_point(VISION_ZERO_CENTER);
  while (true) {

    pros::vision_object_s_t rtn = vision_sensor.get_by_size(0);
    // Gets the largest object
    if(rtn.signature == 1){
      outt = "Purple";
    }
    else if(rtn.signature == 2){
      outt = "Green";
    }
    else if(rtn.signature == 3){
      outt = "Orange";
    }
    else{
      outt = "None lol";
    }
    //std::cout << "\nColor:" << outt << " ";
    pros::lcd::print(4,  "Color: %s\n", outt);
    //std::cout << "sig: " << rtn.signature;
    pros::delay(2);
  }
}
void testcode(){
  while (true) {
		int n = vision_sensor.get_object_count();
		pros::lcd::print(0, "%d ", n );

    pros::vision_object_s_t  obj;
		if( n > 0 ) {
			pros::vision_object_s_t  obj1 = vision_sensor.get_by_sig(0, 1);
      pros::vision_object_s_t  obj2 = vision_sensor.get_by_sig(0, 2);
      pros::vision_object_s_t  obj3 = vision_sensor.get_by_sig(0, 3);
      if(obj1.x_middle_coord > obj2.x_middle_coord
      && obj1.x_middle_coord > obj3.x_middle_coord){
        obj = obj1;
      }
      else if(obj2.x_middle_coord > obj1.x_middle_coord
      && obj2.x_middle_coord > obj3.x_middle_coord){
        obj = obj2;
      }
      else if(obj3.x_middle_coord > obj1.x_middle_coord
      && obj3.x_middle_coord > obj2.x_middle_coord){
        obj = obj3;
      }
			pros::lcd::print(1, "x: %3d y: %3d", obj.x_middle_coord, obj.y_middle_coord );
			pros::lcd::print(2, "w: %3d h: %3d", obj.width, obj.height );
      pros::lcd::print(3, "sig: %d", obj.signature);
		}
		else {
			pros::lcd::print(1, "no objects");
			pros::lcd::print(2, "          ");
		}

		pros::delay(20);
	}
}
void LargestColorAcquisition2(){
   std::string outt = "";
   pros::vision_object_s_t object_arr[NUM_VISION_OBJECTS];
   while (true) {
    vision_sensor.read_by_size(0, NUM_VISION_OBJECTS, object_arr);
    if(object_arr[0].signature == 1){
      outt = "Green";
    }
    else if(object_arr[0].signature == 2){
      outt = "Purple";
    }
    else{
      outt = "None lol";
    }
    std::cout << "\nColor:" << outt << " ";
    // Prints the signature of the largest object found
    pros::delay(2);
  }
}
class visionSensor
{
  public:
  //visionSensor();

  std::string largestSigStr()
  {
    std::string sigColor;
    pros::vision_object_s_t largestSig = vision_sensor.get_by_size(0);

    if (largestSig.signature == 3)
    {sigColor = "Orange";}
    else if (largestSig.signature == 2)
    {sigColor = "Purple";}
    else if (largestSig.signature == 1)
    {sigColor = "Green";}
    else
    {sigColor = "No sig detected";}

    return sigColor;
  }

  bool colorCheck(std::string Color)
  {
    int n;
    bool colorMatch;
      if (largestSigStr() != Color)
      { colorMatch = false;}
      else
      colorMatch = true;
    pros::lcd::print(4,  "Color is Purple %s\n", colorMatch);
    return colorMatch;
  }

  void centerCube(std::string Color, int voltage, int threshold)
  {
    if (colorCheck(Color) == true)
      {
        pros::vision_object_s_t cube = vision_sensor.get_by_size(0);
        while((-threshold <= cube.x_middle_coord <= threshold) && (cube.x_middle_coord != 0))
        {
          if (cube.x_middle_coord > 10)
          {
           //strafeVoltage(-voltage); //strafe left
          }
          else if(cube.x_middle_coord < -10 )
          {
           //strafeVoltage(voltage);  //strafe right
          }
           //stopDrive(); //stop motors
        }

      }
    else;
  }
};
inline visionSensor vision; //object creation
