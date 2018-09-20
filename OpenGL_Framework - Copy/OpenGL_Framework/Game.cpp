#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
	delete updateTimer;

	//...
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
	switch(key)
	{
	case 27: // the escape key
	case 'a':

		for (int i = 0; i < sqaure1.size(); i++) {
			sqaure1[i].x -= .1;
		}
			break;
	case 's':
		for (int i = 0; i < sqaure1.size(); i++) {
			sqaure1[i].y -= .1;
		}
		break;
	case 'w':
		for (int i = 0; i < sqaure1.size(); i++) {
			sqaure1[i].y += .1;
		}
		break;
	case 'd':
		for (int i = 0; i < sqaure1.size(); i++) {
			sqaure1[i].x += .1;
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
