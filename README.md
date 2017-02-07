# GDP-melco
The W3C Websocket implementation. The W3CClient is a HMTL client developed using Netbeans 8.1. The server is implemented in Qt 5.6 and tested on GDP-11 (Genivi Linux) on RPI3.

Note: To get qtwebsockets on the GDP-11 image use IMAGE_INSTALL_append in local.conf. See Yocto and Genivi for further info.

The server uses a self-signed certificate. The trick is to run W3CTestClient is to call https://server-ip:8080 and have the browser to trust the certifcate provided by the W3CServer. The server is started with -secure param to enable wss.


