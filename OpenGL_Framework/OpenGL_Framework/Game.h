#pragma once

#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <string>

#include "GL\glew.h"

#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Camera.h"
#include "Player.h"
#include "Timer.h"
#include "ClientNetwork.h"
#include "Packet.h"
#include "FormatString.h"
#include "MessageHandler.h"
#include "Tokenizer.h"
#include "glm/ext.hpp"

using glm::vec2;

#define WINDOW_WIDTH		    1280
#define WINDOW_HEIGHT			720
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

	void cameraFollow();

	

private:
	void handlePackets();

	void updatePlayers(const std::vector<std::string>&, PacketTypes _packet);

	int playerNum = 0;

	float* getData(glm::mat4);

	std::vector<PhysicalEntity * > collisionObjects;

	Camera camera;

	Mesh box;
	Mesh box2;
	Mesh map;

	Texture GrassTexture;
	Texture FlatBlueTexture;
	Texture Sky;

	ShaderProgram PassThrough;
	ShaderProgram BoundingShader;

	PhysicalEntity *mapTransform = new PhysicalEntity();
	PhysicalEntity *skyBoxTransform = new PhysicalEntity();


	Player *player1 = new Player();
	Player *player2 = new Player();

	Player *currentPlayer = new Player();

	glm::vec3 cameraVelocity = {0,0,0};
	glm::vec3 cameraSteering = {0,0,0};

	glm::vec4 boundingBoxColor = { 1.0f, 0.0f, 0.0f, 1.0f };

	float t = 0;

	void drawBoundingBox(BoxCollider boundingbox, Mesh* mesh);

	void checkCollisions();

	bool wPushed = false;
	bool sPushed = false;
	bool aPushed = false;
	bool dPushed = false;
	bool qPushed = false;
	bool ePushed = false;
};
