#include "Move.hpp"

Move::Move(){

}

Move::Move(std::string fromStr){
	startPos.first = l2i[fromStr.substr(0, 1)];
	startPos.second = std::stoi(fromStr.substr(1, 1));
	endPos.first = l2i[fromStr.substr(2, 1)];
	endPos.second = std::stoi(fromStr.substr(3, 1));
	if(fromStr.length() > 4){
		std::string pr = fromStr.substr(4, 1);
		if(pr == "n"){
			promoteKnight = true;
		}else{
			promoteKnight = false;
		}
	}else{
		promoteKnight = std::nullopt;
	}
}

std::string Move::toLan(){
	std::string output;
	output += i2l[startPos.first];
	output += std::to_string(startPos.second);
	output += i2l[endPos.first];
	output += std::to_string(endPos.second);
	if(promoteKnight){
		if(promoteKnight.value()){
			output += "n";
		}else{
			output += "q";
		}
	}
	return(output);
}
