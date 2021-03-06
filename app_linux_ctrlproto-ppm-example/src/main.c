
/**
 * \file main.c
 * \brief Example Master App for Profile Position (on PC)
 * \author Pavan Kanajar <pkanajar@synapticon.com>
 * \author Christian Holl <choll@synapticon.com>
 * \version 1.0
 * \date 10/04/2014
 */

/*
 * Copyright (c) 2014, Synapticon GmbH
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Execution of this software or parts of it exclusively takes place on hardware
 *    produced by Synapticon GmbH.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the Synapticon GmbH.
 *
 */

#include <ctrlproto_m.h>
#include <ecrt.h>
#include "ethercat_setup.h"
#include <stdio.h>
#include <stdbool.h>
#include "profile.h"
#include "drive_function.h"
#include <motor_define.h>
#include <sys/time.h>
#include <time.h>



int main()
{
	int flag_position_set = 0;

	int actual_position = 0;			// ticks
	int target_position = 350;			// ticks
	int tolerance = 35;	 				// ticks
	int actual_velocity;				// rpm
	float actual_torque;				// mNm
	int slave_number = 0;
	int stop_position;
	int ack = 0;
	int flag = 0;
	int i = 0;

	/* Initialize Ethercat Master */
	init_master(&master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);

	/* Initialize torque parameters */
	initialize_torque(slave_number, slv_handles);

	/* Initialize all connected nodes with Mandatory Motor Configurations (specified under config/motor/)*/
	init_nodes(&master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);

	/* Initialize the node specified with slave_number with PP configurations (specified under config/motor/)*/
	set_operation_mode(PP, slave_number, &master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);

	/* Enable operation of node in PP mode */
	enable_operation(slave_number, &master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);

	ack = 0;
	i = 0;
	while(1)
	{
		/* Update the process data (EtherCat packets) sent/received from the node */
		pdo_handle_ecat(&master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);

		if(master_setup.op_flag)	/*Check if the master is active*/
		{
			/* Read Actual Position from the node */
			if(flag == 0)
			{
				 actual_position = get_position_actual_ticks(slave_number, slv_handles);

				 i = i+1;
				 if(i>3)
				 {
					 target_position =  actual_position + 10000;
					 if(target_position > 52000)
						 target_position = 52000;
					 flag = 1;
					 printf("target_position %d actual_position %d\n", target_position, actual_position);
				 }
			}
			if(flag == 1)
			{
				/* Send target position for the node specified by slave_number */
				set_profile_position_ticks(target_position, slave_number, slv_handles);
				//printf("\n pos %d target_pos %d tol %d", actual_position,target_position, tolerance);

				/* Check if target position is reached with specified tolerance */
				ack = target_position_reached(slave_number, target_position, tolerance, slv_handles);

				/* Read actual node sensor values */
				actual_position = get_position_actual_ticks(slave_number, slv_handles);
				actual_velocity = get_velocity_actual_rpm(slave_number, slv_handles);
				actual_torque = get_torque_actual_mNm(slave_number, slv_handles);
				printf("Position: %d Velocity: %d Torque: %f ack: %d\n", actual_position, actual_velocity, actual_torque, ack);
			}
		}
		if(ack == 1)
		{
			break;
		}

	}

	printf("reached \n");

	flag_position_set = 0;
	target_position = actual_position - 10000;
	if(target_position < -52000)
		target_position = -52000;
	stop_position = target_position + 4000;

	while(1)
	{
		pdo_handle_ecat(&master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);

		if(master_setup.op_flag)	// Check if the master is active
		{
			set_profile_position_ticks(target_position, slave_number, slv_handles);
			flag_position_set = position_set_flag(slave_number, slv_handles); 		//ensures the new way point is taken awhen ack = 0;
			actual_position = get_position_actual_ticks(slave_number, slv_handles);

			printf("position %d ack %d   position_set_flag %d\n", actual_position, ack, flag_position_set);
		}
		if(flag_position_set == 1)
		{
			printf("\nexecuting \n");
			break;
		}
	}

	ack = 0;
	while(!ack)
	{
		pdo_handle_ecat(&master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);
		if(master_setup.op_flag)	// Check if the master is active
		{
			actual_position = get_position_actual_ticks(slave_number, slv_handles);
			if(actual_position < stop_position)
			{
				/* Quick stop position mode (for emergency) */
				quick_stop_position(slave_number, &master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);
				ack = 1;
			}
			actual_velocity = get_velocity_actual_rpm(slave_number, slv_handles);
			actual_torque = get_torque_actual_mNm(slave_number, slv_handles);
			printf("Position: %d Velocity: %d Torque: %f ack: %d\n", actual_position, actual_velocity, actual_torque, ack);
		}
	}
	printf("reached \n");


	/* Regain control of node to continue after quick stop */
	renable_ctrl_quick_stop(PP, slave_number, &master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);

	set_operation_mode(PP, slave_number, &master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);

	enable_operation(slave_number, &master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);

	/* Shutdown node operations */
	shutdown_operation(PP, slave_number, &master_setup, slv_handles, TOTAL_NUM_OF_SLAVES);

	return 0;
}


