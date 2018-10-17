#include "Game.h"

Game::Game()
{
	
	network = new ClientNetwork();

	// send init packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = INIT_CONNECTION;

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);

	sendMessage(MESSAGE, "Hello");
	
	//Tokenizer::tokenize(' ', "This is a test string ");
}

Game::~Game()
{
	delete updateTimer;

	PassThrough.UnLoad();
	box.Unload();
}

void Game::sendMessage(int packet_type, std::string message)
{
	Packet packet;

	strcpy_s(packet.data, message.c_str() + '\0');

	packet.packet_type = packet_type;

	const unsigned int packet_size = sizeof(packet);
	char packet_data[packet_size];

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
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
	camera.getTransform()->setRotationAngleX(-25.0f);

	player1.getTransform()->setPosition(glm::vec3(3.5f, -1.0f, 0.0f));
	player2.getTransform()->setPosition(glm::vec3(-3.5f, -1.0f, 0.0f));


}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

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
		break;
	case 's':
		break;
	case 'w':
		break;
	case 'd':
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
	vec2 translate = {std::stof(data[1]), std::stof(data[2])};
	
	if (playerToMove == 1) {
	}
	else {
	}
}

float* Game::getData(glm::mat4 obj)
{
	//data in column major
	float data[16] = { obj[0][0] , obj[1][0], obj[2][0], obj[3][0],
	obj[0][1] , obj[1][1], obj[2][1], obj[3][1],
	obj[0][2] , obj[1][2], obj[2][2], obj[3][2],
	obj[0][3] , obj[1][3], obj[2][3], obj[3][3]
	};
	return data;
}
