#include "MessageHandler.h"

void MessageHandler::sendInitConnection(ClientNetwork * network) 
{
	// send init packet
	sendToServer(network, INIT_CONNECTION, "");
}

void MessageHandler::sendMessage(ClientNetwork * network, std::string message)
{
	// send a simple Message
	sendToServer(network, MESSAGE, message);
}

void MessageHandler::sendKeyInput(ClientNetwork * network, int keycode, int playerNum)
{
	std::string message = std::to_string(playerNum) + "," + std::to_string(keycode) + ",";
	sendToServer(network, INPUT_DATA, message);

}

void MessageHandler::sendMovementInput(ClientNetwork * network, int keycode, glm::vec3 currentPosition, int playerNum)
{
	std::string message = std::to_string(playerNum) + "," + std::to_string(keycode) + "," + std::to_string(currentPosition.x) + "," +
		std::to_string(currentPosition.y) + "," + std::to_string(currentPosition.z) + ",";
	sendToServer(network, POSITION_DATA, message);
}


void MessageHandler::sendToServer(ClientNetwork* network, int packet_type, std::string message)
{
	Packet packet;

	strcpy_s(packet.data, message.c_str() + '\0');

	packet.packet_type = packet_type;

	const unsigned int packet_size = sizeof(packet);
	char packet_data[packet_size];

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}
