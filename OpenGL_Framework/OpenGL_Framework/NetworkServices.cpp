#include "NetworkServices.h"

int NetworkServices::sendMessage(SOCKET curSocket, char * message, int messageSize)
{
	//make sure to error check this in a real application 
	//send a socket object and its message to the server 
	//return an int value representing the number of bytes sucessfully sent 
	return send(curSocket, message, messageSize, 0);
}

int NetworkServices::receiveMessage(SOCKET curSocket, char * buffer, int bufSize)
{
	return recv(curSocket, buffer, bufSize, 0);
}