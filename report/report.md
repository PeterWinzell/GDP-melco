# Author
Peter Winzell MSc, Mitsubishi Electric, Gothenburg office. 2017-24-03

# Abstract

The ability to access vehicle data signals with a standardized API opens up for innovation and feature growth outside the vehicle maker. It is also facilitating application development within the car - when there is a standard way of accessing signals applications can target multiple vehicles across OEM brands. 

  The W3C Automotive Working Group is currently developing specifications allowing developers to "write applications running in a web browser or an embedded web runtime on a car’s infotainment system to access signals from vehicle sensors and to carry out actions via the vehicles actuators" [[1]](https://www.w3.org/blog/auto/2017/01/04/vehicle-data-interfaces/). The applications can also be developed complete apart from the web runtime as any application framework that supports web sockets will be enough. 
  
  This is a reference implementation of the W3C Vehicle Information Service specification [[2]](https://github.com/w3c/automotive/blob/gh-pages/vehicle_data/vehicle_information_service.html). The implementation is done on the GDP-11 [[3]](https://at.projects.genivi.org/wiki/pages/viewpage.action?pageId=14090346) platform targeting the RPI3 as its primary SOC. It is an open source project where the server is implemented with the Qt framework and added to the yocto based GDP-11 software platform.
  
# Architecture and Design

The goal with this implementation is to provide the W3C VISS specification with a reference. This means that we are prioritizing specification feature implementation and will not enforce any performance optimizing on this design. Furthermore , we are not imposing nor are we suggesting any other security layers other than those specified.  For production both performance optimization and layered security would have to be added.

When selecting the Qt platform as the basis for the server implementation we argued that the Qt framework since v 5.4 inlcudes a web socket api, and since the Qt platform is currently one of the prefered application frameworks on Linux based infotainment platforms is a natural choice. Furthermore, Qt allows for rapid testing and development outside the actual target platform - it is a cross-platform development tool.

Figure 1 shows the basic architectural design with applications on top and the signal sources at the bottom, or if you prefer applications in the north and signals sources south.

![Server Arch](W3CServerArch.png)<br>
*Fig 1. W3C ref implementation architecture, (VSIAdapter migh be inluded in actual server process)*

One key benefit to the W3C VISS specification and its use of web sockets is that any application framework that supports web sockets is available for developers - they are not limited to the web browser and javascript/HTML5. Most application frameworks have support for web sockets today and we will illustrate this with a number of different applications running  across different devices. The application - if it is allowed to interact with the exposed network upon which the w3c server is running - can reside anywhere. It is also true that in theory the w3c server could also reside outside the vehicle and just interface the signal sources through an ip based communication channel. This independency opens up many other use cases - the vehicle is not longer just a connected vehicle, it is actually part of the cloud infrastructure itself. For this implementation we are mainly considering a traditional approach where the w3c server is running inside the main infotainment unit. 

The VISS specification identifies 7 different requests that are sent from a client using wss (secure web socket protocol) using a json based request protocol.

``` json
client -> {
		"action": "get",
		"path": "Signal.Drivetrain.InternalCombustionEngine.RPM",
		"requestId": "8756"
	}

	receive <- {
		"action": "get",
		"requestId": "8756",
		"value": 2372,
		"timestamp": <DOMTimeStamp>
	}
```
<br>

The basic server design is to spawn an independent thread through a thread pool mechanism for each client request. The following requests are defined in the specification: GET,SET,GETVSS,SUBSCRIBE, UNSUBSCRIBE, UNSUBSCRIBEALL, AUTHORIZE. The thread design will allow a request to run independently of any other client requests. Communication between threads is handled using the Qt signal and slot mechanism[[4]](http://doc.qt.io/qt-4.8/signalsandslots.html): for example when we have an unsubscribe request that needs to inform the corresponding subscription that it should stop sending data back to the client and terminate the subscription thread.

Figure 2 shows the flow through the components when subscribing to a signal which is interfacing openDS[[5]](https://www.opends.eu).

![Signal flow](signalFlow-3.png)<br>
*Fig 2. Basic signal flow through W3C web socket server*


The server south bound interface uses a tcp socket solution to collect vehicle data signals and promote to the right source. 


# Usability According to Spec

 We are able to use with a limited amount of vehicle signals using openDS car simulation and ODB signals that specification is impementable. We 
# Tests and Demo applications
# Future work
# References
[1] https://www.w3.org/blog/auto/2017/01/04/vehicle-data-interfaces/

[2] https://github.com/w3c/automotive/blob/gh-pages/vehicle_data/vehicle_information_service.html

[3] https://at.projects.genivi.org/wiki/pages/viewpage.action?pageId=14090346
