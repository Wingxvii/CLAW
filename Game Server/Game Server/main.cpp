#include "ServerGame.h"
// used for multi-threading
#include <process.h>
#include <ctime>
#include <iostream>


void serverLoop(void);

ServerGame * server;
float queueTime = 0.0f;

int main()
{

	// initialize the server
	server = new ServerGame();

	// create thread with arbitrary argument for the run function
	//_beginthread(serverLoop, 0, (void*)12);
	serverLoop();
}

void serverLoop()
{
	float queueTime = 0.0f;
	clock_t deltaTime = 0.0f;
	clock_t prevTime = 0.0f;


	while (true) {

		deltaTime = std::clock() - prevTime;
		prevTime = std::clock();
		queueTime += deltaTime;
		//std::cout << " Time Elapsed: " << deltaTime << " Ms" << endl;

		if (queueTime > 33) {
			queueTime -= 33;
			server->update();
			//std::cout << " Updated" << endl;
		}

	}


}
