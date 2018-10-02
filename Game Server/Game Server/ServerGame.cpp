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

void ServerGame::update()
{
	// get new clients
	if (network->acceptNewClient(client_id))
	{
		printf("client %d has been connected to the server\n", client_id);

		client_id++;

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


					break;

				case MESSAGE:

						printf(packet.data, "\n");
					
						//sendMessage(iter->first, MESSAGE, "Hello Back");
			
					break;

				case INPUT_DATA:
					parsedData = Tokenizer::tokenize(',', packet.data);

					handleInputData(parsedData);
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

	strcpy_s(packet.data, message.c_str());

	packet.packet_type = packetType;

	const unsigned int packet_size = sizeof(packet);
	char packet_data[packet_size];

	packet.serialize(packet_data);

	network->sendTo(packet_data, packet_size, clientID);
}
//Index 0 is the player num 
//Index 1 is the key pushed
void ServerGame::handleInputData(const std::vector<std::string>& data)
{
	float x = 0;
	float y = 0;

	int playerNum = std::stoi(data[0]);

	switch ((data[1])[0])
	{
	case 'a':
		x = -0.1f;
		
		break;
	case 's':
		y = -0.1f;
		
		break;
	case 'w':
		y = 0.1f;
		
		break;
	case 'd':
		x = 0.1f;
	
		break;
	default:
		break;
	}

	sendMessage(0, POSITION_DATA, std::to_string(playerNum) + "," + to_string(x) + "," + to_string(y) + ","); //send vecoter to add;
		if (network->sessions.size() > 1) {
			sendMessage(1, POSITION_DATA, std::to_string(playerNum) + "," + to_string(x) + "," + to_string(y) + ","); //send vecoter to add;
		}

}

