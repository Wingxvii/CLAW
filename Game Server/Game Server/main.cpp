#include "ServerGame.h"
// used for multi-threading
#include <process.h>

void serverLoop(void);

ServerGame * server;

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
	while (true)
	{
		server->update();
	}
}
