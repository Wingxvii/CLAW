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
#include "MeshAnimator.h"

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
	void handleMouse(int mousex, int mousey);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;

	void cameraFollow();

	const float TO_RADS = 3.141592654f / 180.0f;


private:
	void handlePackets();

	void updatePlayers(const std::vector<std::string>&, PacketTypes _packet);

	int playerNum = 0;

	std::vector<PhysicalEntity * > collisionObjects;

	Camera camera;
	Camera orthoCamera; // for UI elements

	MeshAnimator healthBar;
	MeshAnimator coolDown;

	MeshAnimator character1Anim;
	MeshAnimator character2Anim;
	MeshAnimator mapAnim;
	MeshAnimator skyBoxAnim;
	MeshAnimator brokenAFBridge;
	MeshAnimator wall;

	MeshAnimator brazier;
	MeshAnimator trees;
	MeshAnimator health;
	MeshAnimator stump;
	MeshAnimator torch;
	

	Texture GrassTexture;
	Texture FlatBlueTexture;
	Texture Sky;
	Texture DevilTexture;
	Texture BridgeTex;
	Texture WallTex;

	Texture BrazierTex;
	Texture TreeTex;
	Texture HealthTex;
	Texture StumpTex;
	Texture TorchTex;

	ShaderProgram PassThrough;
	ShaderProgram MapShader;
	ShaderProgram BoundingShader;
	ShaderProgram healthShader;
	ShaderProgram coolDownShader;

	PhysicalEntity *mapTransform = new PhysicalEntity();
	PhysicalEntity *skyBoxTransform = new PhysicalEntity();
	PhysicalEntity *bridgeTransform = new PhysicalEntity();
	PhysicalEntity *wallTranform = new PhysicalEntity();

	PhysicalEntity *brazierTransform = new PhysicalEntity();
	PhysicalEntity *treeTransform = new PhysicalEntity();
	PhysicalEntity *HealthTransfrom = new PhysicalEntity();
	PhysicalEntity *StumpTransfrom = new PhysicalEntity();
	PhysicalEntity *TorchTransfrom = new PhysicalEntity();


	Player *player1 = new Player();
	Player *player2 = new Player();

	Player *currentPlayer = new Player();

	//Lights 
	Transform light1;
	Transform light2;

	Transform healthBarTransform;
	Transform coolDownTransform;

	std::vector<PhysicalEntity* > entities;

	glm::vec3 cameraVelocity = {0,0,0};
	glm::vec3 cameraSteering = {0,0,0};
	//new camera
	glm::vec3 cameraRot = { 0.0f,0.0f,0.0f };
	float sensitivityx = 10.0f;
	float sensitivityy = 10.0f;

	glm::vec4 boundingBoxColor = { 1.0f, 0.0f, 0.0f, 1.0f };

	float t = 0;

	void drawBoundingBox(BoxCollider boundingbox, Mesh& mesh);

	bool wPushed = false;
	bool sPushed = false;
	bool aPushed = false;
	bool dPushed = false;

	bool sendBoundingInfo = true;

	bool playIdle1;
	bool playWalk1;

	float testangle = 0.0f;

	float sunAttenuation = 0.000001f;

	float playerHealth = 1.0f;
};
