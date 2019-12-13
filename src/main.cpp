#include "main.h"

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
	Motor testMotor1 (3, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor testMotor2 (4, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
	Motor testMotor3 (5, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);

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
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);
		int leftHoriz = master.get_analog(ANALOG_LEFT_X);
		
		testMotor1.move(left);
		testMotor2.move(left);
		testMotor3.move(right);

		//lcd::print(2, "%d %d", left, right);
		

		if(!(timer % 25)) {
			master.print(0, 0, "%d", leftHoriz);
		}

		timer++;
		delay(2);
	}
}
