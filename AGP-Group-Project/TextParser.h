#pragma once
#ifndef B00289996_TEXT_PARSER_H
#define B00289996_TEXT_PARSER_H
#include <string>
class TextParser {
public:
	~TextParser() {}
	static std::string ReadFile(const std::string & location);
};

#endif // !B00289996_TEXT_PARSER_H
