#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include <unordered_map>

class Move{
public:
	static inline std::vector<std::string> i2l = {"", "a", "b", "c", "d", "e", "f", "g", "h"};
	static inline std::unordered_map<std::string, uint8_t> l2i = {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}, {"f", 6}, {"g", 7}, {"h", 8}};
	std::pair<uint8_t, uint8_t> startPos;
	std::pair<uint8_t, uint8_t> endPos;
	std::optional<bool> promoteKnight;//otherwise it premots the queen
	Move();
	Move(std::string fromStr);
	std::string toLan();
};
