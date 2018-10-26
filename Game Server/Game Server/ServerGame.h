#pragma once
#include "ServerNetwork.h"
#include "Packet.h"
#include <vector>
#include "ClientPair.h"
#include "Tokenizer.h"
#include "Player.h"

#include "include/glm/glm.hpp"
#include "include/glm/ext.hpp"

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

	Player p[2] = { Player(1),Player(2) };

	void pairClients(int);
	void sendMessage(int clientid, int message_type, std::string message);

	void handleIncomingKey(const std::vector<std::string>&);
	void handleIncomingTransformation(const std::vector<std::string>&);

	unsigned int currentClient;

};