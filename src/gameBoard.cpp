#include <gameBoard.hpp>
// Used for random numbers
#include <algorithm>
void GameBoard::newGame() {
	int i;

    // Reset board 
	for (i = 0; i < 64; i++) {
		if (board[i] != nullptr) {
			delete board[i];
			board[i] = nullptr;
		}
	}

	// White army
	board[0] = new Rook(0, true, 4);
	board[7] = new Rook(7, true, 4);

	board[1] = new Knight(1, true, 2);
	board[6] = new Knight(6, true, 2);

	board[2] = new Bishop(2, true, 3);
	board[5] = new Bishop(5, true, 3);

	board[3] = new Queen(3, true, 5);
	board[4] = new King(4, true, 6);
	whiteKingPos = 4;

	for (i = 8; i < 16; i++) {
		board[i] = new Pawn(i, true, 1);
	}

	// Black army
	board[56] = new Rook(56, false, 4);
	board[63] = new Rook(63, false, 4);

	board[57] = new Knight(57, false, 2);
	board[62] = new Knight(62, false, 2);

	board[58] = new Bishop(58, false, 3);
	board[61] = new Bishop(61, false, 3);

	board[59] = new Queen(59, false, 5);
	board[60] = new King(60, false, 6);
	blackKingPos = 60;

	for (i = 48; i < 56; i++) {
		board[i] = new Pawn(i, false, 1);
	}

    // Set color flag
	color = true;
}

// The AI algorithm that moves a piece
void GameBoard::botMove() {
	std::pair<int, int> move;
	std::vector<std::pair<int, int>> legalMoves;
	char promote = 0;

	// Choose at random
	//move = legalMoves[rand() % legalMoves.size()];

	move = negamax(*this, 4, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()).second;
	//std::cout << "/msakjsaDSALDSALds " << move.first << move.second << '\n';

	if (move.first == -1 ) {
		resign();
		return;
	}

	if (board[move.first]->id == 1) {
		if (move.second / 8 == 7 || move.second / 8 == 0) {
			promote = 'q';
		}
	}

	board[move.first]->moveTo(move.second, promote, *this);
	color = !color;

	// Send our move to the output
	if (promote == 0) {
		std::cout << "move " << posItoS(move) << std::endl;
	} else {
		std::cout << "move " << posItoS(move) << promote << std::endl;
	}
}

// Executes the move of the human
void GameBoard::playerMove(const std::string &pos) {
	std::pair<int, int> move = moveStoI(pos);
	char promote = 0;

	if (pos.size() == 5) {
		promote = pos[4];
	}

	board[move.first]->moveTo(move.second, promote, *this);
	color = !color;
	// Then check if force is true | false
	// If it is false, call botMove(); 
	if (!force) {
		botMove();
	}
}

// Send resign message
void GameBoard::resign() const {
	std::cout << "resign\n";
}

