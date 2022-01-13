#include "UCI.hpp"

bool strEqL(std::string& base, std::string sub, int pos = 0){
	return(base.substr(pos, sub.length()) == sub);
}

void UCI::parse(std::string in){
	if(in == "uci"){
		std::cout<<"id name CLC"<<std::endl;
		std::cout<<"id author 0x15"<<std::endl;
		std::cout<<"uciok"<<std::endl;
	}else if(strEqL(in, "debug")){
		if(in.find("on") != std::string::npos){
			debug = true;
		}else if(in.find("off") != std::string::npos){
			debug = false;
		}
		//if it's neither idk
	}else if(in == "isready"){
		std::cout<<"readyok"<<std::endl;
	}else if(strEqL(in, "setoption")){

	}else if(in == "ucinewgame"){
		
	}else if(strEqL(in, "position")){

	}else if(strEqL(in, "go")){
		if(in.find("searchmoves")){

		}else if(in.find("ponder")){

		}else if(in.find("wtime")){

		}else if(in.find("btime")){

		}else if(in.find("winc")){

		}else if(in.find("binc")){

		}else if(in.find("movestogo")){
			
		}else if(in.find("depth")){

		}else if(in.find("nodes")){

		}else if(in.find("mate")){

		}else if(in.find("movetime")){

		}else if(in.find("infinite")){

		}
	}else if(in == "stop"){

	}else if(in == "ponderhit"){

	}else if(in == "quit"){
		exit(0);
	}else{
		//not a known uci command :/
	}
}
