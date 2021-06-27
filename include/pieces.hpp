#ifndef PIECES_H
#define PIECES_H

#include <string>
#include <vector>
#include <memory_resource>

//using vector_moves = std::pmr::vector<int>;
using vector_moves = std::vector<int>;

#define WHITE true
#define BLACK false

class GameBoard;

class Piece {
public:
	inline static std::pmr::unsynchronized_pool_resource s_resource;
    // Position from 0 to 63
	int pos;
    // Color true = WHITE, false = BLACK
	const bool color;  
    // P1 N2 B3 R4 Q5 K6 
	const int id;
	// Value of the piece
	int value;
	// Used for last pawn move for En passant and Rook-King castling
	bool everMoved = false;  

	Piece(int pos, bool color, const int id)
		:pos(pos), 
		color(color),
		id(id) {}

	// Virtual function that returns legal moves
	virtual vector_moves legalMoves(const GameBoard &gameBoard, bool noCheckKing) const = 0;

	// Used to check if a certain piece move leaves the king in check
	// Or just as well doesn't resolve check
	bool isCheckAfterMove(const GameBoard &gameBoard, const int pos) const;

	Piece* moveTo(int newPos, char promote, GameBoard &gameBoard);

	virtual ~Piece() = default;
};

class Pawn : public Piece {
public:
	Pawn(int pos, bool color, const int id)
		:Piece(pos, color, id) {
			value = 1;
			}

	vector_moves legalMoves(const GameBoard &gameBoard, bool noCheckKing) const;
};

class Knight : public Piece {
public:
	Knight(int pos, bool color, const int id)
		:Piece(pos, color, id) {
			value = 3;
			}

	vector_moves legalMoves(const GameBoard &gameBoard, bool noCheckKing) const;
};

class Bishop : public Piece {
public:
	Bishop(int pos, bool color, const int id)
		:Piece(pos, color, id) {
			value = 3;
			}

	vector_moves legalMoves(const GameBoard &gameBoard, bool noCheckKing) const;
};

class Rook : public Piece {
public:
	Rook(int pos, bool color, const int id)
		:Piece(pos, color, id) {
			value = 5;
			}

	vector_moves legalMoves(const GameBoard &gameBoard, bool noCheckKing) const;
};

class Queen : public Piece {
public:
	Queen(int pos, bool color, const int id)
		:Piece(pos, color, id) {
			value = 9;
			}

	vector_moves legalMoves(const GameBoard &gameBoard, bool noCheckKing) const;
};

class King : public Piece {
public:
	King(int pos, bool color, const int id)
		:Piece(pos, color, id) {
			value = 100;
			}

	vector_moves legalMoves(const GameBoard &gameBoard, bool noCheckKing) const;
};

#endif