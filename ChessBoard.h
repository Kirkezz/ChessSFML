#pragma once
#include "ChessFigure.h"
#include <inttypes.h>
#include <string>
#include <vector>
#include <list>
#include <map>
class Figure;
class ChessBoard {
public:
    ChessBoard(std::string FENBoard = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", bool colorToMove = 0);
    Figure& operator()(int8_t x, int8_t y) { return board[x][y]; }
    bool move(int8_t x, int8_t y, int8_t toX, int8_t toY);
    bool move(std::string from, std::string to);
    bool canMove(int8_t x, int8_t y, int8_t toX, int8_t toY);
    bool isKInCheck(bool color);
    bool isUnderAttack(int8_t x, int8_t y, int8_t ignoreColor = -1);
    void generatePossibleMoves(int8_t x, int8_t y);
    unsigned moves = 0, cachedMoves = 0;
    int8_t WKX, WKY, BKX, BKY, lastX, lastY;
    bool inMove = 0;
    std::map<std::pair<int8_t, int8_t>, std::vector<std::vector<bool>>> cached;
    std::list<std::pair<Square, Square>> movesList;
    Square ANToXY(std::string an) { return { int8_t(an[0] - 'a'), int8_t(8 - an[1] + '0') }; }
    std::string XYToAN(Square c) { return std::string(1, c.x + 'a') + std::string(1, 8 - c.y + '0'); }
private:
    Figure board[8][8];
    void initBoard(std::string FENBoard);
    static std::map<char, int8_t> FENToEnum;
};
