#include "main.h"

double atan2(double x, double y);

void initialize() {
	
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	Controller master (CONTROLLER_MASTER);
	Motor leftMotor (1, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor rightMotor (2, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
	Motor leftIntakeMotor (3, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor rightIntakeMotor (4, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
	Motor intakeAngleMotor (5, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	ADIDigitalIn limitSwitch(1);

	uint8_t intakeDir = 0;
	bool pressed = false;

	lv_obj_t * scr1 = lv_obj_create(NULL, NULL);
	lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);             /*Add to the active screen*/
	lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, -40);

	lv_obj_t * label = lv_label_create(btn1, NULL);                  /*Put on 'btn1'*/
	lv_label_set_text(label, "Click me"); 

	int timer = 0;

	while (true) {
		/*
		lcd::print(1, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		*/
		int leftVert = master.get_analog(ANALOG_LEFT_Y);
		int rightVert = master.get_analog(ANALOG_RIGHT_Y);
		int leftHoriz = master.get_analog(ANALOG_LEFT_X);
		int rightHoriz = master.get_analog(ANALOG_RIGHT_X);

		leftMotor.move(sqrt(rightHoriz*rightHoriz + rightVert*rightVert) * cos(atan2(-1 * rightHoriz, rightVert) + M_PI_4));
		rightMotor.move(sqrt(rightHoriz*rightHoriz + rightVert*rightVert) * sin(atan2(-1 * rightHoriz, rightVert) + M_PI_4));

		if(master.get_digital(DIGITAL_R1) && !limitSwitch.get_value()) {
			intakeAngleMotor.move(127);
		} else if(master.get_digital(DIGITAL_R2)) {
			intakeAngleMotor.move(-127);
		} else {
			intakeAngleMotor.move(0);
		}

		if(master.get_digital(DIGITAL_L1) && !pressed) {
			pressed = true;
			if(intakeDir != 1) intakeDir = 1;
			else intakeDir = 0;
		} else if(master.get_digital(DIGITAL_L2)) {
			intakeDir = -1;
			pressed = true;
		} else {
			if(intakeDir == -1) {
				intakeDir = 0;
			}
			pressed = false;
		}

		if(intakeDir == 1) {
			leftIntakeMotor.move(127 + leftHoriz);
			rightIntakeMotor.move(127 + leftHoriz);
		} else if(intakeDir == -1) {
			leftIntakeMotor.move(-127 + leftHoriz);
			rightIntakeMotor.move(-127 + leftHoriz);
		} else {
			leftIntakeMotor.move(0);
			rightIntakeMotor.move(0);
		}

		//lcd::print(2, "%d %d", left, right);

		timer++;
		delay(2);
	}
}


// Gets angle of point from origin(doesn't count down from 90 after reaching it like atan)
double atan2(double x, double y) {
	if(x > 0) return atan(y / x);
	else if(x < 0 && y >= 0) return atan(y / x) + M_PI;
	else if(x < 0 && y < 0) return atan(y / x) - M_PI;
	else if(x == 0 && y > 0) return M_PI_2;
	else if(x == 0 && y < 0) return -1 * M_PI_2;
	
	return 0;
}