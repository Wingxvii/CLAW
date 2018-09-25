#pragma once
#include <string>
struct FormatString
{
public:
	static std::string format(int messagetype, std::string message) {

		return messagetype + " " + message;
	}
};

