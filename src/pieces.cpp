#include <pieces.hpp>
#include <gameBoard.hpp>
#include <memory_resource>

bool Piece::isCheckAfterMove(const GameBoard& gameBoard,
    const int destination) const {

    auto& board = const_cast<std::vector<Piece*>&>(gameBoard.board);
    int tempWhite = gameBoard.whiteKingPos, tempBlack = gameBoard.blackKingPos;

    // En Passant simulation
    auto diff = abs(destination - pos);
    Piece* specialAux;
    // if (board[destination] == nullptr) {
    //     std::cout << "How dafuq????S?d.sa?.dsa" << '\n';
    // }
    if (board[destination] == nullptr && id == 1 && diff != 8 && diff != 16) {
        specialAux = board[gameBoard.lastPawnDoubleMove];
        board[gameBoard.lastPawnDoubleMove] = nullptr;
    }

    Piece* aux = board[destination];
    board[destination] = board[pos];
    board[pos] = nullptr;

    if (id == 6) {
        if (color) {
            tempWhite = destination;
        }
        else {
            tempBlack = destination;
        }
    }

    if (color) {
        if (gameBoard.isAttacked(tempWhite, !color)) {
            board[pos] = board[destination];
            board[destination] = aux;
            if (board[destination] == nullptr && id == 1 && diff != 8 && diff != 16) {
                board[gameBoard.lastPawnDoubleMove] = specialAux;
            }
            return true;
        }
    }
    else {
        if (gameBoard.isAttacked(tempBlack, !color)) {
            board[pos] = board[destination];
            board[destination] = aux;
            if (board[destination] == nullptr && id == 1 && diff != 8 && diff != 16) {
                board[gameBoard.lastPawnDoubleMove] = specialAux;
            }
            return true;
        }
    }

    board[pos] = board[destination];
    board[destination] = aux;

    
    // En Passant reconstruct
    if (board[destination] == nullptr && id == 1 && diff != 8 && diff != 16) {
        board[gameBoard.lastPawnDoubleMove] = specialAux;
    }

    return false;
}

Piece* Piece::moveTo(int newPos, char promote, GameBoard& gameBoard) {
    auto& board = gameBoard.board;
    auto diff = newPos - pos;
    Piece* ret = nullptr;
    // If pos is ocupied kill the ocupant

    
    if (board[newPos] == nullptr && id == 1 && abs(diff) != 8 && abs(diff) != 16) {
        ret = board[gameBoard.lastPawnDoubleMove];   
        board[gameBoard.lastPawnDoubleMove] = nullptr; 
    } else {
        ret = board[newPos];
        board[newPos] = nullptr;
    }
        
    // Move the piece
    board[newPos] = board[pos];
    board[pos] = nullptr;
    pos = newPos;
    everMoved = true;
    gameBoard.lastPawnDoubleMove = -1;

    // King position update and Castling
    if (id == 6) {
        if (color) {
            gameBoard.whiteKingPos = newPos;
        }
        else {
            gameBoard.blackKingPos = newPos;
        }

        // O-O
        if (diff == 2) {
            board[newPos - 1] = board[newPos + 1];
            board[newPos + 1] = nullptr;
            board[newPos - 1]->everMoved = true;
            board[newPos - 1]->pos = newPos - 1;
        }

        // O-O-O
        if (diff == -2) {
            board[newPos + 1] = board[newPos - 2];
            board[newPos - 2] = nullptr;
            board[newPos + 1]->everMoved = true;
            board[newPos + 1]->pos = newPos + 1;
        }

    }

    // Case pawn move (promotion)
    if (id == 1) {
        if (abs(diff) == 16) {
            gameBoard.lastPawnDoubleMove = newPos;
        }

        if (color) {
            if (newPos / 8 == 7) {
                Piece* aux = board[newPos];

                switch (promote) {
                    case 'n':
                        board[newPos] = new Knight(newPos, color, 2);
                        break;
                    case 'b':
                        board[newPos] = new Bishop(newPos, color, 3);
                        break;
                    case 'r':
                        board[newPos] = new Rook(newPos, color, 4);
                        break;
                    case 'q':
                        board[newPos] = new Queen(newPos, color, 5);
                        break;
                    default:
                        break;   
                }

                board[newPos]->everMoved = true;
                //delete aux;
            }
        }
        else {
            if (newPos / 8 == 0) {
                Piece* aux = board[newPos];

                switch (promote) {
                    case 'n':
                        board[newPos] = new Knight(newPos, color, 2);
                        break;
                    case 'b':
                        board[newPos] = new Bishop(newPos, color, 3);
                        break;
                    case 'r':
                        board[newPos] = new Rook(newPos, color, 4);
                        break;
                    case 'q':
                        board[newPos] = new Queen(newPos, color, 5);
                        break;
                    default:
                        break;   
                }
                board[newPos]->everMoved = true;
                //delete aux;
            }
        }
    }

    if (gameBoard.isAttacked(gameBoard.whiteKingPos, BLACK)) {
        gameBoard.nrWhiteChecks++;
    }
    if (gameBoard.isAttacked(gameBoard.blackKingPos, WHITE)) {
        gameBoard.nrBlackChecks++;
    }

    return ret;
}

