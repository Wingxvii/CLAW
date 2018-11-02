#include "MessageHandler.h"

void MessageHandler::sendInitConnection(ClientNetwork * network, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int playerNum)
{
	std::string message = std::to_string(playerNum) + "," + std::to_string(position.x) + "," + std::to_string(position.y) + "," + std::to_string(position.z)
		+ "," + std::to_string(rotation.x) + "," + std::to_string(rotation.y) + "," + std::to_string(rotation.x)
		+ "," + std::to_string(scale.x) + "," + std::to_string(scale.y) + "," + std::to_string(scale.y) + ",";

	// send init packet
	sendToServer(network, INIT_CONNECTION, message);
}

void MessageHandler::sendMessage(ClientNetwork * network, std::string message)
{
	// send a simple Message
	sendToServer(network, MESSAGE, message);
}

void MessageHandler::sendKeyInput(ClientNetwork * network, char key, int playerNum)
{
	std::string message = std::to_string(playerNum) + "," + std::to_string(key) + ",";
	sendToServer(network, KEY_INPUT, message);

}

void MessageHandler::sendTransformationInput(ClientNetwork * network, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int playerNum)
{
	std::string message = std::to_string(playerNum) + "," + std::to_string(position.x) + "," + std::to_string(position.y) + "," + std::to_string(position.z)
		+ "," + std::to_string(rotation.x) + "," + std::to_string(rotation.y) + "," + std::to_string(rotation.x)
		+ "," + std::to_string(scale.x) + "," + std::to_string(scale.y) + "," + std::to_string(scale.y) + ",";


	sendToServer(network, TRANSFORMATION_DATA, message);
}

void MessageHandler::sendBoundingBoxInfo(ClientNetwork * network, std::vector<PhysicalEntity* > e)
{
	std::string message = "";

	for (int i = 0; i < e.size(); i++) {
		glm::mat4 transform = glm::mat4(1.0);
		transform = glm::translate(glm::mat4(1), e[i]->getMesh()->BoundingBox.m_center);
		transform = transform * glm::scale(glm::mat4(1), e[i]->getMesh()->BoundingBox.m_size);
		glm::mat4 m = e[i]->getMesh()->transform->getLocalToWorldMatrix() * transform;
		glm::vec3 position = { m[0][3], m[1][3], m[2][3] };

		if (e[i]->m_entityType = (int)EntityTypes::PLAYER) {
			message = std::to_string(e[i]->m_entityType) + "," + std::to_string(e[i]->getMesh()->BoundingBox.m_size.x) + "," + std::to_string(e[i]->getMesh()->BoundingBox.m_size.y) +
				"," + std::to_string(e[i]->getMesh()->BoundingBox.m_size.z) + "," + std::to_string(e[i]->getMesh()->BoundingBox.m_size.x / 2) + "," +
				std::to_string(e[i]->getMesh()->BoundingBox.m_size.y / 2) + "," + std::to_string(e[i]->getMesh()->BoundingBox.m_size.z / 2) + ",";
		}
		else {

			
			
			message = std::to_string(e[i]->m_entityType) + "," + std::to_string(e[i]->getMesh()->BoundingBox.m_size.x) + "," + std::to_string(e[i]->getMesh()->BoundingBox.m_size.y) +
				"," + std::to_string(e[i]->getMesh()->BoundingBox.m_size.z) + "," + std::to_string(position.x) + "," +
				std::to_string(position.y) + "," + std::to_string(position.z) + ",";
		}

		sendToServer(network, LOAD_COLLISIONS, message);


		
	}

	




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
