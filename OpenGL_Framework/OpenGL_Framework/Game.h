#pragma once

#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <string>

#include "GL\glew.h"

#include "ShaderProgram.h"
#include "Mesh.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


#include "Camera.h"
#include "Timer.h"
#include "ClientNetwork.h"
#include "Packet.h"
#include "FormatString.h"
#include "Tokenizer.h"

using glm::vec2;

#define WINDOW_WIDTH			800
#define WINDOW_HEIGHT			600
#define FRAMES_PER_SECOND		60

class Game
{
public:
	Game();
	~Game();

	ClientNetwork* network;

	void sendMessage(int, std::string);

	char network_data[MAX_PACKET_SIZE];

	void initializeGame();
	void update();
	void draw();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;

	//object
	Mesh Crate;
	glm::mat4 CrateTransform;

	//shader
	ShaderProgram PassThrough;

	//camera
	Camera camera;


private:
	void handlePackets();

	void updatePlayers(const std::vector<std::string>&);

	int playerNum = 0;

	float getData(glm::mat4);
};
