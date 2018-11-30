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
	//load health shader
	if (!healthShader.Load("./Assets/Shaders/HealthShader.vert", "./Assets/Shaders/HealthShader.frag")) {
		std::cout << "Shaders failed to init.\n";
	}

	if (!coolDownShader.Load("./Assets/Shaders/CoolDownShader.vert", "./Assets/Shaders/CoolDownShader.frag")) {
		std::cout << "Shaders failed to init.\n";
	}

	//load healthbar
	if (!healthBar.loadMeshes("./Assets/Models/Health_", 2)) {
		std::cout << "Model failed to load.\n";

	}

	//load cooldown
	if (!coolDown.loadMeshes("./Assets/Models/CoolDown_", 1)) {
		std::cout << "Model failed to load.\n";

	}

	//load character 1 idle 
	if (!character1Anim.loadMeshes("./Assets/Models/devil idle_", 11)) {
		std::cout << "Model failed to load.\n";

	}
	//load character 1 walk
	if (!character1Anim.loadMeshes("./Assets/Models/devil walk cycle_", 13)) {
		std::cout << "Model failed to load.\n";
	
	}
	character1Anim.loops = true;
	//load character 2 idle 
	if (!character2Anim.loadMeshes("./Assets/Models/devil idle_", 11)) {
		std::cout << "Model failed to load.\n";
		
	}
	//load character 2 walk
	if (!character2Anim.loadMeshes("./Assets/Models/devil walk cycle_", 13)) {
		std::cout << "Model failed to load.\n";
		
	}
	character2Anim.loops = true;
	//load map mesh - for static objects load animation with only one frame
	if (!mapAnim.loadMeshes("./Assets/Models/map_lava_", 1)) {
		std::cout << "Model failed to load.\n";
		
	}
	for (int i = 0; i < 3; i++) {
		MeshAnimator temp;
		if (!temp.loadMeshes("./Assets/Models/dedTree_", 1)) {
			std::cout << "Model failed to load.\n";
		}
		trees.push_back(temp);
	}

	//load sky box mesh
	if (!skyBoxAnim.loadMeshes("./Assets/Models/skybox_", 1)) {
		std::cout << "Model failed to load.\n";
	
	}

	if (!brokenAFBridge.loadMeshes("./Assets/Models/bridge_", 1)) {

	}

	if (!wall.loadMeshes("./Assets/Models/wall_", 1)) {

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

	if (!BridgeTex.load("./Assets/Textures/bridgeTex.png"))
	{
		system("Pause");
		exit(0);
	}

	if (!WallTex.load("./Assets/Textures/wallTex.png"))
	{
		system("Pause");
		exit(0);
	}

	if (!TreeTex.load("./Assets/Textures/dedTreeTex.png"))
	{
		system("Pause");
		exit(0);
	}

	player1->setMesh(&character1Anim.animations[0][0]);
	player2->setMesh(&character2Anim.animations[0][0]);

	skyBoxTransform->setMesh(&skyBoxAnim.animations[0][0]);

	camera.perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 2000.0f);
	camera.transform->m_pLocalPosition = glm::vec3(0.0f, 1.5f, 6.0f);

	//************************************************************************//
	orthoCamera.orthographic(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.1f, 100.0f);
	orthoCamera.transform->m_pLocalPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	mapTransform->getMesh()->transform->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	skyBoxTransform->getMesh()->transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	bridgeTransform->getMesh()->transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	wallTranform->getMesh()->transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	player1->getMesh()->transform->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	player1->getMesh()->transform->setRotation(glm::vec3(0, 0, 0));
	player1->m_entityType = (int)EntityTypes::PLAYER;

	player2->getMesh()->transform->setPosition(glm::vec3(1.0f, 0.0f, -10.0f));
	player2->getMesh()->transform->setRotation(glm::vec3(0,180,0));
	player2->m_entityType = (int)EntityTypes::PLAYER;

	light1.setPosition(glm::vec3(0.0f,10.0f,0.0f));
	light2.setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	healthBarTransform.setPosition(glm::vec3(25.0f, 50.0f, 0.0f));
	coolDownTransform.setPosition(glm::vec3(1200.0f, 60.0f, 0.0f));

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

	light1.setPosition(player1->getMesh()->transform->getPosition());
	light2.setPosition(player2->getMesh()->transform->getPosition());

	//***********************************************************************************************************************************************************************************************************
	//How to make basic animations 
	character1Anim.playAnimations(deltaTime, 1);
	character2Anim.playAnimations(deltaTime, 0);


	mapAnim.playAnimations(deltaTime, 0);
	skyBoxAnim.playAnimations(deltaTime, 0);
	brokenAFBridge.playAnimations(deltaTime, 0);
	healthBar.playAnimations(deltaTime, 0);
	coolDown.playAnimations(deltaTime, 0);
	wall.playAnimations(deltaTime, 0);
	light1.update(deltaTime);
	light2.update(deltaTime);
	healthBarTransform.update(deltaTime);
	coolDownTransform.update(deltaTime);
	

	currentPlayer->getMesh()->transform->m_pRotation.y = camera.transform->m_pRotation.y;
	cameraFollow();

	//printf("Player Position: (%f,%f,%f), Camera Position: (%f,%f,%f)\n", player1->getMesh()->transform->getPosition().x, player1->getMesh()->transform->getPosition().y, player1->getMesh()->transform->getPosition().z, camera.transform->getPosition().x, camera.transform->getPosition().y, camera.transform->getPosition().z);

	camera.transform->update(deltaTime);
	//orthoCamera.transform = camera.transform;
	//orthoCamera.transform->update(deltaTime);

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
	PassThrough.SendUniform("lightPosition", glm::vec4(glm::vec3{0.0f, 5.0f, 5.0f}, 1));
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
	
	PassThrough.UnBind();
	//unbinds

	MapShader.Bind();

	MapShader.SendUniformMat4("uView", glm::value_ptr(glm::inverse(camera.transform->getLocalToWorldMatrix())), false);
	MapShader.SendUniform("viewPosition", camera.transform->m_pLocalPosition);
	MapShader.SendUniformMat4("uProj", glm::value_ptr(camera.getProjection()), false);
	MapShader.SendUniform("uTex", 0);
	
	MapShader.SendUniform("material.shininess", 2.0f);
	MapShader.SendUniform("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	MapShader.SendUniform("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	MapShader.SendUniform("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	MapShader.SendUniform("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

	glm::vec4 lightPos = glm::inverse(camera.getView()) * glm::vec4(light1.getPosition(), 1.0f);

	MapShader.SendUniform("pointLights[0].position", glm::vec3(lightPos.x, lightPos.y + 5.0f, lightPos.z));
	MapShader.SendUniform("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	MapShader.SendUniform("pointLights[0].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
	MapShader.SendUniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	MapShader.SendUniform("pointLights[0].constant", 1.0f);
	MapShader.SendUniform("pointLights[0].linear", 0.09f);
	MapShader.SendUniform("pointLights[0].quadratic", 0.032f);

	lightPos = glm::inverse(camera.getView()) * glm::vec4(light2.getPosition(), 1.0f);

	MapShader.SendUniform("pointLights[0].position", glm::vec3(lightPos.x, lightPos.y + 5.0f, lightPos.z));
	MapShader.SendUniform("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	MapShader.SendUniform("pointLights[1].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
	MapShader.SendUniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	MapShader.SendUniform("pointLights[1].constant", 1.0f);
	MapShader.SendUniform("pointLights[1].linear", 0.09f);
	MapShader.SendUniform("pointLights[1].quadratic", 0.032f);
		
	//map
	FlatBlueTexture.bind(0);
	MapShader.SendUniformMat4("uModel", glm::value_ptr(mapTransform->getMesh()->transform->getLocalToWorldMatrix()), false);
	glBindVertexArray(mapAnim.VAO);
	glDrawArrays(GL_TRIANGLES, 0, mapAnim.animations[0][0]._NumVertices);

	FlatBlueTexture.unbind(0);

	BridgeTex.bind(0);
	MapShader.SendUniformMat4("uModel", glm::value_ptr(bridgeTransform->getMesh()->transform->getLocalToWorldMatrix()), false);
	glBindVertexArray(brokenAFBridge.VAO);
	glDrawArrays(GL_TRIANGLES, 0, brokenAFBridge.animations[0][0]._NumVertices);

	BridgeTex.unbind(0);

	WallTex.bind(0);
	MapShader.SendUniformMat4("uModel", glm::value_ptr(wallTranform->getMesh()->transform->getLocalToWorldMatrix()), false);
	glBindVertexArray(wall.VAO);
	glDrawArrays(GL_TRIANGLES, 0, wall.animations[0][0]._NumVertices);

	MapShader.UnBind();

	healthShader.Bind();

	healthShader.SendUniformMat4("uView", glm::value_ptr(glm::inverse(orthoCamera.transform->getLocalToWorldMatrix())), false);
	healthShader.SendUniformMat4("uProj", glm::value_ptr(orthoCamera.getProjection()), false);
	healthShader.SendUniform("healthAmount", playerHealth);


	healthShader.SendUniformMat4("uModel", glm::value_ptr(healthBarTransform.getLocalToWorldMatrix()), false);
	glBindVertexArray(healthBar.VAO);
	glDrawArrays(GL_TRIANGLES, 0, healthBar.animations[0][0]._NumVertices);
	healthShader.UnBind();


	coolDownShader.Bind();

	coolDownShader.SendUniformMat4("uView", glm::value_ptr(glm::inverse(orthoCamera.transform->getLocalToWorldMatrix())), false);
	coolDownShader.SendUniformMat4("uProj", glm::value_ptr(orthoCamera.getProjection()), false);
	
	coolDownShader.SendUniformMat4("uModel", glm::value_ptr(coolDownTransform.getLocalToWorldMatrix()), false);
	glBindVertexArray(coolDown.VAO);
	glDrawArrays(GL_TRIANGLES, 0, coolDown.animations[0][0]._NumVertices);
	coolDownShader.UnBind();

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



