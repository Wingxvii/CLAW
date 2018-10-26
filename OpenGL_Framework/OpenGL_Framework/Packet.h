#pragma once
#include <string>

#define MAX_PACKET_SIZE 1000000
#define DEFAULT_DATA_SIZE 512

enum PacketTypes {

	//recieves 3 vec3 for transform
	INIT_CONNECTION = 0,

	//recieves a string
	MESSAGE = 1,

	//recieves 3 vec3 for transform
	TRANSFORMATION_DATA = 2,

	INPUT_DATA = 3,

	PLAYER_NUM = 4,

	//recieves letter
	KEY_INPUT = 5,

};

struct Packet {

	unsigned int packet_type;

	char data[DEFAULT_DATA_SIZE];

	void serialize(char * data) {
		memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(Packet));
	}
};