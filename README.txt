## ________  _________  ___  _________  ________  ___  ___  ___  ___  ________   
##|\   ____\|\___   ___\\  \|\___   ___\\   ____\|\  \|\  \|\  \|\  \|\   __  \  
##\ \  \___|\|___ \  \_\ \  \|___ \  \_\ \  \___|\ \  \\\  \ \  \\\  \ \  \|\  \ 
## \ \_____  \   \ \  \ \ \  \   \ \  \ \ \  \    \ \   __  \ \  \\\  \ \   ____\
##  \|____|\  \   \ \  \ \ \  \   \ \  \ \ \  \____\ \  \ \  \ \  \\\  \ \  \___|
##    ____\_\  \   \ \__\ \ \__\   \ \__\ \ \_______\ \__\ \__\ \_______\ \__\   
##   |\_________\   \|__|  \|__|    \|__|  \|_______|\|__|\|__|\|_______|\|__|   
##   \|_________|                                                                
                                                                               
StitchUp is a plugin to the LegUp High Level Synthesis tool that enables the generation of fault tolerant FPGA circuits.

Verison 1.0 : release date 06/06/88

##Docker file
The fastest way to get up and running with StitchUp is to use docker.

docker build -t "stitchup_container" .
docker run -ti stitchup_container /bin/bash

## Installation
./patch {LegUp root directory} 

##Benchmarks
./benchmarks/chstone\_stitchup/build\_script will produce a graph in ./benchmarks/chstone\_stitchup/results/ of the post place and route
resource saving for duplicating control flow via StitchUp.


##Development Log

* 20/07/2015 - ./patch now works with legup 4.0