vector_moves Pawn::legalMoves(const GameBoard& gameBoard,
    bool noCheckKing) const
{
    auto& board = gameBoard.board;

    //vector_moves moves{&s_resource};
    vector_moves moves;
    moves.reserve(5);
    int inc = color ? 8 : -8;

    // Normal move
    if (board[pos + inc] == nullptr) {
        if (noCheckKing || !isCheckAfterMove(gameBoard, pos + inc)) {
            moves.push_back(pos + inc);
        }

        if (!everMoved && board[pos + 2 * inc] == nullptr) {
            if (noCheckKing || !isCheckAfterMove(gameBoard, pos + 2 * inc)) {
                moves.push_back(pos + 2 * inc);
            }
        }
    }

    // Normal caputre
    if (board[pos + inc - 1] && pos % 8 != 0 && board[pos + inc - 1]->color == !color) {
        if (noCheckKing || !isCheckAfterMove(gameBoard, pos + inc - 1)) {
            moves.push_back(pos + inc - 1);
        }
    }
    if (board[pos + inc + 1] && (pos + 1) % 8 != 0 && board[pos + inc + 1]->color == !color) {
        if (noCheckKing || !isCheckAfterMove(gameBoard, pos + inc + 1)) {
            moves.push_back(pos + inc + 1);
        }
    }

    // En Passant
    if (gameBoard.lastPawnDoubleMove == (pos - 1) && pos % 8 != 0 && board[gameBoard.lastPawnDoubleMove]->color == !color) {
        if (noCheckKing || !isCheckAfterMove(gameBoard, pos + inc - 1)) {
            moves.push_back(pos + inc - 1);
        }
    }
    if (gameBoard.lastPawnDoubleMove == (pos + 1) && (pos + 1) % 8 != 0 && board[gameBoard.lastPawnDoubleMove]->color == !color) {
        if (noCheckKing || !isCheckAfterMove(gameBoard, pos + inc + 1)) {
            moves.push_back(pos + inc + 1);
        }
    }

    return moves;
}

vector_moves Knight::legalMoves(const GameBoard& gameBoard,
    bool noCheckKing) const
{
    auto& board = gameBoard.board;

    //vector_moves moves{&s_resource};
    vector_moves moves;
    moves.reserve(6);
    int i = pos / 8, j = pos % 8, ia, ja, k;
    int x[] = { 1, 1, -1, -1, 2, 2, -2, -2 };
    int y[] = { 2, -2, 2, -2, 1, -1, 1, -1 };

    for (k = 0; k < 8; k++) {
        ia = i + x[k];
        ja = j + y[k];

        if (ia >= 0 && ia < 8 && ja >= 0 && ja < 8) {
            ia = ia * 8 + ja;
            if (board[ia] == nullptr || board[ia]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia)) {
                    moves.push_back(ia);
                }
            }
        }
    }

    return moves;
}

