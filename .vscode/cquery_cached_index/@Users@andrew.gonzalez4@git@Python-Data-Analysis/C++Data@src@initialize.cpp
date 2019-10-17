 #include "main.h"
 #include <vector>
 #include <sstream>
 #include <string>
 using namespace okapi;

   int* definitions(int motors){

   #define MAX_SPEED 100
   #define LEFT_WHEEL_DOWN 2
   #define RIGHT_WHEEL_DOWN 1
   #define LEFT_WHEEL_UP 5
   #define RIGHT_WHEEL_UP 4
   std::vector<std::vector<int> > velocities;
   /////////////////////////////
   pros::Motor left_wheels_down (LEFT_WHEEL_DOWN);
   pros::Motor right_wheels_down (RIGHT_WHEEL_DOWN, true);//true means it goes reversed.
   pros::Motor left_wheels_up (LEFT_WHEEL_UP);
   pros::Motor right_wheels_up (RIGHT_WHEEL_UP, true);//true means it goes reversed.
   ////////////////////////////
   right_wheels_down.move_relative(1000,MAX_SPEED);
   left_wheels_down.move_relative(1000,MAX_SPEED);
   right_wheels_up.move_relative(1000,MAX_SPEED);
   left_wheels_up.move_relative(1000,MAX_SPEED);

   std::vector<int> LDVelocity;
   std::vector<int> LUVelocity;
   std::vector<int> RDVelocity;
   std::vector<int> RUVelocity;

   LDVelocity.push_back(left_wheels_down.get_actual_velocity());
   LUVelocity.push_back(left_wheels_up.get_actual_velocity());
   RDVelocity.push_back(right_wheels_down.get_actual_velocity());
   RUVelocity.push_back(right_wheels_up.get_actual_velocity());

   velocities.push_back(LDVelocity);
   velocities.push_back(LUVelocity);
   velocities.push_back(RDVelocity);
   velocities.push_back(RUVelocity);

   return velocities;
 }

void copytoJSON(){
  std::vector<int*> buffer;
  Timer timer;
  timer.placeMark();

  while(timer.getDtFromMark() < 10000_ms){
    buffer.push_back(definitions(4));
    pros::delay(20);
  }
}

void initialize()
{
}

void disabled() {}

void competition_initialize() {}
