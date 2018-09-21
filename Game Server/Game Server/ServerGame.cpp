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
		//pairClients(client_id);
	}

	receiveFromClients();
}

void ServerGame::receiveFromClients()
{
	Packet packet;

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

					//sendActionPackets();

					break;

				case MESSAGE:

					//printf(packet.message.c_str(), "\n");

				    //sendActionPackets();

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

//void ServerGame::pairClients(int id)
//{
//	if (pair[0] == -1) {
//		pair[0] = id;
//		//sendMessage("You are player 1", pair[0]);
//	}
//	else {
//		pair[1] = id;
//		//sendMessage("You are player 2", pair[1]);
//	}
//
//}

//void ServerGame::sendMessage(std::string message, int id)
//{
//	Packet packet;
//	packet.message = "sdfsddsffdsfsdf";
//
//	const unsigned int packet_size = sizeof(packet);
//	char packet_data[packet_size];
//
//
//	packet.packet_type = MESSAGE;
//
//	packet.serialize(packet_data);
//
//
//	network->sendTo(packet_data, packet_size, id);
//}