vector_moves Bishop::legalMoves(const GameBoard& gameBoard,
    bool noCheckKing) const
{
    auto& board = gameBoard.board;

    //vector_moves moves{&s_resource};
    vector_moves moves;
    moves.reserve(16);
    int i = pos / 8, j = pos % 8, ia, ja, k;

    for (k = 1; k < 8; k++) {
        ia = i + k;
        ja = j + k;
        if (ia >= 0 && ia < 8 && ja >= 0 && ja < 8) {
            if (board[ia * 8 + ja] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
            }
            else if (board[ia * 8 + ja]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = i - k;
        ja = j - k;
        if (ia >= 0 && ia < 8 && ja >= 0 && ja < 8) {
            if (board[ia * 8 + ja] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
            }
            else if (board[ia * 8 + ja]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = i + k;
        ja = j - k;
        if (ia >= 0 && ia < 8 && ja >= 0 && ja < 8) {
            if (board[ia * 8 + ja] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
            }
            else if (board[ia * 8 + ja]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = i - k;
        ja = j + k;
        if (ia >= 0 && ia < 8 && ja >= 0 && ja < 8) {
            if (board[ia * 8 + ja] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
            }
            else if (board[ia * 8 + ja]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
                break;
            }
            else {
                break;
            }
        }
    }

    return moves;
}

vector_moves Rook::legalMoves(const GameBoard& gameBoard,
    bool noCheckKing) const
{
    auto& board = gameBoard.board;

    //vector_moves moves{&s_resource};
    vector_moves moves;
    moves.reserve(16);
    int i = pos / 8, j = pos % 8, ia, k;

    for (k = 1; k < 8; k++) {
        ia = i + k;
        if (ia >= 0 && ia < 8) {
            if (board[ia * 8 + j] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + j)) {
                    moves.push_back(ia * 8 + j);
                }
            }
            else if (board[ia * 8 + j]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + j)) {
                    moves.push_back(ia * 8 + j);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = i - k;
        if (ia >= 0 && ia < 8) {
            if (board[ia * 8 + j] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + j)) {
                    moves.push_back(ia * 8 + j);
                }
            }
            else if (board[ia * 8 + j]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + j)) {
                    moves.push_back(ia * 8 + j);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = j + k;
        if (ia >= 0 && ia < 8) {
            if (board[i * 8 + ia] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, i * 8 + ia)) {
                    moves.push_back(i * 8 + ia);
                }
            }
            else if (board[i * 8 + ia]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, i * 8 + ia)) {
                    moves.push_back(i * 8 + ia);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = j - k;
        if (ia >= 0 && ia < 8) {
            if (board[i * 8 + ia] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, i * 8 + ia)) {
                    moves.push_back(i * 8 + ia);
                }
            }
            else if (board[i * 8 + ia]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, i * 8 + ia)) {
                    moves.push_back(i * 8 + ia);
                }
                break;
            }
            else {
                break;
            }
        }
    }

    return moves;
}

