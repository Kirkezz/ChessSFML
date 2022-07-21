#pragma once
#include <cstdint>
#include <iostream>
class ChessBoard;
enum figures {
    WHITE_PAWN = 0,
    BLACK_PAWN = 1,
    WHITE_KNIGHT = 2,
    BLACK_KNIGHT = 3,
    WHITE_BISHOP = 4,
    BLACK_BISHOP = 5,
    WHITE_ROOK = 6,
    BLACK_ROOK = 7,
    WHITE_QUEEN = 8,
    BLACK_QUEEN = 9,
    WHITE_KING = 10,
    BLACK_KING = 11,
    NOTHING = 12
};
struct Square {
    int x, y;
};
class Figure {
public:
    int xPos, yPos, lastX, lastY;
    unsigned moves = 0;
    int type;
    bool color;
    Figure(ChessBoard* board = nullptr, int type = NOTHING, int x = -1, int y = -1);
    bool canMoveTo(int toX, int toY) {
        return canMove(toX, toY, this);
    }
    bool isWhite() { return !color; }
    bool isBlack() { return color; }
    operator bool() {
        return type != NOTHING;
    }
    bool operator==(Figure f) {
        return f.xPos == xPos && f.yPos == yPos;
    }
    bool operator!=(Figure f) {
        return !(this->operator==(f));
    }
    void reset() {
        type = NOTHING;
    }
    void updatePos(int x, int y);
    void updateType(int newType);
    ChessBoard* board;
private:
    bool (*canMove) (int, int, Figure*);
};
