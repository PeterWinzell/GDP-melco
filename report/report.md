# Author
Peter Winzell, Mitsubishi Electric, Gothenburg office. 2017-24-03

# Abstract

The ability to access vehicle data signals with a standardized API opens up for innovation and feature growth outside the vehicle maker. It is also facilitating application development within the car - when there is a standard way of accessing signals applications can target multiple vehicles across OEM brands. 
  The W3C Automotive Working Group is currently developing specifications allowing developers to "write applications running in a web browser or an embedded web runtime on a car’s infotainment system to access signals from vehicle sensors and to carry out actions via the vehicles actuators" [1](https://www.w3.org/blog/auto/2017/01/04/vehicle-data-interfaces/). The applications can also be developed complete apart from the web runtime as any application framework that supports web sockets will be enough. 
  This is a reference implementation of the W3C Vehicle Information Service specification [2](https://github.com/w3c/automotive/blob/gh-pages/vehicle_data/vehicle_information_service.html). The implementation is done on the GDP-11 platform targeting the RPI3 as its primary SOC. It is an open source project where the server is implemented with the Qt framework and added to the yocto based GDP-11 software platform.
  
# Archictecutre and Design
# Usability
# Tests and Demo applications
# Future work
# References
[1] https://www.w3.org/blog/auto/2017/01/04/vehicle-data-interfaces/
[2] https://github.com/w3c/automotive/blob/gh-pages/vehicle_data/vehicle_information_service.html
