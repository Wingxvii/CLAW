#include "ServerGame.h"
#include <string>

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
			p[0].active = true;
		}
		else if (client_id == 2) {
			p[1].active = true;
		}

		pairClients(client_id);
	}

	receiveFromClients();

	if (start) {

		//update physics
		//move cube transform
		p[0].rigidbody.update();
		p[1].rigidbody.update();

//move cube collider & checks collisions
		if (p[0].collider) {
			p[0].collider->center += p[0].transform.position - prevPosition1;

			if (collisionCheck(p[0])) {
				//revert movement if collide		
				p[0].collider->center -= p[0].transform.position - prevPosition1;
				p[0].transform.position = prevPosition1;
			}
		}
		if (p[1].collider) {
			p[1].collider->center += p[1].transform.position - prevPosition2;

			if (collisionCheck(p[1])) {
				//revert movement if collide 
				p[1].collider->center -= p[1].transform.position - prevPosition2;
				p[1].transform.position = prevPosition2;
			}
		}


		//update last frame position
		prevPosition1 = p[0].transform.position;
		prevPosition2 = p[1].transform.position;


	}

	//here goes hardcoded collisions

	printf("Linear Velocity:(%f,%f,%f)\n", p[0].rigidbody.lVelocity.x, p[0].rigidbody.lVelocity.y, p[0].rigidbody.lVelocity.z);

	//pass data back
	if (network->sessions.size() > 0) {
		sendMessage(0, TRANSFORMATION_DATA, std::to_string(0) + "," + to_string(p[0].transform.position.x) + "," + to_string(p[0].transform.position.y)
			+ "," + to_string(p[0].transform.position.z) + "," + to_string(p[0].transform.rotation.x) + "," + to_string(p[0].transform.rotation.y)
			+ "," + to_string(p[0].transform.rotation.z) + "," + to_string(p[0].transform.scale.x) + "," + to_string(p[0].transform.scale.y)
			+ "," + to_string(p[0].transform.scale.z) + ",");
		sendMessage(0, TRANSFORMATION_DATA, std::to_string(1) + "," + to_string(p[1].transform.position.x) + "," + to_string(p[1].transform.position.y)
			+ "," + to_string(p[1].transform.position.z) + "," + to_string(p[1].transform.rotation.x) + "," + to_string(p[1].transform.rotation.y)
			+ "," + to_string(p[1].transform.rotation.z) + "," + to_string(p[1].transform.scale.x) + "," + to_string(p[1].transform.scale.y)
			+ "," + to_string(p[1].transform.scale.z) + ",");
	}
	if (network->sessions.size() > 1) {
		sendMessage(1, TRANSFORMATION_DATA, std::to_string(1) + "," + to_string(p[1].transform.position.x) + "," + to_string(p[1].transform.position.y)
			+ "," + to_string(p[1].transform.position.z) + "," + to_string(p[1].transform.rotation.x) + "," + to_string(p[1].transform.rotation.y)
			+ "," + to_string(p[1].transform.rotation.z) + "," + to_string(p[1].transform.scale.x) + "," + to_string(p[1].transform.scale.y)
			+ "," + to_string(p[1].transform.scale.z) + ",");
		sendMessage(1, TRANSFORMATION_DATA, std::to_string(0) + "," + to_string(p[0].transform.position.x) + "," + to_string(p[0].transform.position.y)
			+ "," + to_string(p[0].transform.position.z) + "," + to_string(p[0].transform.rotation.x) + "," + to_string(p[0].transform.rotation.y)
			+ "," + to_string(p[0].transform.rotation.z) + "," + to_string(p[0].transform.scale.x) + "," + to_string(p[0].transform.scale.y)
			+ "," + to_string(p[0].transform.scale.z) + ",");

	}




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
				case KEY_INPUT:
					parsedData = Tokenizer::tokenize(',', packet.data);
					handleIncomingKey(parsedData);
					break;

				case LOAD_COLLISIONS:
					parsedData = Tokenizer::tokenize(',', packet.data);
					handleIncomingCollider(parsedData);

					if (collisionBoxes.size() == 2) {
						p[0].collider = &collisionBoxes[0];
						p[1].collider = &collisionBoxes[1];
					}

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
		sendMessage(pairs.back()->getClient1()-1, PLAYER_NUM, "0,");
	}
	else {
		pairs.back()->setClient2(id);
		sendMessage(pairs.back()->getClient2()-1, PLAYER_NUM, "1,");
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

//checks if a player collides with anything
bool ServerGame::collisionCheck(Player _player)
{
	for (BoxCollider collision : collisionBoxes) {
		if (collision.index != _player.collider->index) {
			if (_player.collider->checkCollision(collision)) {
				return true;
			}
		}
	}
	return false;

}
//Index 0 is the player num 
//Index 1 is the key pushed
//Index 2-4 player position data
void ServerGame::handleIncomingKey(const std::vector<std::string>& data)
{

	int playerNum = std::stoi(data[0]);
	int keycode = std::stoi(data[1]);

	glm::vec3 tempPosition = p[playerNum].transform.position;

	switch (keycode)
	{
	case 'a':
		p[playerNum].rigidbody.addForce(0.3f * p[playerNum].transform.getLeft());
		
		break;
	case 's':
		p[playerNum].rigidbody.addForce(0.3f * p[playerNum].transform.getBackward());

		break;
	case 'w':
		p[playerNum].rigidbody.addForce(-0.3f * p[playerNum].transform.getBackward());

		break;
	case 'd':
		p[playerNum].rigidbody.addForce(-0.3f * p[playerNum].transform.getLeft());

		break;

	case 'q':
		p[playerNum].transform.rotation.y += 1.0f;
		break;
	case 'e':
		p[playerNum].transform.rotation.y -= 1.0f;

		break;


	case 32: // jump charge

		break;
	case 33: // jump release

		break;
	default:
		break;
	}


}

//processes the data for player transformaitons
void ServerGame::handleIncomingTransformation(const std::vector<std::string>& data)
{
	int playerNum = std::stoi(data[0]);
	p[playerNum].transform.position.x = std::stof(data[1]);
	p[playerNum].transform.position.y = std::stof(data[2]);
	p[playerNum].transform.position.z = std::stof(data[3]);
	p[playerNum].transform.rotation.x = std::stof(data[4]);
	p[playerNum].transform.rotation.y = std::stof(data[5]);
	p[playerNum].transform.rotation.z = std::stof(data[6]);
	p[playerNum].transform.scale.x = std::stof(data[7]);
	p[playerNum].transform.scale.y = std::stof(data[8]);
	p[playerNum].transform.scale.z = std::stof(data[9]);
	
	//here is the init function:
	p[playerNum].rigidbody.keepUpdating = true;
	p[playerNum].rigidbody.gravity = true;
	p[playerNum].rigidbody.gravAccel = -0.1f;
	p[playerNum].rigidbody.lDrag = 0.2f;
	p[playerNum].rigidbody.mass = 1;
	p[playerNum].rigidbody.maxVelocity = 5.0f;
	p[playerNum].rigidbody.minVelocity = 0.0f;
	p[playerNum].rigidbody.rDrag = 0.05f;

	if (playerNum == 1) {
		start = true;
	}
}

//processes the data for colliders
void ServerGame::handleIncomingCollider(const std::vector<std::string>& data)
{
	BoxCollider newCollider = BoxCollider();

	newCollider.tag = (ColliderTag)std::stoi(data[0]);
	newCollider.size.x = std::stof(data[1]);
	newCollider.size.y = std::stof(data[2]);
	newCollider.size.z = std::stof(data[3]);
	newCollider.center.x = std::stof(data[4]);
	newCollider.center.y = std::stof(data[5]);
	newCollider.center.z = std::stof(data[6]);

	newCollider.index = collisionBoxes.size();

	collisionBoxes.push_back(newCollider);

}

