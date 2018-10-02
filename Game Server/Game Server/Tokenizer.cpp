#include "Tokenizer.h"

std::vector<std::string> Tokenizer::tokenize(char token, std::string text)
{
	std::vector<std::string> temp;
	int lastTokenLocation = 0;

	for (int i = 0; i < text.size(); i++) {
		if (text[i] == token) {
			temp.push_back(text.substr(lastTokenLocation, i - lastTokenLocation));
			lastTokenLocation = i + 1;

		}
	}
	return temp;
}