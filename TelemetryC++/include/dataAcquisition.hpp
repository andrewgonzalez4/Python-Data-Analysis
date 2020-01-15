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

#define VISION_PORT 21

pros::ADIUltrasonic ultrasonic (ULTRA_PING_PORT, ULTRA_ECHO_PORT);
pros::ADIUltrasonic ultrasonic2 (ULTRA_PING_PORT2, ULTRA_ECHO_PORT2);

pros::Vision vision_sensor (VISION_PORT);

pros::Motor left_wheels_down (LEFT_WHEEL_DOWN);
pros::Motor right_wheels_down (RIGHT_WHEEL_DOWN, true);//true means it goes reversed.
pros::Motor left_wheels_up (LEFT_WHEEL_UP);
pros::Motor right_wheels_up (RIGHT_WHEEL_UP, true);//true means it goes reversed.
pros::Motor rotation_arm (ROTATION_ARM);

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

//Method that intakes data from motors, sensors and PID values and graphs them
//in an auxiliary python program
//Andrew González Perez || Alberto Cruz Salamán

void convert(){

   std::string filename;
   //Vectors that will be initialized as json dictionaries
   std::vector<std::vector<std::vector<float>>> motors;
   std::vector<std::vector<std::vector<int>>> sensors;
   std::vector<std::vector<int>> PIDvalues;

   //MOTORS
   //MTR_1
   std::vector<std::vector<float>> mtr1;
   std::vector<float> speeds_1;
   std::vector<float> x_1;
   std::vector<float> y_1;
   std::vector<float> theta_1;
   //MTR_2
   std::vector<std::vector<float>> mtr2;
   std::vector<float> speeds_2;
   std::vector<float> x_2;
   std::vector<float> y_2;
   std::vector<float> theta_2;
   //MTR_3
   std::vector<std::vector<float>> mtr3;
   std::vector<float> speeds_3;
   std::vector<float> x_3;
   std::vector<float> y_3;
   std::vector<float> theta_3;

   //SENSORS
   //ULT_SONIC
   std::vector<std::vector<int>> ultsonic;
   std::vector<std::vector<int>> ultsonic2;
   std::vector<int> ult_1;
   std::vector<int> ult_2;


   Timer timer;
   timer.placeMark();
   json j;

   while(timer.getDtFromMark() < 10000_ms){
     moveMotors();

     //Acquiring motor velocities
     speeds_1.push_back(left_wheels_up.get_actual_velocity());//MTR_1
     speeds_2.push_back(right_wheels_up.get_actual_velocity());//MTR_2
     speeds_3.push_back(rotation_arm.get_actual_velocity());//MTR_3

     //Acquiring ultrasonic proximity values
     ult_1.push_back(demaFilter.filter(ultrasonic.get_value()));//ULT_1
     ult_2.push_back(demaFilter.filter(ultrasonic2.get_value()));//ULT_2

     pros::delay(30);
   }
   stopMotors();

     //Pushing Individual Values
     //
     //
     //MTR_1
     mtr1.push_back(speeds_1);
     mtr1.push_back(x_1);
     mtr1.push_back(y_1);
     mtr1.push_back(theta_1);
     motors.push_back(mtr1);
     //
     //MTR_2
     mtr2.push_back(speeds_2);
     mtr2.push_back(x_2);
     mtr2.push_back(y_2);
     mtr2.push_back(theta_2);
     motors.push_back(mtr2);
     //
     //MTR_3
     mtr3.push_back(speeds_3);
     mtr3.push_back(x_3);
     mtr3.push_back(y_3);
     mtr3.push_back(theta_3);
     motors.push_back(mtr3);
     //
     //ULTRASONIC
     ultsonic.push_back(ult_1);
     ultsonic2.push_back(ult_2);
     sensors.push_back(ultsonic);
     sensors.push_back(ultsonic2);
     //
     //
     //Creating JSON
     //Creating dicitionaries
     j["Motors"] = motors;
     j["Sensors"] = sensors;
     j["PID"] = PIDvalues;

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
