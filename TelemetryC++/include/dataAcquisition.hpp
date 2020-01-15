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

pros::Motor left_wheels_down (LEFT_WHEEL_DOWN);
pros::Motor right_wheels_down (RIGHT_WHEEL_DOWN, true);//true means it goes reversed.
pros::Motor left_wheels_up (LEFT_WHEEL_UP);
pros::Motor right_wheels_up (RIGHT_WHEEL_UP, true);//true means it goes reversed.
pros::Motor rotation_arm (ROTATION_ARM);

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
