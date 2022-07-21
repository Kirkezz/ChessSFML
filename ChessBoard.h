#pragma once
#include <string>
#include <vector>
#include "ChessFigure.h"
#include <map>
#include <list>
class Figure;
class ChessBoard {
public:
    ChessBoard(std::string FENBoard = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", bool colorToMove = 0);
    Figure& operator()(int x, int y) { return board[x][y]; }
    bool move(int x, int y, int toX, int toY);
    bool move(std::string from, std::string to);
    bool canMove(int x, int y, int toX, int toY);
    bool isKInCheck(bool color);
    bool isUnderAttack(int x, int y, int ignoreColor = -1);
    void generatePossibleMoves(int x, int y);
    unsigned moves = 0, cachedMoves = 0;
    int WKX, WKY, BKX, BKY, lastX, lastY;
    bool inMove = 0;
    std::map<std::pair<int, int>, std::vector<std::vector<bool>>> cached;
    std::list<std::pair<Square, Square>> movesList;
    Square ANToXY(std::string an) { return { int(an[0] - 'a'), int(8 - an[1] + '0') }; }
    std::string XYToAN(Square c) { return std::string(1, c.x + 'a') + std::string(1, 8 - c.y + '0'); }
private:
    Figure board[8][8];
    void initBoard(std::string FENBoard);
    static std::map<char, int> FENToEnum;
};