vector_moves Queen::legalMoves(const GameBoard& gameBoard,
    bool noCheckKing) const
{
    auto& board = gameBoard.board;

    //vector_moves moves{&s_resource};
    vector_moves moves;
    moves.reserve(32);
    int i = pos / 8, j = pos % 8, ia, ja, k;

    // Bishop
    for (k = 1; k < 8; k++) {
        ia = i + k;
        ja = j + k;
        if (ia >= 0 && ia < 8 && ja >= 0 && ja < 8) {
            if (board[ia * 8 + ja] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
            }
            else if (board[ia * 8 + ja]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = i - k;
        ja = j - k;
        if (ia >= 0 && ia < 8 && ja >= 0 && ja < 8) {
            if (board[ia * 8 + ja] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
            }
            else if (board[ia * 8 + ja]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = i + k;
        ja = j - k;
        if (ia >= 0 && ia < 8 && ja >= 0 && ja < 8) {
            if (board[ia * 8 + ja] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
            }
            else if (board[ia * 8 + ja]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = i - k;
        ja = j + k;
        if (ia >= 0 && ia < 8 && ja >= 0 && ja < 8) {
            if (board[ia * 8 + ja] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
            }
            else if (board[ia * 8 + ja]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + ja)) {
                    moves.push_back(ia * 8 + ja);
                }
                break;
            }
            else {
                break;
            }
        }
    }

    // Rook
    for (k = 1; k < 8; k++) {
        ia = i + k;
        if (ia >= 0 && ia < 8) {
            if (board[ia * 8 + j] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + j)) {
                    moves.push_back(ia * 8 + j);
                }
            }
            else if (board[ia * 8 + j]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + j)) {
                    moves.push_back(ia * 8 + j);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = i - k;
        if (ia >= 0 && ia < 8) {
            if (board[ia * 8 + j] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + j)) {
                    moves.push_back(ia * 8 + j);
                }
            }
            else if (board[ia * 8 + j]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia * 8 + j)) {
                    moves.push_back(ia * 8 + j);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = j + k;
        if (ia >= 0 && ia < 8) {
            if (board[i * 8 + ia] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, i * 8 + ia)) {
                    moves.push_back(i * 8 + ia);
                }
            }
            else if (board[i * 8 + ia]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, i * 8 + ia)) {
                    moves.push_back(i * 8 + ia);
                }
                break;
            }
            else {
                break;
            }
        }
    }
    for (k = 1; k < 8; k++) {
        ia = j - k;
        if (ia >= 0 && ia < 8) {
            if (board[i * 8 + ia] == nullptr) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, i * 8 + ia)) {
                    moves.push_back(i * 8 + ia);
                }
            }
            else if (board[i * 8 + ia]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, i * 8 + ia)) {
                    moves.push_back(i * 8 + ia);
                }
                break;
            }
            else {
                break;
            }
        }
    }

    return moves;
}

vector_moves King::legalMoves(const GameBoard& gameBoard,
    bool noCheckKing) const
{
    auto& board = gameBoard.board;

    //vector_moves moves{&s_resource};
    vector_moves moves;
    moves.reserve(8);
    int i = pos / 8, j = pos % 8, ia, ja, k;
    int x[] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    int y[] = { 0, 0, 1, -1, 1, -1, 1, -1 };

    for (k = 0; k < 8; k++) {
        ia = i + x[k];
        ja = j + y[k];

        if (ia >= 0 && ia < 8 && ja >= 0 && ja < 8) {
            ia = ia * 8 + ja;
            if (board[ia] == nullptr || board[ia]->color != color) {
                if (noCheckKing || !isCheckAfterMove(gameBoard, ia)) {
                    moves.push_back(ia);
                }
            }
        }
    }

    // Castling
    if (!everMoved && !noCheckKing) {
        // O-O
        if (board[pos + 3] != nullptr && !board[pos + 3]->everMoved && board[pos + 1] == nullptr &&
            board[pos + 2] == nullptr &&
            !gameBoard.isAttacked(pos, !color) &&
            !gameBoard.isAttacked(pos + 1, !color) &&
            !gameBoard.isAttacked(pos + 2, !color)
            ) {

                moves.push_back(pos + 2);
        }

        // O-O-O
        if (board[pos - 4] != nullptr && !board[pos - 4]->everMoved && board[pos - 1] == nullptr &&
            board[pos - 2] == nullptr && board[pos - 3] == nullptr &&
            !gameBoard.isAttacked(pos, !color) &&
            !gameBoard.isAttacked(pos - 1, !color) &&
            !gameBoard.isAttacked(pos - 2, !color)
            ) {
                moves.push_back(pos - 2);
        }
    }

    return moves;
}