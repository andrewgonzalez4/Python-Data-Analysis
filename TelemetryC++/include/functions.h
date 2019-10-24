#include "main.h"
#include "json.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
using namespace okapi;
using nlohmann::json;
#define MAX_SPEED 100
#define LEFT_WHEEL_DOWN 2
#define RIGHT_WHEEL_DOWN 1
#define LEFT_WHEEL_UP 5
#define RIGHT_WHEEL_UP 9
#define ROTATION_ARM 10
//Author: andrew.gonzalez4 on gitlab and andrewgonzalez4 on github.

pros::Motor left_wheels_down (LEFT_WHEEL_DOWN);
pros::Motor right_wheels_down (RIGHT_WHEEL_DOWN, true);//true means it goes reversed.
pros::Motor left_wheels_up (LEFT_WHEEL_UP);
pros::Motor right_wheels_up (RIGHT_WHEEL_UP, true);//true means it goes reversed.
pros::Motor rotation_arm (ROTATION_ARM);

  void moveMotors(){
   right_wheels_down.move_voltage(12000);
   right_wheels_up.move_voltage(10000);
   rotation_arm.move_voltage(1000);
 }

 void stopMotors(){
   right_wheels_down.move_voltage(0);
   right_wheels_up.move_voltage(0);
   rotation_arm.move_voltage(0);
 }

 void convert(){
   std::string filename;
   std::vector<std::vector<std::vector<int>>> motors;
   std::vector<std::vector<int>> organize;
   std::vector<std::vector<int>> organize2;
    std::vector<std::vector<int>> organize3;
   std::vector<int> speeds;
   std::vector<int> speeds2;
   std::vector<int> speeds3;


  Timer timer;
  timer.placeMark();
  json j;
  json i;

  while(timer.getDtFromMark() < 10000_ms){
    moveMotors();
    speeds.push_back(right_wheels_down.get_actual_velocity());
    speeds2.push_back(right_wheels_up.get_actual_velocity());
    speeds3.push_back(rotation_arm.get_actual_velocity());
    std::cout <<  right_wheels_down.get_actual_velocity() << "Motor right down";
    std::cout <<  right_wheels_up.get_actual_velocity() << "Motor right up";
    std::cout <<  rotation_arm.get_actual_velocity() << "Motor arm";
    //std::printf("%f", right_wheels_down.get_actual_velocity());
    pros::delay(30);
  }
  stopMotors();

  organize.push_back(speeds);
  organize2.push_back(speeds2);
  organize3.push_back(speeds3);
  motors.push_back(organize);
  motors.push_back(organize2);
  motors.push_back(organize3);
  j["Motors"] = motors;

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
