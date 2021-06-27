#include <iostream>
#include <string.h>
#include <gameBoard.hpp>
// Used for debug
#include <fstream>

int main () {
    // Fixed the output and input buffering problem 
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	
	std:: string str;
	GameBoard gameBoard;

	while (1) {
		
		std::cin >> str;

		if (str == "xboard") {
			// Established connection

		} else if (str == "protover") {
			// Sent features
			std::cout<< "feature myname=\"ChessQABM\"\n"; 
			std::cout<< "feature sigint=0 sigterm=0 san=0\n";

		} else if (str == "new") {
			// Reset board and force flag
			gameBoard.newGame();
			gameBoard.force = false;

		} else if (str == "force") {
			// Set force flag
			gameBoard.force = true;

		} else if (str  == "go") {
			// Reset force flag, called botMove() 
			gameBoard.force = false;
			gameBoard.botMove();

		} else if (str == "quit") {
			// Exit operation
			exit(0);

		} else if (str == "resign") {
			// Resign Action
			gameBoard.resign();

		} else {
			if (gameBoard.validate(str)) {
				// Validated if is move operation type
				// Called playerMove(string str); 
				gameBoard.playerMove(str);
			}
		}
	}
	return 0;
}