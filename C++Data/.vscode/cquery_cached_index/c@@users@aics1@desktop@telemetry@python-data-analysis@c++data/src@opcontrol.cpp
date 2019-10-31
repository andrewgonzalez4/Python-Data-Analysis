#include "main.h"
#include <vector>
#include <sstream>
#include <string>
#include "json.hpp"
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

	/////////////////////////////
	////////////////////////////

	// std::vector<int> rwd;
	// std::vector<int> rwu;
	// std::vector<int> lwd;
	// std::vector<int> lwu;

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
