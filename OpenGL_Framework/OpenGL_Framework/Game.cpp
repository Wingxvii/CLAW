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
	Crate.Unload();
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
	if (!Crate.LoadfromFile("./Assets/Models/Crate.obj")) {
		std::cout << "Model failed to load.\n";
		system("pause");
		exit(0);
	}

	//create camera
	float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
	camera.perspective(60.0f, aspectRatio, 1.0f, 1000.0f);
	camera.m_pLocalPosition = glm::vec3(0.0f, 0.0f, 5.0f);


}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	camera.update(deltaTime);


	handlePackets();
		
	//...
}

//std::vector<vec2> sqaure1 = { {-5.0f, -1.0f}, {-5.0f, 1.0f}, {-3.0f, 1.0f}, {-3.0f, -1.0f} };
//std::vector<vec2> sqaure2 = { {3.0f, -1.0f}, {3.0f, 1.0f}, {5.0f, 1.0f}, {5.0f, -1.0f} };

void Game::draw()
{

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our window to red  
	glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)  

	PassThrough.Bind();

	//data in column major
	float crateData[16] = { CrateTransform[0][0] , CrateTransform[1][0], CrateTransform[2][0], CrateTransform[3][0], 
	CrateTransform[0][1] , CrateTransform[1][1], CrateTransform[2][1], CrateTransform[3][1],
	CrateTransform[0][2] , CrateTransform[1][2], CrateTransform[2][2], CrateTransform[3][2],
	CrateTransform[0][3] , CrateTransform[1][3], CrateTransform[2][3], CrateTransform[3][3]
	};



	PassThrough.SendUniformMat4("uModel", crateData, true);
	PassThrough.SendUniformMat4("uView", crateData, true);

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
