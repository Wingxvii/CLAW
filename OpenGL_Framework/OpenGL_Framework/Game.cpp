#include "Game.h"

Game::Game()
{
	network = new ClientNetwork();

	MessageHandler::sendMessage(network, "A player has connected.");

}

Game::~Game()
{
	delete updateTimer;

	PassThrough.UnLoad();
	
	GrassTexture.unload();
}

void Game::initializeGame()
{
	updateTimer = new Timer();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);

	ShaderProgram::initDefault();

	//load player shaders
	if (!PassThrough.Load("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough.frag")) {
		std::cout << "Shaders failed to init.\n";
	}
	//load map shaders
	if (!MapShader.Load("./Assets/Shaders/Map.vert", "./Assets/Shaders/Map.frag")) {
		std::cout << "Shaders failed to init.\n";
	}
	//load bounding box
	if (!BoundingShader.Load("./Assets/Shaders/BoundingBox.vert", "./Assets/Shaders/BoundingBox.frag")) {
		std::cout << "Shaders failed to init.\n";
		
	}

	//load character 1 idle 
	if (!character1Anim.loadMeshes("./Assets/Models/Devil_Idle", 4)) {
		std::cout << "Model failed to load.\n";

	}
	//load character 1 walk
	if (!character1Anim.loadMeshes("./Assets/Models/DevilWalk", 8)) {
		std::cout << "Model failed to load.\n";
	
	}
	character1Anim.loops = true;
	//load character 2 idle 
	if (!character2Anim.loadMeshes("./Assets/Models/Devil_Idle", 4)) {
		std::cout << "Model failed to load.\n";
		
	}
	//load character 2 walk
	if (!character2Anim.loadMeshes("./Assets/Models/DevilWalk", 8)) {
		std::cout << "Model failed to load.\n";
		
	}
	character2Anim.loops = true;
	//load map mesh - for static objects load animation with only one frame
	if (!mapAnim.loadMeshes("./Assets/Models/map_lava", 1)) {
		std::cout << "Model failed to load.\n";
		
	}

	//load sky box mesh
	if (!skyBoxAnim.loadMeshes("./Assets/Models/skybox", 1)) {
		std::cout << "Model failed to load.\n";
	
	}

	//load texture
	if (!GrassTexture.load("./Assets/Textures/Grass.png"))
	{
		system("Pause");
		exit(0);
	}

	//load texture
	if (!FlatBlueTexture.load("./Assets/Textures/LavaMapTexture.png"))
	{
		system("Pause");
		exit(0);
	}

	if (!DevilTexture.load("./Assets/Textures/Devil.png"))
	{
		system("Pause");
		exit(0);
	}

	//load texture
	if (!Sky.load("./Assets/Textures/skybox.png"))
	{
		system("Pause");
		exit(0);
	}

	player1->setMesh(&character1Anim.animations[0][0]);
	player2->setMesh(&character2Anim.animations[0][0]);

	skyBoxTransform->setMesh(&skyBoxAnim.animations[0][0]);

	camera.perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 1000.0f);
	camera.transform->m_pLocalPosition = glm::vec3(0.0f, 1.5f, 6.0f);


	mapTransform->getMesh()->transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	skyBoxTransform->getMesh()->transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	player1->getMesh()->transform->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	player1->getMesh()->transform->setRotation(glm::vec3(0, 0, 0));
	player1->m_entityType = (int)EntityTypes::PLAYER;

	player2->getMesh()->transform->setPosition(glm::vec3(1.0f, 0.0f, -10.0f));
	player2->getMesh()->transform->setRotation(glm::vec3(0,180,0));
	player2->m_entityType = (int)EntityTypes::PLAYER;

	sunPosition.setPosition(glm::vec3(0.0f,10.0f,0.0f));
	

	MessageHandler::sendInitConnection(network, player1->getMesh()->transform->m_pLocalPosition, player1->getMesh()->transform->m_pRotation, player1->getMesh()->transform->m_pScale, 0);
	MessageHandler::sendInitConnection(network, player2->getMesh()->transform->m_pLocalPosition, player2->getMesh()->transform->m_pRotation, player2->getMesh()->transform->m_pScale, 1);

	entities.push_back(player1);
	entities.push_back(player2);
	
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	
	t = pow(0.1, 60.0f * deltaTime);

	character1Anim.playAnimations(deltaTime, 1);
	character2Anim.playAnimations(deltaTime, 0);
	mapAnim.playAnimations(deltaTime, 0);
	skyBoxAnim.playAnimations(deltaTime, 0);
	sunPosition.update(deltaTime);

	
	currentPlayer->getMesh()->transform->m_pRotation.y = camera.transform->m_pRotation.y;
	cameraFollow();

	//printf("Player Position: (%f,%f,%f), Camera Position: (%f,%f,%f)\n", player1->getMesh()->transform->getPosition().x, player1->getMesh()->transform->getPosition().y, player1->getMesh()->transform->getPosition().z, camera.transform->getPosition().x, camera.transform->getPosition().y, camera.transform->getPosition().z);

	camera.transform->update(deltaTime);

	if (playWalk1) {
		
	}
	else {
		
	}
	//player1->setMesh(&character1Anim.interpolatedMesh);
	//player2->setMesh(&character2Anim.interpolatedMesh);

	player1->getMesh()->transform->update(deltaTime);
	player2->getMesh()->transform->update(deltaTime);

	//checkCollisions();

	handlePackets();

	MessageHandler::sendRotationinfo(network, playerNum, currentPlayer->getMesh()->transform->m_pRotation);

	if (wPushed) {
		MessageHandler::sendKeyInput(network, 'w', playerNum);
	} 

	if (aPushed) {
		MessageHandler::sendKeyInput(network, 'a', playerNum);
	}

	if (sPushed) {
		MessageHandler::sendKeyInput(network, 's', playerNum);
	}

	if (dPushed) {
		MessageHandler::sendKeyInput(network, 'd', playerNum);
	}


	//make sure this happens last in the update 
	if (playerNum == 0 && sendBoundingInfo) {
		MessageHandler::sendBoundingBoxInfo(network, entities);
		sendBoundingInfo = false;
	}
	
		
	//...
}
void Game::draw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//binds
	PassThrough.Bind();
	PassThrough.SendUniformMat4("uView", glm::value_ptr(glm::inverse(camera.transform->getLocalToWorldMatrix())), false);
	PassThrough.SendUniformMat4("uProj", glm::value_ptr(camera.getProjection()), false);
	

	PassThrough.SendUniform("uTex", 0);
	PassThrough.SendUniform("lightPosition", glm::vec4(sunPosition.getPosition(), 1));
	PassThrough.SendUniform("lightAmbient", glm::vec3(0.2f, 0.2f, 0.2f));
	PassThrough.SendUniform("lightDiffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	PassThrough.SendUniform("lightSpecular", glm::vec3(0.9f, 0.9f, 0.9f));
	

	DevilTexture.bind(0);

	//cube 1
	PassThrough.SendUniformMat4("uModel", glm::value_ptr(player1->getMesh()->transform->getLocalToWorldMatrix()), false);
	PassThrough.SendUniform("uInterpParam", character1Anim.interpParam);
	glBindVertexArray(character1Anim.VAO);
	glDrawArrays(GL_TRIANGLES, 0, character1Anim.animations[0][0]._NumVertices);
	glBindVertexArray(0);

	//cube 2
	PassThrough.SendUniformMat4("uModel", glm::value_ptr(player2->getMesh()->transform->getLocalToWorldMatrix()), false);
	
	glBindVertexArray(character2Anim.VAO);
	PassThrough.SendUniform("uInterpParam", character2Anim.interpParam);
	glDrawArrays(GL_TRIANGLES, 0, character2Anim.animations[0][0]._NumVertices);
	glBindVertexArray(0);
	DevilTexture.unbind(0);

	Sky.bind(0);
	PassThrough.SendUniformMat4("uModel", glm::value_ptr(skyBoxTransform->getMesh()->transform->getLocalToWorldMatrix()), false);

	glBindVertexArray(skyBoxAnim.VAO);
	PassThrough.SendUniform("uInterpParam", skyBoxAnim.interpParam);
	glDrawArrays(GL_TRIANGLES, 0, skyBoxAnim.animations[0][0]._NumVertices);
	glBindVertexArray(0);
	Sky.unbind(0);
	
	//unbinds

	//MapShader.Bind();

	//MapShader.SendUniformMat4("uView", glm::value_ptr(glm::inverse(camera.transform->getLocalToWorldMatrix())), false);
	//MapShader.SendUniformMat4("uProj", glm::value_ptr(camera.getProjection()), false);
	//MapShader.SendUniform("uTex", 0);
	//glm::vec4 lightPos = glm::inverse(camera.getView()) * glm::vec4(player1->getMesh()->transform->m_pLocalPosition, 1.0f);
	//MapShader.SendUniform("lightPosition", lightPos);
	//glm::vec4 lightPos2 = glm::inverse(camera.getView()) * glm::vec4(player2->getMesh()->transform->m_pLocalPosition, 1.0f);
	//MapShader.SendUniform("lightPosition2", lightPos);
	//MapShader.SendUniform("lightAmbient", glm::vec3(0.2f, 0.2f, 0.2f));
	//MapShader.SendUniform("lightDiffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	//MapShader.SendUniform("lightSpecular", glm::vec3(0.9f, 0.9f, 0.9f));

	// Ask for the handles identfying the uniform variables in our shader.
;
	
	//map
	FlatBlueTexture.bind(0);
	MapShader.SendUniformMat4("uModel", glm::value_ptr(mapTransform->getMesh()->transform->getLocalToWorldMatrix()), false);
	glBindVertexArray(mapAnim.VAO);
	MapShader.SendUniform("uInterpParam", mapAnim.interpParam);
	glDrawArrays(GL_TRIANGLES, 0, mapAnim.animations[0][0]._NumVertices);

	FlatBlueTexture.unbind(0);
	

	PassThrough.UnBind();
	//MapShader.UnBind();
	//drawBoundingBox(player1->getMesh()->BoundingBox, *player1->getMesh());
	//drawBoundingBox(player2->getMesh()->BoundingBox, *player2->getMesh());




	glutSwapBuffers();


}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	//try using bools 
	switch(key)
	{
	case 'a':
		aPushed = true;
		//MessageHandler::sendKeyInput(network, 'a', playerNum);
		break;
	case 's':
		sPushed = true;
		//MessageHandler::sendKeyInput(network, 's', playerNum);
		break;
	case 'w':
		wPushed = true;
		//MessageHandler::sendKeyInput(network, 'w', playerNum);
		break;
	case 'd':
		dPushed = true;
		//MessageHandler::sendKeyInput(network, 'd', playerNum);
		break;
	case 32:
		MessageHandler::sendKeyInput(network, 32, playerNum);
		break;
	case 'r':
		PassThrough.reload();
		break;
	case 'm':
		testangle++;
		break;
			case 'o':
		sunAttenuation += 0.1f;
		break;
	case 'l':
		sunAttenuation -= 0.1f;
		if (sunAttenuation < 0) {
			sunAttenuation = 0;
		}
		break;
	default:
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	//try using bools 
	switch (key)
	{
	case 'a':
		aPushed = false;
		break;
	case 's':
		sPushed = false;
		break;
	case 'w':
		playWalk1 = false;
		wPushed = false;
		break;
	case 'd':
		dPushed = false;
		break;
	case 'r':
		PassThrough.reload();
		break;
	case 32:
		MessageHandler::sendKeyInput(network, 33, playerNum);
		break;
	case 27:
		exit(0);
		break;
	default:
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

			glm::vec3 lookDir;
			lookDir.x = glm::inverse(camera.getView())[0][2];
			lookDir.y = glm::inverse(camera.getView())[1][2];
			lookDir.z = glm::inverse(camera.getView())[2][2];

			printf("(%f,%f,%f)", lookDir.x,lookDir.y, lookDir.z);

			MessageHandler::sendAttackinfo(network, playerNum, 1, lookDir, 0);


			break;
		case GLUT_RIGHT_BUTTON:

			MessageHandler::sendAttackinfo(network, playerNum, 2, glm::vec3{0,0,0}, 0);


			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else if (state == GLUT_UP) {
		switch (button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:
			MessageHandler::sendAttackinfo(network, playerNum, 2, glm::vec3{ 0,0,0 }, 1);

			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
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

	float cameraDistance = 8.0f - glm::abs(glm::sin(cameraRot.x * TO_RADS))*7.0f;
	float cameraY = 4.0f - (glm::sin(cameraRot.x* TO_RADS) *8.0f);

	//printf("%f\n", glm::sin(cameraRot.x* TO_RADS));
	glm::vec2 offset2D = currentPlayer->getMesh()->transform->forward*cameraDistance;


	offset = glm::vec3(offset2D.x, cameraY, offset2D.y);

	playerPositionWithOffset = currentPlayer->getMesh()->transform->getPosition() + offset;

	camera.transform->setPosition(playerPositionWithOffset);
	camera.transform->setRotation(cameraRot);
}

void Game::handleMouse(int mousex, int mousey)
{
	int xMouse = mousex - (WINDOW_WIDTH / 2);
	int yMouse = mousey - (WINDOW_HEIGHT / 2);

	//this is correct. dont ask me, idk
	cameraRot.y -= xMouse / sensitivityx;
	cameraRot.x -= yMouse / sensitivityy;
	
	if (cameraRot.x < -90.0f)
	{
		cameraRot.x = -90.0f;
	}
	if (cameraRot.x > 90.0f)
	{
		cameraRot.x = 90.0f;
	}

	if (cameraRot.y < -180.0f)
	{
		cameraRot.y += 360.0f;
	}

	if (cameraRot.y > 180.0f)
	{
		cameraRot.y -= 360.0f;
	}
	glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

}

void Game::drawBoundingBox(BoxCollider boundingbox, Mesh& mesh)
{
	//if (mesh._NumVertices == 0)
	//	return;

	//BoundingShader.Bind();
	//BoundingShader.SendUniformMat4("uView", glm::value_ptr(glm::inverse(camera.transform->getLocalToWorldMatrix())), false);
	//BoundingShader.SendUniformMat4("uProj", glm::value_ptr(camera.getProjection()), false);
	//BoundingShader.SendUniform("uColor", boundingBoxColor);
	//GLuint VAO = GL_NONE;
	////Vertex array object
	//glGenVertexArrays(1, &VAO);
	////bind to opengl
	//glBindVertexArray(VAO);


	//// Cube 1x1x1, centered on origin
	//GLfloat vertices[] = {
	////bottom
	//  -0.5, -0.5, -0.5, 1.0,
	//   0.5, -0.5, -0.5, 1.0,
	//   0.5, -0.5,  0.5, 1.0,
	//  -0.5, -0.5,  0.5, 1.0,

	// //top
	//  -0.5,  0.5, -0.5, 1.0,
	//   0.5,  0.5, -0.5, 1.0,
	//   0.5,  0.5,  0.5, 1.0,
	//  -0.5,  0.5, -0.5, 1.0,
	//};

	//GLuint vbo_vertices;
	//glGenBuffers(1, &vbo_vertices);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//GLushort elements[] = {
	//  0, 1, 2, 3,
	//  4, 5, 6, 7,
	//  0, 4, 1, 5, 2, 6, 3, 7
	//};
	//GLuint ibo_elements;
	//glGenBuffers(1, &ibo_elements);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glm::vec3 size = boundingbox.m_size;
	//glm::vec3 center = boundingbox.m_center;
	//
	//glm::mat4 transform = glm::mat4(1.0);
	//transform = glm::translate(glm::mat4(1), center);
	//transform = transform * glm::scale(glm::mat4(1), size);
	//glm::mat4 m = mesh.transform->getLocalToWorldMatrix() * transform;

	//BoundingShader.SendUniformMat4("uModel", glm::value_ptr(m), false);
	///* Apply object's transformation matrix */
	//

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(
	//	0,                  // attribute
	//	4,                  // number of elements per vertex
	//	GL_FLOAT,           // the type of each element
	//	GL_FALSE,           // take our values as-is
	//	0,                  // no extra data between each position
	//	0                   // offset of first element
	//);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	//glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	//glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	//glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glDisableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	//

	//glDeleteBuffers(1, &vbo_vertices);
	//glDeleteBuffers(1, &ibo_elements);

	//BoundingShader.UnBind();
	
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

			if (playerNum == 0) {
				currentPlayer = player1;
				collisionObjects.push_back(player1);
				collisionObjects.push_back(player2);
			}
			else {
				currentPlayer = player2;
			}

			break;

		case TRANSFORMATION_DATA:
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
		int playerToMove = std::stoi(data[0]);
		glm::vec3 translate = { std::stof(data[1]), std::stof(data[2]), std::stof(data[3]) };
		glm::vec3 rotation = { std::stof(data[4]), std::stof(data[5]), std::stof(data[6]) };
		glm::vec3 scale = { std::stof(data[7]), std::stof(data[8]), std::stof(data[9]) };


		if (playerToMove == 0) {
			player1->getMesh()->transform->setPosition(translate);
			player1->getMesh()->transform->setScale(scale);
			if (playerNum == 1) {
				player1->getMesh()->transform->setRotation(rotation);
			}
		}
		else {
			player2->getMesh()->transform->setPosition(translate);
			player2->getMesh()->transform->setScale(scale);
			if (playerNum == 0) {
				player2->getMesh()->transform->setRotation(rotation);
			}

		}

}



