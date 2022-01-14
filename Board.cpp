#include "Board.hpp"

uint8_t get_4bits(const unsigned char *arr, size_t index) {
    return arr[index >> 1] >> ((index & 1) << 2);
}

void set_4bits(unsigned char *arr, size_t index, int value) {
    arr[index >> 1] &= ~ 0x0F << ((index & 1) << 2);
    arr[index >> 1] |= (value & 0x0F) << ((index & 1) << 2);
}

uint8_t getBoardPiece(const uint8_t* board, uint8_t xpos, uint8_t ypos){
	return(get_4bits(board, (xpos*ypos)-1));
}

void setBoardPiece(uint8_t* board, uint8_t xpos, uint8_t ypos, uint8_t value){
	set_4bits(board, (xpos*ypos)-1, value);
}

uint8_t Board::getPiece(uint8_t xpos, uint8_t ypos){
	return(getBoardPiece(Position, xpos, ypos));
}

void Board::setPiece(uint8_t xpos, uint8_t ypos, uint8_t value){
	setBoardPiece(Position, xpos, ypos, value);
}

Board::Board(){

}

std::vector<std::string> strSplit(std::string& base, std::string spl){
	std::vector<std::string> output;
	int i=0;
	while(int fpos = base.find(spl) != std::string::npos){
		std::string splitstr = base.substr(i, fpos);
		output.push_back(splitstr);
		i=fpos+spl.length();
	}
	output.push_back(base.substr(i, base.length()-i));
	return(output);
}

void Board::fromFen(std::string fromFen){
	std::vector<std::string> fenComponents = strSplit(fromFen, " ");
	if(fenComponents.size() < 6){
		std::cerr<<"Error: invalid fen string: "<<fromFen<<std::endl;
		return;
	}
	for(int i=0;i<4;i++){
		((uint64_t*)Position)[i] = 0;//sets all bytes of position to 0(divides 32 bytes into 4 uint64_t)
	}
	//Position = {0};
	int usedBoards = 0;
	std::pair<uint8_t, uint8_t> pos = std::pair<uint8_t, uint8_t>(1,1);
	for(int i=0;i<fenComponents[0].length();i++){//piece placement
		std::string character = fenComponents[0].substr(i, 1);
		if(pieceTypes.contains(character)){
			setPiece(pos.first, pos.second, pieceTypes[character]);
			pos.first++;
		}else if(character == "/"){
			pos.first = 1;
			pos.second++;
		}else if(std::isdigit(character[0])){
			pos.first+=std::stoi(character);
		}
	}
	if(fenComponents[1] == "w"){
		bMove = false;
	}else if(fenComponents[1] == "b"){
		bMove = true;
	}//side to move

	for(int i=0;i<fenComponents[2].length();i++){
		std::string character = fenComponents[2].substr(i, 1);
		if(character == "q"){
			std::get<0>(castling) = true;
		}else if(character == "k"){
			std::get<1>(castling) = true;
		}else if(character == "Q"){
			std::get<2>(castling) = true;
		}else if(character == "K"){
			std::get<3>(castling) = true;
		}//castling
	}

	if(fenComponents[3].length() == 1){
		//must be "-"
		enPassant = std::nullopt;
	}else if(fenComponents[3].length() == 2){
		std::string char1 = fenComponents[3].substr(0, 1);
		std::string char2 = fenComponents[3].substr(1, 1);
		std::pair<uint8_t, uint8_t> pval;
		pval.first = l2i[char1];
		pval.second = std::stoi(char2);
		enPassant = pval;
	}//en passant

	halfmoveClock = std::stoi(fenComponents[4]);//halfmove clock
	fullmoveCounter = std::stoi(fenComponents[5]);//fullmove counter
}

Board::Board(std::string fen){
	fromFen(fen);
}

std::string Board::toFen(){

}
