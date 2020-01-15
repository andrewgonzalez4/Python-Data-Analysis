#include "main.h"
#include "dataAcquisition.hpp"
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


void initialize() {

	 pros::lcd::initialize();
	 pros::lcd::set_text(1, "Initialize");



}
void disabled() {}
void competition_initialize() {}
void autonomous() {}
void opcontrol() {}
