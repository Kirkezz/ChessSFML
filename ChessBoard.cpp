#include "ChessBoard.h"
#include "ChessFigure.h"
using namespace std;
ChessBoard::ChessBoard(string FENBoard, bool colorToMove) {
    initBoard(FENBoard);
    moves += colorToMove;
}
map<char, int8_t> ChessBoard::FENToEnum = {{'K', WHITE_KING}, {'k', BLACK_KING}, {'Q', WHITE_QUEEN}, {'q', BLACK_QUEEN},
                                 {'R', WHITE_ROOK}, {'r', BLACK_ROOK},{'B', WHITE_BISHOP}, {'b', BLACK_BISHOP},
                                 {'N', WHITE_KNIGHT}, {'n', BLACK_KNIGHT}, {'P', WHITE_PAWN}, {'p', BLACK_PAWN}};
void ChessBoard::initBoard(string FENBoard) {
    for(int8_t i = 0, k = 0; i<FENBoard.length(); ++i) {
        if (isdigit(FENBoard[i])) {
            k += FENBoard[i] - '0';
        }
        int8_t x = (k & 7), y = k >> 3;
        auto type = FENToEnum.find(FENBoard[i]);
        if (type == FENToEnum.end()) {
            continue;
        }
        if (type->second == WHITE_KING) {
            WKX = x; WKY = y;
        } else
        if (type->second == BLACK_KING) {
            BKX = x; BKY = y;
        }
        board[x][y] = Figure(this, type->second, x, y);
        ++k;
    }
}
bool ChessBoard::move(int8_t x, int8_t y, int8_t toX, int8_t toY) {
    inMove = 1;
    if (canMove(x, y, toX, toY)) {
        board[toX][toY] = board[x][y];
        board[x][y].reset();
        board[toX][toY].updatePos(toX, toY);
        if (board[toX][toY].type == WHITE_KING) {
            WKX = toX; WKY = toY;
        }
        else if (board[toX][toY].type == BLACK_KING) {
            BKX = toX; BKY = toY;
        }
        lastX = toX;
        lastY = toY;
        ++moves;
        movesList.push_back({ { x, y }, { toX, toY } });
        cached.clear();
        cachedMoves = 0;
        inMove = 0;
        return 1;
    }
    inMove = 0;
    return 0;
}
bool ChessBoard::canMove(int8_t x, int8_t y, int8_t toX, int8_t toY) {
    //auto t = cached.find({ x, y }); ломает рокировку, canMoveTo не вызывается при inMove
    //if (t != cached.end())
        //return t->second[toX][toY];
    if ((x == toX && y == toY) || !board[x][y] || (moves % 2 != board[x][y].color) || (board[x][y].color == board[toX][toY].color && board[toX][toY])) {
        return 0;
    }
    if (board[x][y].canMoveTo(toX, toY)) {
        Figure t = board[toX][toY], t1 = board[x][y];
        int8_t KX, KY;
        board[toX][toY] = board[x][y];
        board[x][y].reset();
        board[toX][toY].updatePos(toX, toY);
        if (board[toX][toY].type == WHITE_KING) {
            KX = WKX; KY = WKY;
            WKX = toX; WKY = toY;
        }
        else if (board[toX][toY].type == BLACK_KING) {
            KX = BKX; KY = BKY;
            BKX = toX; BKY = toY;
        }
        bool f = isKInCheck(moves % 2);
        if (board[toX][toY].type == WHITE_KING) {
            WKX = KX; WKY = KY;
        }
        else if (board[toX][toY].type == BLACK_KING) {
            BKX = KX; BKY = KY;
        }
        board[toX][toY] = t;
        board[x][y] = t1;
        return !f;
    }
    return 0;
}
bool ChessBoard::move(string from, string to) {
    Square t = ANToXY(from), t1 = ANToXY(to);
    return move(t.x, t.y, t1.x, t1.y);
}
bool ChessBoard::isUnderAttack(int8_t x, int8_t y, int8_t ignoreColor) {
    int8_t color = (!board[x][y]) ? (ignoreColor == -1 ? 42 : ignoreColor) : board[x][y].color;
    for(int8_t i = 0; i<8; ++i) {
        for(int8_t j = 0; j<8; ++j) {
            if (board[i][j] && board[i][j].color != color && board[i][j].canMoveTo(x, y)) {
                return 1;
            }
        }
    }
    return 0;
}
bool ChessBoard::isKInCheck(bool color) {
    return isUnderAttack(color ? BKX : WKX, color ? BKY : WKY, color);
}
void ChessBoard::generatePossibleMoves(int8_t x, int8_t y) {
    if (cached.find({ x, y }) != cached.end()) {
        return;
    }
    vector<vector<bool>> result(8);
    for (auto& i : result)
        i.resize(8);
    for (int8_t i = 0; i < 8; ++i)
        for (int8_t j = 0; j < 8; ++j) {
            result[i][j] = canMove(x, y, i, j);
            if (result[i][j]) {
                ++cachedMoves;
            }
        }
    cached.insert({ make_pair(x, y), result });
}