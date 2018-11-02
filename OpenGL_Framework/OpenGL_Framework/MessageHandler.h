#pragma once

#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <string>

#include "ClientNetwork.h"
#include "Packet.h"

#include "glm/ext.hpp"

#include "PhysicalEntity.h"
class MessageHandler
{
public:
	//Init is used as a simple ping to make sure connection is established
	static void sendInitConnection(ClientNetwork* network, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int playerNum);
	//a Message is a simple string with that the server can print to it's console 
	static void sendMessage(ClientNetwork* network, std::string message);
	//This sends a keycode of a key that was pressed which will allow the server to process 
	static void sendKeyInput(ClientNetwork* network, char keycode, int playerNum);

	static void sendTransformationInput(ClientNetwork* network, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int playerNum);

	static void sendBoundingBoxInfo(ClientNetwork* network, std::vector<PhysicalEntity* > e);
private: 
	static void sendToServer(ClientNetwork* network, int packet_type, std::string message);
};

