	 ________  _________  ___  _________  ________  ___  ___  ___  ___  ________   
	|\   ____\|\___   ___\\  \|\___   ___\\   ____\|\  \|\  \|\  \|\  \|\   __  \  
	\ \  \___|\|___ \  \_\ \  \|___ \  \_\ \  \___|\ \  \\\  \ \  \\\  \ \  \|\  \ 
	 \ \_____  \   \ \  \ \ \  \   \ \  \ \ \  \    \ \   __  \ \  \\\  \ \   ____\
	  \|____|\  \   \ \  \ \ \  \   \ \  \ \ \  \____\ \  \ \  \ \  \\\  \ \  \___|
	    ____\_\  \   \ \__\ \ \__\   \ \__\ \ \_______\ \__\ \__\ \_______\ \__\   
	   |\_________\   \|__|  \|__|    \|__|  \|_______|\|__|\|__|\|_______|\|__|   
	   \|_________|                                                                
                                                                               
StitchUp is a plugin to the LegUp High Level Synthesis tool that enables the generation of FPGA circuits with fault tolerant control decisions.

Verison 1.0 : release date 06/06/2016

--------------  Docker file  ---------------------
The fastest way to get up and running with StitchUp is to use docker.
Make sure Docker is installed on your machine and type the following commands in the root directory of the project.

		docker build -t "stitchup_container" .
		docker run -ti stitchup_container /bin/bash

This will setup a container, install all the legup & stitchup required dependencies, download legup, build legup, and patch in stitchup.

---------------  Running example builds ---------------------------

Examples are contained in the directory examples/.
To build one of the examples navigate to that directory and type make.

In the docker image the examples build a verilog file of the StitchUp protected circuit and an AXI wrapper for instantiating them in a Xilinx device.
The build script does not wrap all the components into an IP core for use in Vivado, however this is possible if the Xilinx build tools are installed.


---------------  Contact and about ------------------------- 
Any queries about using the tool feel free to contact me at: shane.fleming06@imperial.ac.uk
Written by, Shane T. Fleming.

