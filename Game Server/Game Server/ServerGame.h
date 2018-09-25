#pragma once
#include "ServerNetwork.h"
#include "Packet.h"

class ServerGame
{

public:

	ServerGame(void);
	~ServerGame(void);

	void update();
	void receiveFromClients();

private:

	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

	void sendActionPackets();
};