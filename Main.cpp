#include "Main.hpp"
#include "board.h"

int Main::main(int argc, char** argv){

    Board board;
    parse_fen((char*)start_position, &board);
    print_board(&board);

	return(0);
}