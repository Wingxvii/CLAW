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

	//...
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

	//...
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	
	handlePackets();
		
	//...
}

std::vector<vec2> sqaure1 = { {-5.0f, -1.0f}, {-5.0f, 1.0f}, {-3.0f, 1.0f}, {-3.0f, -1.0f} };
std::vector<vec2> sqaure2 = { {3.0f, -1.0f}, {3.0f, 1.0f}, {5.0f, 1.0f}, {5.0f, -1.0f} };

void renderSquare(int s) {
	glBegin(GL_QUADS); // Start drawing a quad primitive  

	for (int i = 0; i < 4; i++) {
		if (s == 1) {
			glVertex3f(sqaure1[i].x, sqaure1[i].y, 0.0f); // The bottom left corner
		}
		else {
			glVertex3f(sqaure2[i].x, sqaure2[i].y, 0.0f); // The bottom left corner
		}

		
	}

	glEnd();
}

void Game::draw()
{

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our window to red  
	glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)  
	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations  

	glTranslatef(0.0f, 0.0f, -5.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive  

	renderSquare(1); // Render the primitive  
	renderSquare(2); // Render the primitive

	glFlush(); // Flush the OpenGL buffers to the window  
	//...

	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	std::string newMessage;

	switch(key)
	{
	case 'a':

		for (int i = 0; i < sqaure1.size(); i++) {
			newMessage = std::to_string(playerNum) + ",a,";

			sendMessage(INPUT_DATA, newMessage);
			
		}
			break;
	case 's':
		for (int i = 0; i < sqaure1.size(); i++) {
			newMessage = std::to_string(playerNum) + ",s,";

			sendMessage(INPUT_DATA, newMessage);
			
		}
		break;
	case 'w':
		for (int i = 0; i < sqaure1.size(); i++) {
			newMessage = std::to_string(playerNum) + ",w,";

			sendMessage(INPUT_DATA, newMessage);
			
		}
		break;
	case 'd':
		for (int i = 0; i < sqaure1.size(); i++) {
			newMessage = std::to_string(playerNum) + ",d,";

			sendMessage(INPUT_DATA, newMessage);
			
		}
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
		for (int i = 0; i < sqaure1.size(); i++) {
			sqaure1[i].x += translate.x;
			sqaure1[i].y += translate.y;
		}
	}
	else {
		for (int i = 0; i < sqaure2.size(); i++) {
			sqaure2[i].x += translate.x;
			sqaure2[i].y += translate.y;
		}
	}
}
