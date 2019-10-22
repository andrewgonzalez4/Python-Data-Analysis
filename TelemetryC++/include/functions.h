#include "main.h"
#include <vector>
using namespace okapi;
#include "json.hpp"
#define MAX_SPEED 100
#define LEFT_WHEEL_DOWN 2
#define RIGHT_WHEEL_DOWN 1
#define LEFT_WHEEL_UP 5
#define RIGHT_WHEEL_UP 4

pros::Motor left_wheels_down (LEFT_WHEEL_DOWN);
pros::Motor right_wheels_down (RIGHT_WHEEL_DOWN, true);//true means it goes reversed.
pros::Motor left_wheels_up (LEFT_WHEEL_UP);
pros::Motor right_wheels_up (RIGHT_WHEEL_UP, true);//true means it goes reversed.


  void moveMotors(){
   right_wheels_down.move_voltage(12000);
 }

 void stopMotors(){
   right_wheels_down.move_voltage(0);
 }

 void convert(){
   std::vector<std::vector<std::vector<int>>> motors;
   std::vector<std::vector<int>> organize;
   std::vector<int> speeds;

  Timer timer;
  timer.placeMark();

  while(timer.getDtFromMark() < 10000_ms){
    moveMotors();
    speeds.push_back(right_wheels_down.get_actual_velocity());
    std::printf("%f", right_wheels_down.get_actual_velocity());
    pros::delay(30);
  }
  stopMotors();

  organize.push_back(speeds);
  motors.push_back(organize);

}