bool GameBoard::isAttacked(const int pos, bool byColor) const {
	//vector_moves moves{&Pawn::s_resource};
	vector_moves moves;
	
	for (int i = 0; i < 64; i++) {
		if (board[i] != nullptr && board[i]->color == byColor) {
			if (board[i]->id == 1) {
				int inc = byColor ? 8 : -8;

				if (i % 8 != 0) {
					moves.push_back(i + inc - 1);
				}
				if ((i + 1) % 8 != 0) {
					moves.push_back(i + inc + 1);
				}
			} else {
				moves = board[i]->legalMoves(*this, true);
			}

			for (auto j = moves.begin(); j < moves.end(); j++) {
				if (*j == pos) {
					return true;
				}
			}
		}
	}
	return false;
}
void prB(GameBoard& gameBoard) {
		std::cout<<'\n';
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (gameBoard.board[i * 8 + j] != nullptr) {
				if (gameBoard.board[i * 8 + j]->color == WHITE) {
					switch (gameBoard.board[i * 8 + j]->id) {
						case 1:
							std::cout << 'P' << ' ';
							break;
						case 2:
							std::cout << 'N' << ' ';
							break;
						case 3:
							std::cout << 'B' << ' ';
							break;
						case 4:
							std::cout << 'R' << ' ';
							break;
						case 5:
							std::cout << 'Q' << ' ';
							break;
						case 6:
							std::cout << 'K' << ' ';
							break;
						default:
							break;   
					}
				} else {
					switch (gameBoard.board[i * 8 + j]->id) {
						case 1:
							std::cout << 'p' << ' ';
							break;
						case 2:
							std::cout << 'n' << ' ';
							break;
						case 3:
							std::cout << 'b' << ' ';
							break;
						case 4:
							std::cout << 'r' << ' ';
							break;
						case 5:
							std::cout << 'q' << ' ';
							break;
						case 6:
							std::cout << 'k' << ' ';
							break;
						default:
							break;   
					}
				}
			} else {
				std::cout << '-' << ' ';
			}
		}
		std::cout<<'\n';
	}

}
std::pair<float, std::pair<int, int>> GameBoard::negamax(GameBoard& gameBoard, int depth, float alpha, float beta) {
	std::vector<std::pair<int, int>> legalMoves;
	legalMoves.reserve(156);
	//std::cout << "Funky stuff open" << '\n';
	
	std::vector<Piece*> notNullofColor = gameBoard.board;
	bool color = gameBoard.color;
	std::sort(notNullofColor.begin(), notNullofColor.end(), [color] (Piece* a, Piece* b) {
		if (a == nullptr || a->color != color) return false;
		else if (b == nullptr || b->color != color) return true;
			else return a->id > b->id;
	});

	for (Piece* i : notNullofColor) {
		if (i != nullptr && i->color == color) {
			for (int j : i->legalMoves(gameBoard, false)) {
				legalMoves.push_back({i->pos, j});
			}
		} else {
			break;
		}
	}

	if (legalMoves.size() == 0 || depth == 0) {
		
    	return {evalPos(gameBoard), {-1, -1}};
    }

    std::pair<float, std::pair<int, int>> bestScore(-std::numeric_limits<float>::infinity(), std::pair<int, int>(0, 0));

	//std::cout << "Nr moves for depth " << depth << " is: " << legalMoves.size() << "\n"; 
    for (const auto& move : legalMoves) {
		
		// Move it!
		//std::cout << "BefMove:";
		//prB(gameBoard);
		int pos = gameBoard.board[move.first]->pos;
		bool everMoved = gameBoard.board[move.first]->everMoved;

		bool color = gameBoard.color;
		int lastPawnDoubleMove = gameBoard.lastPawnDoubleMove;
		int whiteKingPos = gameBoard.whiteKingPos, blackKingPos = gameBoard.blackKingPos;
		int nrWhiteChecks = gameBoard.nrWhiteChecks, nrBlackChecks = gameBoard.nrBlackChecks;
		Piece* oldStartPiece = gameBoard.board[move.first];
		Piece* oldDestPiece = gameBoard.board[move.first]->moveTo(move.second, 'q', gameBoard);

 
        // STEP 3.2: play for the opponent
		gameBoard.color = !gameBoard.color;
		//std::cout << "MARCO " << depth << ' ';
        std::pair<float, std::pair<int, int>> score = negamax(gameBoard, depth - 1, -beta, -alpha);
		//std::cout << "POLO" << '\n';
		
		score.first = -score.first;

		//std::cout << "AftNeg:";
		//prB(gameBoard);


        // opponent allows player to obtain this score if player will do current move.
        // player chooses this move only if it has a better score.
        if (score.first > bestScore.first) {
            bestScore.first = score.first;
			bestScore.second = move;
        }
 
        // STEP 3.3: update alpha (found a better move?)
        if (bestScore.first > alpha) {
            alpha = bestScore.first;
        }

		// UNDO
		
		gameBoard.color = color;
		gameBoard.lastPawnDoubleMove = lastPawnDoubleMove;
		gameBoard.whiteKingPos = whiteKingPos;
		gameBoard.blackKingPos = blackKingPos;
		gameBoard.nrWhiteChecks = nrWhiteChecks;
		gameBoard.nrBlackChecks = nrBlackChecks;

		if (gameBoard.board[move.second]->id == 6 && abs(move.second - pos) == 2) {
			
			if (move.second > pos) {
				gameBoard.board[move.second + 1] = gameBoard.board[move.second - 1];
				gameBoard.board[move.second - 1] = nullptr;
				gameBoard.board[move.second + 1]->pos = move.second + 1;
				gameBoard.board[move.second + 1]->everMoved = false;
			} else {
				gameBoard.board[move.second - 2] = gameBoard.board[move.second + 1];
				gameBoard.board[move.second + 1] = nullptr;
				gameBoard.board[move.second - 2]->pos = move.second - 2;
				gameBoard.board[move.second - 2]->everMoved = false;
			}
		}
		
		gameBoard.board[move.first] = oldStartPiece;
		gameBoard.board[move.first]->pos = pos;
		gameBoard.board[move.first]->everMoved = everMoved;
		gameBoard.board[move.second] = nullptr;

		if (oldDestPiece != nullptr) {
			gameBoard.board[oldDestPiece->pos] = oldDestPiece;
		} 

        // STEP 3.4: cut-off
        if (alpha >= beta) {
            break;
        }
 
        // STEP 3.4: undo move
        //undo_move(state, move);
    }

    return bestScore;
}

