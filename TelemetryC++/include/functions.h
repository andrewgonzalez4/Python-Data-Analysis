#include "main.h"
#include "json.hpp"
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
//Author: andrew.gonzalez4 on gitlab and andrewgonzalez4 on github.

pros::Motor left_wheels_down (LEFT_WHEEL_DOWN);
pros::Motor right_wheels_down (RIGHT_WHEEL_DOWN, true);//true means it goes reversed.
pros::Motor left_wheels_up (LEFT_WHEEL_UP);
pros::Motor right_wheels_up (RIGHT_WHEEL_UP, true);//true means it goes reversed.
pros::Motor rotation_arm (ROTATION_ARM);
pros::ADIUltrasonic ultrasonic (ULTRA_PING_PORT, ULTRA_ECHO_PORT);
pros::ADIUltrasonic ultrasonic2 (ULTRA_PING_PORT2, ULTRA_ECHO_PORT2);

inline okapi::AverageFilter<5> avgFilter;
inline okapi::DemaFilter demaFilter(0.2, 0.05);


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
   std::vector<int> speeds;
   std::vector<int> speeds2;
   std::vector<int> speeds3;
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
  mtr_1.push_back(speeds);
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

void SonicAdjustment(int voltage){
  Timer timer;
  timer.placeMark();
  //Attemp for 10 seconds
  while(timer.getDtFromMark() < 10000_ms){
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
  }


}
