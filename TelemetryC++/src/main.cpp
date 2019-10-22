#include "main.h"
#include <vector>
using namespace okapi;
#include "json.hpp"

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

std::vector<std::vector<int>> definitions(int num){
	std::vector<std::vector<int>> index;
	std::vector<std::vector<int>> motors;

	#define MAX_SPEED 100
	#define LEFT_WHEEL_DOWN 2
	#define RIGHT_WHEEL_DOWN 1
	#define LEFT_WHEEL_UP 5
	#define RIGHT_WHEEL_UP 4
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

	std::vector<int> rwd;
	std::vector<int> rwu;
	std::vector<int> lwd;
	std::vector<int> lwu;

	rwd.push_back(right_wheels_down.get_actual_velocity());
	rwu.push_back(right_wheels_up.get_actual_velocity());
	lwd.push_back(left_wheels_down.get_actual_velocity());
	lwu.push_back(left_wheels_up.get_actual_velocity());

	motors.push_back(rwd);
	motors.push_back(rwu);
	motors.push_back(lwd);
	motors.push_back(lwu);

	index.push_back(rwd);
	index.push_back(rwu);
	index.push_back(lwd);
	index.push_back(lwu);
	return index;
}

void initialize() {


 void copytoJSON(){
   std::vector<std::vector<std::vector<int>>> buff;
   Timer timer;
   timer.placeMark();

   while(timer.getDtFromMark() < 10000_ms){
     buff.push_back(definitions(4));
     pros::delay(20);
   }
 }

 void initialize()
 {
   copytoJSON();
 }

 void disabled() {}

 void competition_initialize() {}

}

void disabled() {}
void competition_initialize() {}
void autonomous() {}
void opcontrol() {
	#include "main.h"
	#include <vector>
	#include <sstream>
	#include <string>
	#include "json.hpp"
	using namespace okapi;
	#define MAX_SPEED 100
	#define LEFT_WHEEL_DOWN 2
	#define RIGHT_WHEEL_DOWN 1
	#define LEFT_WHEEL_UP 5
	#define RIGHT_WHEEL_UP 4
	std::array<std::array<std::vector<float>, 2>, 2> whlmtrs;
	pros::Motor left_wheels_down (LEFT_WHEEL_DOWN);
	pros::Motor right_wheels_down (RIGHT_WHEEL_DOWN, true);//true means it goes reversed.
	pros::Motor left_wheels_up (LEFT_WHEEL_UP);
	pros::Motor right_wheels_up (RIGHT_WHEEL_UP, true);//true means it goes reversed.

	std::vector<std::vector<int>> definitions(int num){
		int motors[2][2] = {
			{0,0},
			{0,0}
		};

		motors[0][0] = right_wheels_up.get_position();
		motors[0][1] = right_wheels_up.get_actual_velocity();

		motors[1][0] = left_wheel_up.get_position();
		motors[1][1] = left_wheel_up.get_actual_velocity();

		return motors;
	}

	void runMotors(){
		right_wheels_down.move_voltage(12000);
		left_wheels_down.move_voltage(12000);
		right_wheels_up.move_voltage(12000);
		left_wheels_up.move_voltage(12000);
	}
	void copytoJSON(){
		std::vector<int> buff;
		Timer timer;
		timer.placeMark();
		while(timer.getDtFromMark() < 10000_ms){
			int output[2][2] = definitions(4);
			for(int motor = 0; motor < 2; motor++){
				whlmtrs[motor][0].push_back(output[motor][0])
			}

			pros::delay(20);
		}
	}
	void opcontrol() {
		while (true) {
			runMotors();
			copytoJSON();
		}
	}

}
