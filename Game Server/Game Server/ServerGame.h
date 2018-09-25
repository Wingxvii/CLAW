#pragma once
#include "ServerNetwork.h"
#include "Packet.h"
#include <vector>
#include "ClientPair.h"
class ServerGame
{

public:

	ServerGame(void);
	~ServerGame(void);

	void update();
	void receiveFromClients();

	std::vector<ClientPair*> pairs;

private:

	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

	void sendActionPackets();

	void pairClients(int);
	void sendMessage(std::string message, int id);
};