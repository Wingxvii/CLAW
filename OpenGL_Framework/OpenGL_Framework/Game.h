#pragma once

#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "MiniMath/Vector.h"
#include "Timer.h"
#include "ClientNetwork.h"
#include "Packet.h"
#include "FormatString.h"

class Game
{
public:
	Game();
	~Game();

	ClientNetwork* network;

	void sendMessage();

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

private:

};
