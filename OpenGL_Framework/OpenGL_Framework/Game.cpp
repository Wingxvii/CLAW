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
}

void Game::initializeGame()
{
	updateTimer = new Timer();
		
	glEnable(GL_DEPTH_TEST);

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

	player1.setMesh(&box);
	player2.setMesh(&box);

	camera.perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 1000.0f);
	camera.getTransform()->m_pLocalPosition = glm::vec3(0.0f, 1.5f, 6.0f);
	

	player1.getTransform()->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	player2.getTransform()->setPosition(glm::vec3(-1.0f, 0.0f, -10.0f));

	player1.directionFacing = { 0.0f, 0.0f, -1.0f };
	player2.directionFacing = { 0.0f, 0.0f, 1.0f };

	
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	

	//camera.getTransform()->setRotationAngleY(45);
	camera.getTransform()->update(deltaTime);
	cameraFollow();

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

	PassThrough.Bind();

	PassThrough.SendUniformMat4("uView", convertToFloats(glm::inverse(camera.getTransform()->getLocalToWorldMatrix())), false);
	PassThrough.SendUniformMat4("uProj", convertToFloats(camera.getProjection()), false);

	//cube 1
	PassThrough.SendUniformMat4("uModel", convertToFloats(player1.getTransform()->getLocalToWorldMatrix()), false);
	glBindVertexArray(box.VAO);
	glDrawArrays(GL_TRIANGLES, 0, box.GetNumVertices());

	//cube 2
	PassThrough.SendUniformMat4("uModel", convertToFloats(player2.getTransform()->getLocalToWorldMatrix()), false);
	glDrawArrays(GL_TRIANGLES, 0, box.GetNumVertices());
	glBindVertexArray(0);

	PassThrough.UnBind();

	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	std::string newMessage;
	
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
	case 'q': // the 'q' key
		exit(1);
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
	float bufferDistance = 2.0f;
	glm::vec3 desired_velocity;
	glm::vec3 distance = (currentPlayer.getTransform()->getPosition() - (camera.getTransform()->getPosition()));
	float magintude = glm::sqrt(glm::pow(distance.x, 2) + glm::pow(distance.y, 2) + pow(distance.z ,2));

	if (magintude > 4.0f) {
		desired_velocity = glm::normalize(currentPlayer.getTransform()->getPosition() - camera.getTransform()->getPosition()) * .07f;
		desired_velocity.y = 0;
	
	}
	else if (magintude > 3.0f) {
		desired_velocity = { 0 ,0 ,0};

	} else {
		desired_velocity = glm::normalize(currentPlayer.getTransform()->getPosition() - camera.getTransform()->getPosition()) * -.07f;
		desired_velocity.y = 0;
		desired_velocity.x = desired_velocity.x  * -1;
	}
	//camera.getTransform()->m_pLocalToWorldMatrix = glm::lookAt(glm::vec3(0.0f, 1.0f, 3.0f), currentPlayer.getTransform()->getPosition(), glm::vec3(0., -1., 0.));
	camera.getTransform()->setPosition(camera.getTransform()->getPosition() + desired_velocity);
	
	
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
			}

			break;

		case POSITION_DATA:
			parsedData = Tokenizer::tokenize(',', packet.data);

			updatePlayers(parsedData);

			break;

		default:

			printf("error in packet types\n");

			break;
		}
	}

}

void Game::updatePlayers(const std::vector<std::string>& data)
{
	int playerToMove = std::stoi(data[0]);
	glm::vec3 translate = {std::stof(data[1]), std::stof(data[2]), std::stof(data[3])};
	
	if (playerToMove == 1) {
		player1.getTransform()->setPosition(translate);
	}
	else {
		player2.getTransform()->setPosition(translate);
	}
}

