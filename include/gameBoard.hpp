#include <pieces.hpp>
#include <limits>
#include <cstdlib> 
#include <iostream>

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class GameBoard {
public:

	// Chess board
	std::vector<Piece*> board;
	// Force == true -> no bot response
	// Force == false -> bot move response
	bool force = false;
	// Color == true -> White piece
	// Color == false -> Black piece
	bool color = true;
	// Used to check En passant
	int lastPawnDoubleMove = -1;
    // Used to always check if either king is in check
	int whiteKingPos, blackKingPos;
	// 3 Check Chess
	int nrWhiteChecks = 0, nrBlackChecks = 0;

	// Constructor
	GameBoard(): board(64, nullptr) {}

	GameBoard(const GameBoard& original):
		force(original.force),
		color(original.color),
		lastPawnDoubleMove(original.lastPawnDoubleMove),
		whiteKingPos(original.whiteKingPos),
		blackKingPos(original.blackKingPos),
		nrWhiteChecks(original.nrWhiteChecks),
		nrBlackChecks(original.nrBlackChecks) {
			board.resize(64, nullptr);
			
			for (int i = 0; i < 64; i++) {
				if (original.board[i] != nullptr) {
					switch (original.board[i]->id) {
						case 1:
							board[i] = new Pawn(i, original.board[i]->color, 1);
							break;
						case 2:
							board[i] = new Knight(i, original.board[i]->color, 2);
							break;
						case 3:
							board[i] = new Bishop(i, original.board[i]->color, 3);
							break;
						case 4:
							board[i] = new Rook(i, original.board[i]->color, 4);
							break;
						case 5:
							board[i] = new Queen(i, original.board[i]->color, 5);
							break;
						case 6:
							board[i] = new King(i, original.board[i]->color, 6);
							break;
						default:
							break;   
                	}
					board[i]->everMoved = original.board[i]->everMoved;
				}
			}
		}

	// Board reset operation 
	void newGame();
     
	// Send our move to the output
	void botMove();

	// PlayerMove(const std::string &pos) calls
	// BotMove() only if force != True  
	void playerMove(const std::string &pos);
    
	// Send resign message
	void resign() const;

	// Check if given squre is attacked by any piece of given color
	bool isAttacked(const int pos, bool byColor) const;

	// Minimax
	static std::pair<float, std::pair<int, int>> negamax(GameBoard& gameBoard, int depth, float alpha, float beta);

	// Evaluate current position
	static float evalPos(GameBoard& gameBoard);

	// Used to validate if the string is a valid move
	bool validate(const std::string &move);

	// Translate String move to Int move
	std::pair<int, int> moveStoI(const std::string &move) const;

	// Translate Int move to String move
	std::string posItoS(const std::pair<int, int> &move) const;
};

#endif