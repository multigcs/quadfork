
Changes again's Autoquad:

	hardware:
		better support for free openfc hardware (http://www.openfc.de/)

	buildysystem:
		moving to a free gcc-compiler-enviroment
		new directory structur

	mavlink:
		report number of satelites
		Follow-ME (Ardupilot style)
		report current (Ampere)
		set ROI in missions

	gps:
		read number of satelites

	parameters:
		CTRL_BATT_COMP: 
		IMU_MPU_LPF: 

	dimu:
		more debug output
		lpf setup for mpu6000

	nav:
		initial roi support



Build for OpenFC 0.90:

	make BOARD_TYPE=1 BOARD_VER=90 clean all
	ls build/autoquad.hex

Build for OpenFC 0.91:

	make BOARD_TYPE=1 BOARD_VER=91 clean all
	ls build/autoquad.hex


