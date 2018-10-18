#pragma once

#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <string>

#include "ClientNetwork.h"
#include "Packet.h"

#include "glm/ext.hpp"
class MessageHandler
{
public:
	//Init is used as a simple ping to make sure connection is established
	static void sendInitConnection(ClientNetwork* network);
	//a Message is a simple string with that the server can print to it's console 
	static void sendMessage(ClientNetwork* network, std::string message);
	//This sends a keycode of a key that was pressed which will allow the server to process 
	static void sendKeyInput(ClientNetwork* network, int keycode, int playerNum);
	//This is for keys related to movement and send which key was pusehd along with the position of the player
	static void sendMovementInput(ClientNetwork* network, int keycode, glm::vec3 currentPosition, int playerNum);

private: 
	static void sendToServer(ClientNetwork* network, int packet_type, std::string message);
};

