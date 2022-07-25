#pragma once
#include <inttypes.h>
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
    int8_t x, y;
};
class Figure {
public:
    int8_t xPos, yPos, lastX, lastY, type;
    unsigned moves = 0;
    bool color;
    Figure(ChessBoard* board = nullptr, int8_t type = NOTHING, int8_t x = -1, int8_t y = -1);
    bool canMoveTo(int8_t toX, int8_t toY) {
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
        return f.xPos != xPos || f.yPos != yPos;
    }
    void reset() {
        type = NOTHING;
    }
    void updatePos(int8_t x, int8_t y);
    void updateType(int8_t newType);
    ChessBoard* board;
private:
    bool (*canMove) (int8_t, int8_t, Figure*);
};