float GameBoard::evalPos(GameBoard& gameBoard) {
	float evaluation = 0;

	for (int i = 0; i < 64; i++) {
		if (gameBoard.board[i] != nullptr && gameBoard.board[i]->color == gameBoard.color) {
			vector_moves places = gameBoard.board[i]->legalMoves(gameBoard, false);
			evaluation += gameBoard.board[i]->value;
			evaluation +=  (float)places.size() *.2f;
			for (const auto place : places) {
				if (gameBoard.board[place] != nullptr && gameBoard.board[place]->id != 6) {
					evaluation +=  (float)gameBoard.board[place]->value *.5f;
				}
			}
		}
	}

	if (gameBoard.color == WHITE) {
		switch (gameBoard.nrBlackChecks) {
			case 1:
				evaluation += 200;
				break;
			case 2:
				evaluation += 5000;
				break;
			case 3:
				evaluation += 1000000;
				break;
			default:
				break;
		}
	} else {
		switch (gameBoard.nrWhiteChecks) {
			case 1:
				evaluation += 200;
				break;
			case 2:
				evaluation += 5000;
				break;
			case 3:
				evaluation += 1000000;
				break;
			default:
				break;
		}
	}

	return evaluation;
}

// Function that checks if a string is a move operation 
bool GameBoard::validate(const std::string &move) {
	if (move.size() < 4) {
		return false;
	}
	return
	move[0] >= 'a' && move[0] <= 'h' &&
	move[1] >= '1' && move[1] <= '8' &&
	move[2] >= 'a' && move[2] <= 'h' &&
	move[3] >= '1' && move[3] <= '8';
}

// Function that converts human move action splitting column and line
// position and converting it to int
std::pair<int, int> GameBoard::moveStoI(const std::string &move) const {
	return {
		(move[1] - '1') * 8  + move[0] - 'a',
		(move[3] - '1') * 8  + move[2] - 'a'
	};
}

// Function that converts from int position to 
// the string that is sent to xboard
std::string GameBoard::posItoS(const std::pair<int, int> &move) const {
	// To compute line: move / 8; 
	// To compute column: move % 8; 
	return {
		move.first % 8 + 'a',
		move.first / 8 + '1',
		move.second % 8 + 'a',
		move.second / 8 + '1'
	};
}