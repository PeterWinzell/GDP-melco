# GDP-melco
The W3C Websocket implementation. The W3CClient is a HMTL client developed using Netbeans 8.1. The server is implemented in Qt 5.6 and tested on GDP-11 (Genivi Linux) on RPI3.

Note: To get qtwebsockets on the GDP-11 image use IMAGE_INSTALL_append in local.conf. See Yocto and Genivi for further info.

The server uses a self-signed certificate. The trick is to run W3CTestClient is to call https://server-ip:8080 and have the browser to trust the certifcate provided by the W3CServer. The server is started with -secure param to enable wss.

# Settings file
The W3CServer uses a settings file (W3CServer.ini) to configure both W3CServer and OpenDS, a default settings file is included in the compiled binary and will be copied to default system path IF the settings file doesnt already exists. On linux system the settings file would be found under ~/.config/MelcoGOT


# OpenDS
Open Driving Simulator, https://www.opends.eu/ is a open source dringing simulator that has an xml API for remote access of car signals using tcp communication.

In order to retrieve car signals from the OpenDS software you need to install and configure the W3CServer to connect to the computer running the simulator, in W3CServer.ini settings file you can define both IP and port to be used aswell as what signals that you want to subscribe to.

Current checked in OpenDS version has this setup for the Village model using Village.xml settings file, you can find our OpenDS fork on https://github.com/PeterWinzell/car-simulation
