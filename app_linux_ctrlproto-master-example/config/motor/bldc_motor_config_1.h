#ifndef _MOTOR_1
#define _MOTOR_1
/*
 * define Motor Specific Constants
 */
#define POLE_PAIRS_1  				8
#define GEAR_RATIO_1 				26
#define MAX_NOMINAL_SPEED_1  		4000		// in rpm
#define MAX_NOMINAL_CURRENT_1  		2		// in A
#define MAX_ACCELERATION_1   		5000     // rpm/s
#define ENCODER_RESOLUTION_1 		4000		// Max count of Quadrature Encoder
#define POLARITY_1 					1					// 1 / -1

/*single variable Sensor selection code*/
#define QEI_WITH_INDEX				1
#define QEI_WITH_NO_INDEX 			0
#define QEI_SENSOR_TYPE  			QEI_WITH_INDEX//QEI_WITH_NO_INDEX
#define HALL 1
#define QEI  2
#define SENSOR_SELECTION_CODE_1       QEI

/*
 * control specific constants/variables
 */
//velocity control
#define VELOCITY_Kp_NUMERATOR_1 	 	5
#define VELOCITY_Kp_DENOMINATOR_1  	10
#define VELOCITY_Ki_NUMERATOR_1    	5
#define VELOCITY_Ki_DENOMINATOR_1  	100
#define VELOCITY_Kd_NUMERATOR_1   	0
#define VELOCITY_Kd_DENOMINATOR_1 	1

//position control

#define MAX_FOLLOWING_ERROR 		0
#define MAX_POSITION_LIMIT 			359
#define MIN_POSITION_LIMIT 			-359

#define POSITION_Kp_NUMERATOR 	 	180
#define POSITION_Kp_DENOMINATOR  	2000
#define POSITION_Ki_NUMERATOR    	50
#define POSITION_Ki_DENOMINATOR  	102000
#define POSITION_Kd_NUMERATOR    	100
#define POSITION_Kd_DENOMINATOR  	10000

#endif
