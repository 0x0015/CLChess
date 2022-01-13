#pragma once
#include <iostream>
#include <optional>
#include <unordered_map>
#include <tuple>
#include <vector>
struct boardstate{
	uint8_t pieceType;
	uint8_t xpos;
	uint8_t ypos;
};

class Board{
public:
	static inline std::unordered_map<std::string, int> pieceTypes = {{"p", 1}, {"n", 2}, {"b", 3}, {"r", 4}, {"q", 5}, {"k", 6}, {"P", 7}, {"N", 8}, {"B", 9}, {"R", 10}, {"Q", 11}, {"K", 12}};
	static inline std::vector<std::string> i2l = {"", "a", "b", "c", "d", "e", "f", "g", "h"};
	static inline std::unordered_map<std::string, uint8_t> l2i = {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}, {"f", 6}, {"g", 7}, {"h", 8}};
	boardstate Position[32];//increase up to 64 if it is likely that there will be more than 32 pieces
	bool bMove;
	std::tuple<bool, bool, bool, bool> castling;//qkQK
	std::optional<std::pair<uint8_t, bool>> enPassant;
	unsigned int halfmoveClock;
	unsigned int fullmoveCounter;
	Board();
	Board(std::string fen);
	void fromFen(std::string fen);
	std::string toFen();
};
