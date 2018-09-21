#pragma once
#include <string>

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

	INIT_CONNECTION = 0,

	MESSAGE = 1,

	POSITION_DATA = 2,

	INPUT_DATA = 3,

};

struct Packet {

	unsigned int packet_type;

	std::string message;

	void serialize(char * data) {
		memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(Packet));
	}
};