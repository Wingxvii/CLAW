#pragma once
#include <string>

#define MAX_PACKET_SIZE 1000000
#define DEFAULT_DATA_SIZE 512

enum PacketTypes {

	INIT_CONNECTION = 0,

	MESSAGE = 1,

	POSITION_DATA = 2,

	INPUT_DATA = 3,

	PLAYER_NUM = 4,

	KEY_INPUT = 5,

	ROTATION_DATA = 6,

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