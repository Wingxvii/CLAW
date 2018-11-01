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
	box.Unload();
	GrassTexture.Unload();
}

void Game::initializeGame()
{
	updateTimer = new Timer();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);

	ShaderProgram::initDefault();

	//load crate shaders
	if (!PassThrough.Load("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough.frag")) {
		std::cout << "Shaders failed to init.\n";
	}
	//load crate shaders
	if (!BoundingShader.Load("./Assets/Shaders/BoundingBox.vert", "./Assets/Shaders/BoundingBox.frag")) {
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
	//load crate mesh
	if (!box2.LoadfromFile("./Assets/Models/crate.obj")) {
		std::cout << "Model failed to load.\n";
		system("pause");
		exit(0);
	}

	//load map mesh
	if (!skyBox.LoadfromFile("./Assets/Models/skybox.obj")) {
		std::cout << "Model failed to load.\n";
		system("pause");
		exit(0);
	}

	//load sky box mesh
	if (!map.LoadfromFile("./Assets/Models/map_lava1.obj")) {
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

	//load texture
	if (!FlatBlueTexture.Load("./Assets/Textures/LavaMapTexture.png"))
	{
		system("Pause");
		exit(0);
	}

	//load texture
	if (!Sky.Load("./Assets/Textures/skybox.png"))
	{
		system("Pause");
		exit(0);
	}

	player1->setMesh(&box);
	player2->setMesh(&box2);

	skyBoxTransform->setMesh(&skyBox);

	camera.perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 1000.0f);
	camera.transform->m_pLocalPosition = glm::vec3(0.0f, 1.5f, 6.0f);


	mapTransform->getMesh()->transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	skyBoxTransform->getMesh()->transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	player1->getMesh()->transform->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	player1->getMesh()->transform->setRotation(glm::vec3(0, 0, 0));

	player2->getMesh()->transform->setPosition(glm::vec3(1.0f, 0.0f, -10.0f));
	player2->getMesh()->transform->setRotation(glm::vec3(0,180,0));

	MessageHandler::sendInitConnection(network, player1->getMesh()->transform->m_pLocalPosition, player1->getMesh()->transform->m_pRotation, player1->getMesh()->transform->m_pScale, 1);
	MessageHandler::sendInitConnection(network, player2->getMesh()->transform->m_pLocalPosition, player2->getMesh()->transform->m_pRotation, player2->getMesh()->transform->m_pScale, 2);
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	
	t = pow(0.1, 60.0f * deltaTime);
	cameraFollow();
	camera.transform->update(deltaTime);


	player1->getMesh()->transform->update(deltaTime);
	
	player2->getMesh()->transform->update(deltaTime);

	//checkCollisions();

	handlePackets();

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

	if (qPushed) {
		MessageHandler::sendKeyInput(network, 'q', playerNum);
	}

	if (ePushed) {
		MessageHandler::sendKeyInput(network, 'e', playerNum);
	}
		
	//...
}
void Game::draw()
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	//binds
	PassThrough.Bind();
	PassThrough.SendUniformMat4("uView", glm::value_ptr(glm::inverse(camera.transform->getLocalToWorldMatrix())), false);
	PassThrough.SendUniformMat4("uProj", glm::value_ptr(camera.getProjection()), false);

	PassThrough.SendUniform("uTex", 0);
	PassThrough.SendUniform("lightPosition", glm::inverse(camera.transform->getLocalToWorldMatrix()) * glm::vec4(2.0f, -4.0f, 3.0f, 0.0f));
	PassThrough.SendUniform("lightAmbient", glm::vec3(0.5f, 0.5f, 0.5f));
	PassThrough.SendUniform("lightDiffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	PassThrough.SendUniform("lightSpecular", glm::vec3(0.9f, 0.9f, 0.9f));
	PassThrough.SendUniform("lightSpecularExponent", 5.0f);
	PassThrough.SendUniform("attenuation_Constant", 1.0f);
	PassThrough.SendUniform("attenuation_Linear", 0.0001f);
	PassThrough.SendUniform("attenuation_Quadratic", 0.00001f);

	GrassTexture.Bind();

	//cube 1
	PassThrough.SendUniformMat4("uModel", glm::value_ptr(player1->getMesh()->transform->getLocalToWorldMatrix()), false);
	
	glBindVertexArray(box.VAO);
	glDrawArrays(GL_TRIANGLES, 0, box.GetNumVertices());

	//cube 2
	PassThrough.SendUniformMat4("uModel", glm::value_ptr(player2->getMesh()->transform->getLocalToWorldMatrix()), false);
	
	glDrawArrays(GL_TRIANGLES, 0, box.GetNumVertices());
	glBindVertexArray(0);
	GrassTexture.UnBind();

	Sky.Bind();
	PassThrough.SendUniformMat4("uModel", glm::value_ptr(skyBoxTransform->getMesh()->transform->getLocalToWorldMatrix()), false);

	glBindVertexArray(skyBox.VAO);
	glDrawArrays(GL_TRIANGLES, 0, skyBox.GetNumVertices());
	glBindVertexArray(0);
	Sky.UnBind();
	
	//map
	FlatBlueTexture.Bind();
	PassThrough.SendUniformMat4("uModel", glm::value_ptr(mapTransform->getMesh()->transform->getLocalToWorldMatrix()), false);
	glBindVertexArray(map.VAO);
	glDrawArrays(GL_TRIANGLES, 0, map.GetNumVertices());	
	
	PassThrough.UnBind();
	//unbinds
	FlatBlueTexture.UnBind();
	
	//drawBoundingBox(player1->getMesh()->BoundingBox, player1->getMesh());
	//drawBoundingBox(player2->getMesh()->BoundingBox, player2->getMesh());

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
		MessageHandler::sendKeyInput(network, 's', playerNum);
		break;
	case 'w':
		wPushed = true;
		//MessageHandler::sendKeyInput(network, 'w', playerNum);
		break;
	case 'd':
		dPushed = true;
		//MessageHandler::sendKeyInput(network, 'd', playerNum);
		break;
	case 'q':
		qPushed = true;
		//MessageHandler::sendKeyInput(network, 'q', playerNum);
		break;
	case 'e':
		ePushed = true;
		MessageHandler::sendKeyInput(network, 'e', playerNum);
		break;
	case 'r':
		PassThrough.reload();
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
		wPushed = false;
		break;
	case 'd':
		dPushed = false;
		break;
	case 'q':
		qPushed = false;
		break;
	case 'e':
		ePushed = false;
		break;
	case 'r':
		PassThrough.reload();
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

	glm::vec3 offset;
	glm::vec2 offset2D = currentPlayer->getMesh()->transform->forward*3.0f;
	offset = glm::vec3(offset2D.x, 2.0f, offset2D.y);

	playerPositionWithOffset = currentPlayer->getMesh()->transform->getPosition() + offset;

	camera.transform->setPosition(camera.transform->getPosition() * (1.0f - t) + (playerPositionWithOffset) * t);
	camera.transform->setRotation(glm::vec3(0,currentPlayer->getMesh()->transform->getRotationAngleY(),0));
}

