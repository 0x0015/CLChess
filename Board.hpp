#pragma once
#include <iostream>
#include <optional>
#include <unordered_map>
#include <tuple>
#include <vector>

uint8_t getBoardPiece(const uint8_t* board, uint8_t xpos, uint8_t ypos);
void setBoardPiece(uint8_t* board, uint8_t xpos, uint8_t ypos, uint8_t value);

class Board{
public:
	static inline std::unordered_map<std::string, int> pieceTypes = {{"p", 1}, {"n", 2}, {"b", 3}, {"r", 4}, {"q", 5}, {"k", 6}, {"P", 7}, {"N", 8}, {"B", 9}, {"R", 10}, {"Q", 11}, {"K", 12}};
	static inline std::vector<std::string> i2l = {"", "a", "b", "c", "d", "e", "f", "g", "h"};
	static inline std::unordered_map<std::string, uint8_t> l2i = {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}, {"f", 6}, {"g", 7}, {"h", 8}};
	uint8_t Position[32];//mask of uint4's(not an actual type) containing the piece value 0pnbrqkPNBRQK
	void setPiece(uint8_t xpos, uint8_t ypos, uint8_t value);
	uint8_t getPiece(uint8_t xpos, uint8_t ypos);
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
