#include "Board.hpp"

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

Board::Board(std::string fromFen){
	std::vector<std::string> fenComponents = strSplit(fromFen, " ");
	if(fenComponents.size() < 6){
		std::cerr<<"Error: invalid fen string: "<<fromFen<<std::endl;
		return;
	}
	int usedBoards = 0;
	std::pair<uint8_t, uint8_t> pos = std::pair<uint8_t, uint8_t>(0,0);
	for(int i=0;i<fenComponents[0].length();i++){//piece placement
		std::string character = fenComponents[0].substr(i, 1);
		if(pieceTypes.contains(character)){
			boardstate newBoard = {.pieceType = pieceTypes[character], .xpos = pos.first, .ypos = pos.second};
			Position[usedBoards] = newBoard;
			usedBoards++;
			pos.first++;
		}else if(character == "/"){
			pos.first = 0;
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

std::string Board::toFen(){

}
