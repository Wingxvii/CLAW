#include "ServerGame.h"


ServerGame::~ServerGame()
{
}

#include "ServerGame.h"


unsigned int ServerGame::client_id;

ServerGame::ServerGame(void)
{
	// id's to assign clients for our table
	client_id = 0;

	// set up the server network to listen 
	network = new ServerNetwork();
}

//fixed update
void ServerGame::update()
{
	// get new clients
	if (network->acceptNewClient(client_id))
	{
		printf("client %d has been connected to the server\n", client_id);

		client_id++;

		if (client_id == 1) {
			p[1].active = true;
		}
		else if (client_id == 2) {
			p[2].active = true;
		}

		pairClients(client_id);
	}

	receiveFromClients();


}

void ServerGame::receiveFromClients()
{
	Packet packet;
	std::vector<std::string> parsedData;

	// go through all clients
	std::map<unsigned int, SOCKET>::iterator iter;

	for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
	{
		int data_length = network->receiveData(iter->first, network_data);
		
		if (data_length > 0)
		{

			int i = 0;
			while (i < (unsigned int)data_length)
			{
				packet.deserialize(&(network_data[i]));
				i += sizeof(Packet);

				switch (packet.packet_type) {

				case INIT_CONNECTION:

					printf("server received init packet from client\n");
					parsedData = Tokenizer::tokenize(',', packet.data);
					handleIncomingTransformation(parsedData);

					break;

				case MESSAGE:
						printf(packet.data, "\n");
					
						//sendMessage(iter->first, MESSAGE, "Hello Back");
			
					break;

				default:
					printf(network_data, "\n");

					printf("error in packet types\n");

					break;
				}
			}
		}
	}
}

void ServerGame::pairClients(int id)
{
	if ((pairs.size() == 0)) {
		ClientPair* c = new ClientPair();
		pairs.push_back(c);
	}
	else if ((pairs.size() > 0) && (pairs.back()->checkIfClient2Assigned())) {
		ClientPair* c = new ClientPair();
		pairs.push_back(c);
	}

	if (pairs.back()->getClient1() == -1) {
		pairs.back()->setClient1(id);
		sendMessage(pairs.back()->getClient1()-1, PLAYER_NUM, "1,");
	}
	else {
		pairs.back()->setClient2(id);
		sendMessage(pairs.back()->getClient2()-1, PLAYER_NUM, "2,");
	}

}

void ServerGame::sendMessage(int clientID, int packetType, std::string message)
{

	Packet packet;

	strcpy_s(packet.data, message.c_str() + '\0');

	packet.packet_type = packetType;

	const unsigned int packet_size = sizeof(packet);
	char packet_data[packet_size];

	packet.serialize(packet_data);

	network->sendTo(packet_data, packet_size, clientID);
}
//Index 0 is the player num 
//Index 1 is the key pushed
//Index 2-4 player position data
void ServerGame::handleIncomingKey(const std::vector<std::string>& data)
{

	int playerNum = std::stoi(data[0]);
	int keycode = std::stoi(data[1]);

	switch (keycode)
	{
	case 'a':
		p[playerNum].transform.position += 0.1f*p[playerNum].transform.getLeft();
		
		break;
	case 's':

		p[playerNum].transform.position += 0.1f*p[playerNum].transform.getBackward();

		break;
	case 'w':
		p[playerNum].transform.position += -0.1f*p[playerNum].transform.getBackward();

		break;
	case 'd':
		p[playerNum].transform.position += -0.1f*p[playerNum].transform.getLeft();

		break;

	case 'q':
		p[playerNum].transform.rotation.y += 1.0f;
		break;
	case 'e':
		p[playerNum].transform.rotation.y -= 1.0f;

		break;
	default:
		break;
	}

	sendMessage(0, TRANSFORMATION_DATA, std::to_string(playerNum) + "," + to_string(p[playerNum].transform.position.x) + "," + to_string(p[playerNum].transform.position.y)
		+ "," + to_string(p[playerNum].transform.position.z) + "," + to_string(p[playerNum].transform.rotation.x) + "," + to_string(p[playerNum].transform.rotation.y)
		+ "," + to_string(p[playerNum].transform.rotation.z) + "," + to_string(p[playerNum].transform.scale.x) + "," + to_string(p[playerNum].transform.scale.y)
		+ "," + to_string(p[playerNum].transform.scale.z) + ",");

		if (network->sessions.size() > 1) {
			sendMessage(1, TRANSFORMATION_DATA, std::to_string(playerNum) + "," + to_string(p[playerNum].transform.position.x) + "," + to_string(p[playerNum].transform.position.y)
				+ "," + to_string(p[playerNum].transform.position.z) + "," + to_string(p[playerNum].transform.rotation.x) + "," + to_string(p[playerNum].transform.rotation.y)
				+ "," + to_string(p[playerNum].transform.rotation.z) + "," + to_string(p[playerNum].transform.scale.x) + "," + to_string(p[playerNum].transform.scale.y)
				+ "," + to_string(p[playerNum].transform.scale.z) + ",");
		}
}

void ServerGame::handleIncomingTransformation(const std::vector<std::string>& data)
{
	int playerNum = std::stoi(data[0]);
	p[playerNum].transform.position.x = std::stoi(data[1]);
	p[playerNum].transform.position.y = std::stoi(data[2]);
	p[playerNum].transform.position.z = std::stoi(data[3]);
	p[playerNum].transform.rotation.x = std::stoi(data[4]);
	p[playerNum].transform.rotation.y = std::stoi(data[5]);
	p[playerNum].transform.rotation.z = std::stoi(data[6]);
	p[playerNum].transform.scale.x = std::stoi(data[7]);
	p[playerNum].transform.scale.y = std::stoi(data[8]);
	p[playerNum].transform.scale.z = std::stoi(data[9]);
}