void Game::drawBoundingBox(BoxCollider boundingbox, Mesh* mesh)
{
	if (mesh->GetNumVertices() == 0)
		return;

	BoundingShader.Bind();
	BoundingShader.SendUniformMat4("uView", glm::value_ptr(glm::inverse(camera.transform->getLocalToWorldMatrix())), false);
	BoundingShader.SendUniformMat4("uProj", glm::value_ptr(camera.getProjection()), false);
	BoundingShader.SendUniform("uColor", boundingBoxColor);
	GLuint VAO = GL_NONE;
	//Vertex array object
	glGenVertexArrays(1, &VAO);
	//bind to opengl
	glBindVertexArray(VAO);


	// Cube 1x1x1, centered on origin
	GLfloat vertices[] = {
	//bottom
	  -0.5, -0.5, -0.5, 1.0,
	   0.5, -0.5, -0.5, 1.0,
	   0.5, -0.5,  0.5, 1.0,
	  -0.5, -0.5,  0.5, 1.0,

	 //top
	  -0.5,  0.5, -0.5, 1.0,
	   0.5,  0.5, -0.5, 1.0,
	   0.5,  0.5,  0.5, 1.0,
	  -0.5,  0.5, -0.5, 1.0,
	};

	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLushort elements[] = {
	  0, 1, 2, 3,
	  4, 5, 6, 7,
	  0, 4, 1, 5, 2, 6, 3, 7
	};
	GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glm::vec3 size = glm::vec3(boundingbox.m_maxBound.x - boundingbox.m_minBound.x, boundingbox.m_maxBound.y - boundingbox.m_minBound.y, boundingbox.m_maxBound.z - boundingbox.m_minBound.z);
	glm::vec3 center = glm::vec3((boundingbox.m_minBound.x + boundingbox.m_maxBound.x) / 2, (boundingbox.m_minBound.y + boundingbox.m_maxBound.y) / 2, (boundingbox.m_minBound.z + boundingbox.m_maxBound.z) / 2);
	size = size ;
	glm::mat4 transform = glm::mat4(1.0);
	transform = glm::translate(glm::mat4(1), center);
	transform = transform * glm::scale(glm::mat4(1), size);
	glm::mat4 m = mesh->transform->getLocalToWorldMatrix() * transform;

	BoundingShader.SendUniformMat4("uModel", glm::value_ptr(m), false);
	/* Apply object's transformation matrix */
	

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute
		4,                  // number of elements per vertex
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	

	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &ibo_elements);

	BoundingShader.UnBind();
}

void Game::checkCollisions()
{
	boundingBoxColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	for (int i = 0; i < collisionObjects.size(); i++) {
		
		if (currentPlayer->getMesh()->BoundingBox.m_maxBound.x < collisionObjects[i]->getMesh()->BoundingBox.m_minBound.x ||
			currentPlayer->getMesh()->BoundingBox.m_minBound.x > collisionObjects[i]->getMesh()->BoundingBox.m_maxBound.x) return;

		if (currentPlayer->getMesh()->BoundingBox.m_maxBound.y < collisionObjects[i]->getMesh()->BoundingBox.m_minBound.y ||
			currentPlayer->getMesh()->BoundingBox.m_minBound.y > collisionObjects[i]->getMesh()->BoundingBox.m_maxBound.y) return;

		if (currentPlayer->getMesh()->BoundingBox.m_maxBound.z < collisionObjects[i]->getMesh()->BoundingBox.m_minBound.z ||
			currentPlayer->getMesh()->BoundingBox.m_minBound.z > collisionObjects[i]->getMesh()->BoundingBox.m_maxBound.z) return;

		boundingBoxColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	}

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
				collisionObjects.push_back(player2);
			}
			else {
				currentPlayer = player2;
				collisionObjects.push_back(player1);
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


		if (playerToMove == 1) {
			player1->getMesh()->transform->setPosition(translate);
			player1->getMesh()->transform->setRotation(rotation);
			player1->getMesh()->transform->setScale(scale);

		}
		else {
			player2->getMesh()->transform->setPosition(translate);
			player2->getMesh()->transform->setRotation(rotation);
			player2->getMesh()->transform->setScale(scale);

		}

}



