 #include "main.h"
 //#include "vector.h"
 using namespace okapi;

 void definitions(bool isUsing){
   #define MAX_SPEED 100
   #define LEFT_WHEEL_DOWN 2
   #define RIGHT_WHEEL_DOWN 1
   #define LEFT_WHEEL_UP 5
   #define RIGHT_WHEEL_UP 4
   std::string Buffer;



   pros::Motor left_wheels_down (LEFT_WHEEL_DOWN);
   pros::Motor right_wheels_down (RIGHT_WHEEL_DOWN, true);//true means it goes reversed.
   pros::Motor left_wheels_up (LEFT_WHEEL_UP);
   pros::Motor right_wheels_up (RIGHT_WHEEL_UP, true);//true means it goes reversed.

   right_wheels_down.move_relative(1000,MAX_SPEED);
   left_wheels_down.move_relative(1000,MAX_SPEED);
   right_wheels_up.move_relative(1000,MAX_SPEED);
   left_wheels_up.move_relative(1000,MAX_SPEED);

  Timer timer;
  timer.placeMark();


  while(timer.getDtFromMark() < 10000_ms){

    pros::delay(20);

  }
  //Speed
  std::cout << "Motor Speeds are:";
  std::cout << left_wheels_down.get_actual_velocity();
  std::cout << right_wheels_down.get_actual_velocity();
  std::cout << left_wheels_up.get_actual_velocity();
  std::cout << right_wheels_up.get_actual_velocity();
 }

 void getSpeed(){

 }

void initialize()
{
definitions(true);
}

void disabled() {}

void competition_initialize() {}
