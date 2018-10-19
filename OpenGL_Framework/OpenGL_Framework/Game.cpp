#include "Game.h"

Game::Game()
{
	network = new ClientNetwork();

	MessageHandler::sendInitConnection(network);

	MessageHandler::sendMessage(network, "Hello");

}

Game::~Game()
{
	delete updateTimer;

	PassThrough.UnLoad();
	box.Unload();
	GrassTexture.Unload();
}

void Game::initializeGame()
{
	updateTimer = new Timer();
		
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);

	//load crate shaders
	if (!PassThrough.Load("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough.frag")) {
		std::cout << "Shaders failed to init.\n";
		system("pause");
		exit(0);
	}

	//load crate mesh
	if (!box.LoadfromFile("./Assets/Models/crate.obj")) {
		std::cout << "Model failed to load.\n";
		system("pause");
		exit(0);
	}

	//load texture
	if (!GrassTexture.Load("./Assets/Textures/Grass.png"))
	{
		system("Pause");
		exit(0);
	}

	player1.setMesh(&box);
	player2.setMesh(&box);

	camera.perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 1000.0f);
	camera.getTransform()->m_pLocalPosition = glm::vec3(0.0f, 1.5f, 6.0f);
	

	player1.getTransform()->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));

	player2.getTransform()->setPosition(glm::vec3(1.0f, 0.0f, -10.0f));
	player2.getTransform()->setRotationAngleY(180);

	
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	
	t = pow(0.1, 60.0f * deltaTime);
	//camera.getTransform()->setRotationAngleY(45);
	cameraFollow();
	camera.getTransform()->update(deltaTime);


	player1.getTransform()->update(deltaTime);
	
	player2.getTransform()->update(deltaTime);

	handlePackets();
		
	//...
}

float* convertToFloats(glm::mat4 matrix) {

	float* fArray = new float[16];

	const float *pSource = (const float*)glm::value_ptr(matrix);
	for (int i = 0; i < 16; ++i)
		fArray[i] = pSource[i];

	return fArray;
}

void Game::draw()
{

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//binds
	PassThrough.Bind();
	PassThrough.SendUniformMat4("uView", convertToFloats(glm::inverse(camera.getTransform()->getLocalToWorldMatrix())), false);
	PassThrough.SendUniformMat4("uProj", convertToFloats(camera.getProjection()), false);

	GrassTexture.Bind();

	//cube 1
	PassThrough.SendUniformMat4("uModel", convertToFloats(player1.getTransform()->getLocalToWorldMatrix()), false);
	glBindVertexArray(box.VAO);
	glDrawArrays(GL_TRIANGLES, 0, box.GetNumVertices());

	//cube 2
	PassThrough.SendUniformMat4("uModel", convertToFloats(player2.getTransform()->getLocalToWorldMatrix()), false);
	glDrawArrays(GL_TRIANGLES, 0, box.GetNumVertices());
	glBindVertexArray(0);

	//unbinds
	GrassTexture.UnBind();
	PassThrough.UnBind();

	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	
	switch(key)
	{
	case 'a':
		MessageHandler::sendMovementInput(network, 'a', currentPlayer.getTransform()->getPosition(),playerNum);
		break;
	case 's':
		MessageHandler::sendMovementInput(network, 's', currentPlayer.getTransform()->getPosition(), playerNum);
		break;
	case 'w':
		MessageHandler::sendMovementInput(network, 'w', currentPlayer.getTransform()->getPosition(), playerNum);
		break;
	case 'd':
		MessageHandler::sendMovementInput(network, 'd', currentPlayer.getTransform()->getPosition(), playerNum);
		break;
	case 'q':
		MessageHandler::sendRotationInput(network, 'q', currentPlayer.getTransform()->getRotationAngleY(), playerNum);
		break;
	case 'e':
		MessageHandler::sendRotationInput(network, 'e', currentPlayer.getTransform()->getRotationAngleY(), playerNum);
		break;
	default:
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:
		
			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

	}
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
}

void Game::cameraFollow()
{
	glm::vec3 playerPositionWithOffset;

	glm::vec3 offset;
	glm::vec2 offset2D = currentPlayer.getTransform()->forward*3.0f;
	offset = glm::vec3(offset2D.x, 2.0f, offset2D.y);

	playerPositionWithOffset = currentPlayer.getTransform()->getPosition() + offset;

	camera.getTransform()->setPosition(camera.getTransform()->getPosition() * (1.0f - t) + (playerPositionWithOffset) * t);
	camera.getTransform()->setRotationAngleY(currentPlayer.getTransform()->getRotationAngleY());
}

void Game::handlePackets()
{
	Packet packet;
	std::vector<std::string> parsedData;

	int data_length = network->receivePackets(network_data);

	if (data_length <= 0)// if data length is zero or less no data recieve 
	{
		return;
	}
	int i = 0;
	while (i < (unsigned int)data_length)
	{
		packet.deserialize(&(network_data[i]));
		i += sizeof(Packet);

		switch (packet.packet_type) {

		case MESSAGE:

			printf(packet.data, "\n");

			break;

		case PLAYER_NUM:

			parsedData = Tokenizer::tokenize(',', packet.data);

			playerNum = std::stoi(parsedData[0]);

			if (playerNum == 1) {
				currentPlayer = player1;
			}
			else {
				currentPlayer = player2;
				camera.getTransform()->setRotationAngleY(-180);
				
			}

			break;

		case POSITION_DATA:
			parsedData = Tokenizer::tokenize(',', packet.data);
			updatePlayers(parsedData, (PacketTypes)packet.packet_type);

			break;
		case ROTATION_DATA:
			parsedData = Tokenizer::tokenize(',', packet.data);
			updatePlayers(parsedData, (PacketTypes)packet.packet_type);
			break;
		default:

			printf("error in packet types\n");

			break;
		}
	}

}

void Game::updatePlayers(const std::vector<std::string>& data, PacketTypes _packet)
{
	if (_packet == POSITION_DATA) {
		int playerToMove = std::stoi(data[0]);
		glm::vec3 translate = { std::stof(data[1]), std::stof(data[2]), std::stof(data[3]) };

		if (playerToMove == 1) {
			player1.getTransform()->setPosition(translate);
		}
		else {
			player2.getTransform()->setPosition(translate);
		}
	}
	else {
		int playerToRotate = std::stoi(data[0]);
		float rotation = std::stof(data[1]);

		if (playerToRotate == 1) {
			player1.getTransform()->setRotationAngleY(rotation);
		}
		else {
			player2.getTransform()->setRotationAngleY(rotation);
		}

	}

}



