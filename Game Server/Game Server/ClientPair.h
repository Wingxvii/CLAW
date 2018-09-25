#pragma once
class ClientPair
{
public:
	ClientPair();
	~ClientPair();

	bool checkIfClient1Assigned();
	bool checkIfClient2Assigned();

	int getClient1();
	int getClient2();

	void setClient1(int);
	void setClient2(int); 

private:
	int client1 = -1;
	int client2 = -1;
};

