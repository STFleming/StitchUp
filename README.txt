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

##Docker file
The fastest way to get up and running with StitchUp is to use docker.
Make sure Docker is installed on your machine and type the following commands in the root directory of the project.

		docker build -t "stitchup_container" .
		docker run -ti stitchup_container /bin/bash

This will setup a container, install all the legup & stitchup required dependencies, builds legup, and patches in stitchup and builds it.


##Running example builds



