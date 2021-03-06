Homing over EtherCAT Demo
=========================
<a href="https://github.com/synapticon/sc_sncn_motorctrl_sin/blob/master/SYNAPTICON.md">
<img align="left" src="https://s3-eu-west-1.amazonaws.com/synapticon-resources/images/logos/synapticon_fullname_blackoverwhite_280x48.png"/>
</a>
<br/>
<br/>

This application provides example Master Application for Homing Mode (host side). The nodes 
must be running test_ethercat-motorctrl-mode from sc_sncn_motorctrl_sin before the Master 
application runs.

In Homing mode we provide homing based on switches (positive switch/ negative switch). If 
configured homing based for positive switch: the node hunts for home switch in the positve 
direction. Once the home switch is found the node resets the position counter with the offset 
found.

Now the node is homed and we can continue with either Cyclic/Profile modes. In the example we
run Cyclic Synchronous position mode.

The motor configuration and control parameter for each node connected to the motor must be
specified at config/motor/bldc_motor_config_N.h

Dependencies: sc_sncn_motorctrl_sin, lib_linux_ctrlproto, lib_linux_motor_drive

NOTE: The application requires EtherCAT Master for Linux from IGH to be installed on your PC. 
The node configuration must be defined in ethercat_setup.h



