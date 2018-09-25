#include "ClientPair.h"



ClientPair::ClientPair()
{
}


ClientPair::~ClientPair()
{
}

bool ClientPair::checkIfClient1Assigned()
{
	if (client1 == -1) {
		return false;
	}
	return true;	
}

bool ClientPair::checkIfClient2Assigned()
{
	if (client2 == -1) {
		return false;
	}
	return true;
}

int ClientPair::getClient1()
{
	return client1;
}

int ClientPair::getClient2()
{
	return client2;
}

void ClientPair::setClient1(int first)
{
	client1 = first;
}

void ClientPair::setClient2(int second)
{
	client2 = second;
}
