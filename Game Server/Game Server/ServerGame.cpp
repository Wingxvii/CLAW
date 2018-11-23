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

	updateTimer = new Timer();

	// set up the server network to listen 
	network = new ServerNetwork();
}

//fixed update
void ServerGame::update()
{

	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

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

		//checks to see if jumping
		handleJump(0);
		handleJump(1);


		//update physics
		//move cube transform
		p[0].rigidbody.update();
		p[1].rigidbody.update();

		//move cube collider & checks collisions 
		if (p[0].collider && p[1].collider) {

			p[0].collider->center += p[0].transform.position - prevPosition1;
			p[1].collider->center += p[1].transform.position - prevPosition2;

			if (collisionCheck(p[0])) {
				//revert movement if collide		
				p[0].collider->center -= p[0].transform.position - prevPosition1;
				p[0].transform.position = prevPosition1;
			}
			if (collisionCheck(p[1])) {
				//revert movement if collide 
				p[1].collider->center -= p[1].transform.position - prevPosition2;
				p[1].transform.position = prevPosition2;
			}

			handleAttackBox(0);
			handleAttackBox(1);
		}


		//update last frame position
		prevPosition1 = p[0].transform.position;
		prevPosition2 = p[1].transform.position;


	}

	//here goes hardcoded collisions
	if (p[0].transform.position.y < 1) {
		p[0].transform.position.y = 1;

		if (p[0].state == PlayerState::JUMP) {
			p[0].state = PlayerState::IDLE;
			p[0].rigidbody.inAir = false;
			p[0].jumpPower = 30;

		}
			
	}
	if (p[1].transform.position.y < 1) {
		p[1].transform.position.y = 1;
		if (p[1].state == PlayerState::JUMP) {
			p[1].state = PlayerState::IDLE;
			p[1].rigidbody.inAir = false;
			p[1].jumpPower = 30;

		}
	}


	//printf("Linear Velocity:(%f,%f,%f)\n", p[0].rigidbody.lVelocity.x, p[0].rigidbody.lVelocity.y, p[0].rigidbody.lVelocity.z);

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
				parsedData = Tokenizer::tokenize(',', packet.data);

				switch (packet.packet_type) {

				case INIT_CONNECTION:

					handleIncomingTransformation(parsedData);

					break;

				case MESSAGE:
						printf(packet.data, "\n");
					
						//sendMessage(iter->first, MESSAGE, "Hello Back");
			
					break;
				case KEY_INPUT:
					handleIncomingKey(parsedData);
					break;

				case LOAD_COLLISIONS:
					handleIncomingCollider(parsedData);

					if (collisionBoxes.size() == 2) {
						p[0].collider = &collisionBoxes[0];
						p[1].collider = &collisionBoxes[1];
					}

					break;
				case ROTATION_DATA:
					handleIncomingRotation(parsedData);
					break;

				case ATTACK:
					handleAttack(parsedData);
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
		p[playerNum].transform.rotation.y += 8.0f;
		break;
	case 'e':
		p[playerNum].transform.rotation.y -= 8.0f;

		break;


	case 32: // jump charge
		if (p[playerNum].jumpPower < 100) {
			p[playerNum].jumpPower++;
		}
		break;
	case 33: // jump release
		p[playerNum].state = PlayerState::JUMP;
		p[playerNum].rigidbody.inAir = true;
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
	p[playerNum].rigidbody.gravAccel = 0.2f;
	p[playerNum].rigidbody.lDrag = 0.35f;
	p[playerNum].rigidbody.mass = 1;
	p[playerNum].rigidbody.maxVelocity = 999.0f;
	p[playerNum].rigidbody.minVelocity = 0.0f;
	p[playerNum].rigidbody.rDrag = 0.05f;
	p[playerNum].rigidbody.aDrag = 0.05f;

	//restart calls
	p[playerNum].startData = data;
	p[playerNum].state = PlayerState::IDLE;
	p[playerNum].jumpPower = 30;
	p[playerNum].rigidbody.lAccel = glm::vec3(0.0f, 0.0f, 0.0f);
	p[playerNum].rigidbody.inAir = false;

	if (p[playerNum].collider) {
		p[playerNum].collider->center = p[playerNum].transform.position + glm::vec3(0.0f, 2.0f, 0.0f);
	}

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


	if (newCollider.index == 0 || newCollider.index == 1) {
		newCollider.size.x = 2.0f;
		newCollider.size.y = 2.0f;
		newCollider.size.z = 2.0f;
	}




	collisionBoxes.push_back(newCollider);

}

void ServerGame::handleIncomingRotation(const std::vector<std::string>& data)
{
	int playerNum = std::stoi(data[0]);
	p[playerNum].transform.rotation.x = std::stof(data[1]);
	p[playerNum].transform.rotation.y = std::stof(data[2]);
	p[playerNum].transform.rotation.z = std::stof(data[3]);

	printf("Recieved");

}

void ServerGame::handleAttack(const std::vector<std::string>& data)
{
	int playerNum = std::stoi(data[0]);
	glm::vec3 attackDirection = { glm::sin(std::stof(data[1])), glm::sin(std::stof(data[2])), glm::sin(std::stof(data[3])) };
	int charge = std::stoi(data[5]);


	switch (std::stoi(data[1])) {
	case 1:
		if (p[playerNum].rigidbody.inAir) {
			p[playerNum].rigidbody.addForce(8.0f, glm::abs(attackDirection));
		}
		else {
			attackDirection.y = 0.0f;
			p[playerNum].rigidbody.addForce(8.0f, glm::abs(attackDirection));
		}
		printf("Swung\n");

		break;
	}
	

}

void ServerGame::handleJump( int player)
{
	//check if jumping
	if (p[player].state == PlayerState::JUMP && p[player].jumpPower >= 0) {
		p[player].rigidbody.addForce(-1.0f, p[player].transform.getBackward());
		p[player].rigidbody.addForce(0.8f, glm::vec3(0, 1, 0));
		p[player].jumpPower -= 8;
	}
}

void ServerGame::handleAttackBox(int player)
{
	p[player].hitBox.center = p[player].collider->center + (p[player].transform.getBackward() * 2.0f);

	if (p[player].state == PlayerState::JUMP) {
		if (player == 0) {
			if (p[0].hitBox.checkCollision(*p[1].collider)) {
				handleIncomingTransformation(p[0].startData);
				handleIncomingTransformation(p[1].startData);
				printf("collided");

			}
		}
		else {
			if (p[1].hitBox.checkCollision(*p[0].collider)) {
				handleIncomingTransformation(p[0].startData);
				handleIncomingTransformation(p[1].startData);
				printf("collided");
			}
		}
	}
}
