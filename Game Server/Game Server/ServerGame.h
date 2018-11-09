#pragma once
#include "ServerNetwork.h"
#include "Packet.h"
#include <vector>
#include "ClientPair.h"
#include "Tokenizer.h"
#include "Player.h"
#include "BoxCollider.h"

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

	//collision stuffs
	std::vector<BoxCollider> collisionBoxes;

	glm::vec3 prevPosition1 = {0.0f,0.0f,0.0f};
	glm::vec3 prevPosition2 = {0.0f, 0.0f, 0.0f};

	void pairClients(int);
	void sendMessage(int clientid, int message_type, std::string message);
	bool collisionCheck(Player);

	void handleIncomingKey(const std::vector<std::string>&);
	void handleIncomingTransformation(const std::vector<std::string>&);
	void handleIncomingCollider(const std::vector<std::string>&);

	unsigned int currentClient;

	bool start = false;


	void handleJump( int player);
	void handleAttack(int player);
	void restart();
};