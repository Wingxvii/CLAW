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
		MessageHandler::sendMovementInput(network, 'a', currentPlayer.getTransform()->getLocalToWorldMatrix(),playerNum);
		break;
	case 's':
		MessageHandler::sendMovementInput(network, 's', currentPlayer.getTransform()->getLocalToWorldMatrix(), playerNum);
		break;
	case 'w':
		MessageHandler::sendMovementInput(network, 'w', currentPlayer.getTransform()->getLocalToWorldMatrix(), playerNum);
		break;
	case 'd':
		MessageHandler::sendMovementInput(network, 'd', currentPlayer.getTransform()->getLocalToWorldMatrix(), playerNum);
		break;
	case 'q':
		MessageHandler::sendMovementInput(network, 'q', currentPlayer.getTransform()->getLocalToWorldMatrix(), playerNum);
		break;
	case 'e':
		MessageHandler::sendMovementInput(network, 'e', currentPlayer.getTransform()->getLocalToWorldMatrix(), playerNum);
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

	if (playerNum == 1) {
		playerPositionWithOffset = currentPlayer.getTransform()->getPosition() - glm::vec3(-0.5f, -2.0f, -3.0f);
	}
	else {
		playerPositionWithOffset = currentPlayer.getTransform()->getPosition() - glm::vec3(0.5f, -2.0f, 3.0f);
	}


	camera.getTransform()->setPosition(camera.getTransform()->getPosition() * (1.0f - t) + (playerPositionWithOffset) * t);

	camera.getTransform()->m_pLocalToWorldMatrix = glm::lookAt(glm::vec3(0.0f, 1.0f, 3.0f), currentPlayer.getTransform()->getPosition(), glm::vec3(0., -1., 0.));


	
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
	glm::mat4 recievedPosition;
	recievedPosition[0][0] = std::stof(data[1]);
	recievedPosition[0][1] = std::stof(data[2]);
	recievedPosition[0][2] = std::stof(data[3]);
	recievedPosition[0][3] = std::stof(data[4]);
	recievedPosition[1][0] = std::stof(data[5]);
	recievedPosition[1][1] = std::stof(data[6]);
	recievedPosition[1][2] = std::stof(data[7]);
	recievedPosition[1][3] = std::stof(data[8]);
	recievedPosition[2][0] = std::stof(data[9]);
	recievedPosition[2][1] = std::stof(data[10]);
	recievedPosition[2][2] = std::stof(data[11]);
	recievedPosition[2][3] = std::stof(data[12]);
	recievedPosition[3][0] = std::stof(data[13]);
	recievedPosition[3][1] = std::stof(data[14]);
	recievedPosition[3][2] = std::stof(data[15]);
	recievedPosition[3][3] = std::stof(data[16]);

	//glm::mat4 transformationMatrix = glm::mat4(1.0);
	//transformationMatrix[0][3] = recievedPosition[0][3];
	//transformationMatrix[1][3] = recievedPosition[1][3];
	//transformationMatrix[2][3] = recievedPosition[2][3];

	recievedPosition = glm::transpose(recievedPosition);
	

	std::cout << "(" << recievedPosition[0][0] << "," << recievedPosition[0][1] << "," << recievedPosition[0][2] << "," << recievedPosition[0][3] << "\n";
	std::cout << recievedPosition[1][0] << "," << recievedPosition[1][1] << "," << recievedPosition[1][2] << "," << recievedPosition[1][3] << "\n";
	std::cout << recievedPosition[2][0] << "," << recievedPosition[2][1] << "," << recievedPosition[2][2] << "," << recievedPosition[2][3] << "\n";
	std::cout << recievedPosition[3][0] << "," << recievedPosition[3][1] << "," << recievedPosition[3][2] << "," << recievedPosition[3][3] << ")\n";

	if (playerToMove == 1) {
		
		
		player1.getTransform()->m_pLocalToWorldMatrix = recievedPosition;
		player1.getTransform()->setPosition(glm::vec3(recievedPosition[0][3], recievedPosition[1][3], recievedPosition[2][3]));

	}
	else {


		player2.getTransform()->m_pLocalToWorldMatrix = recievedPosition;
		player2.getTransform()->setPosition(glm::vec3(recievedPosition[0][3], recievedPosition[1][3], recievedPosition[2][3]));
	}
}

