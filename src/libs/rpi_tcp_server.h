/* 
A simple server for Raspberry Pi application
rpi_tcp_server.cpp
H. Shin
Korea Optron Corp.
2019-05-27

Adapted from D. Thiebaut, http://www.science.smith.edu/dftwiki/index.php/Tutorial:_Client/Server_on_the_Raspberry_Pi
The port number used in 58262
The server waits for a connection request from a client.
The server assumes the client will send positive integers, which it sends back multiplied by 2.
If the server receives -1 it closes the socket with the client.
If the server receives -2, it exits.
*/
