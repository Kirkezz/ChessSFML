#include "ChessFigure.h"
#include "ChessBoard.h"
bool p_cm(int x, int y, Figure* f) {
    ChessBoard& board = *(f->board);
    int dir = f->isWhite() ? -1 : 1;
    if (x - f->xPos == 0 && !board(x, y)) {
        if ((y - f->yPos == 2 * dir && f->moves == 0 && !board(x, y - dir)) || (y - f->yPos == dir)) {
            return 1;
        }
    } else if (abs(x - f->xPos) == 1 && y - f->yPos == dir) {
        if (board(x, y)) {
            return 1;
        } else if (f->yPos == (f->isWhite() ? 3 : 4) && board(x, y - dir) && board.lastX == x && board.lastY == y - dir && board(x, y - dir).moves == 1) {
            if (board.inMove) {
                board(x, y - dir).reset();
            }
            return 1;
        }
    }
    return 0;
}
const Square moves[8] = { {-1, 2}, {-1, -2}, {1, 2}, {1, -2}, {-2, 1}, {-2, -1}, {2, 1}, {2, -1} };
bool n_cm(int x, int y, Figure* f) {
    x -= f->xPos; y -= f->yPos;
    for (int i = 0; i < 8; ++i) {
        if (x == moves[i].x && y == moves[i].y)
            return 1;
    }
    return 0;
}
bool b_cm(int x, int y, Figure* f) {
    ChessBoard& board = *(f->board);
    if (abs(x - f->xPos) == abs(y - f->yPos)) {
        if (f->xPos > x && f->yPos > y)
            for (int i = 1; i < 8; ++i)
                if (board(f->xPos - i, f->yPos - i) || f->xPos - i == 0 || f->yPos - i == 0)
                    return f->xPos - i <= x && f->yPos - i <= y;
        if (f->xPos > x && f->yPos < y)
            for (int i = 1; i < 8; ++i)
                if (board(f->xPos - i, f->yPos + i) || f->xPos - i == 0 || f->yPos + i == 7)
                    return f->xPos - i <= x && f->yPos + i >= y;
        if (f->xPos < x && f->yPos > y)
            for (int i = 1; i < 8; ++i)
                if (board(f->xPos + i, f->yPos - i) || f->xPos + i == 7 || f->yPos - i == 0)
                    return f->xPos + i >= x && f->yPos - i <= y;
        if (f->xPos < x && f->yPos < y)
            for (int i = 1; i < 8; ++i)
                if (board(f->xPos + i, f->yPos + i) || f->xPos + i == 7 || f->yPos + i == 7)
                    return f->xPos + i >= x && f->yPos + i >= y;
    }
    return 0;
}
bool r_cm(int x, int y, Figure* f) {
    ChessBoard& board = *(f->board);
    if (x == f->xPos && y < f->yPos)
        for (int i = f->yPos - 1; i > -1; --i)
            if (board(f->xPos, i) || i == 0)
                return y >= i;
    if (x == f->xPos && y > f->yPos)
        for (int i = f->yPos + 1; i < 8; ++i)
            if (board(f->xPos, i) || i == 7)
                return y <= i;
    if (x < f->xPos && y == f->yPos)
        for (int i = f->xPos - 1; i > -1; --i)
            if (board(i, f->yPos) || i == 0)
                return x >= i;
    if (x > f->xPos && y == f->yPos)
        for (int i = f->xPos + 1; i < 8; ++i)
            if (board(i, f->yPos) || i == 7)
                return x <= i;
    return 0;
}
bool q_cm(int x, int y, Figure* f) {
    return r_cm(x, y, f) || b_cm(x, y, f);
}
bool k_cm(int x, int y, Figure* f) {
    ChessBoard& board = *(f->board);
    if (abs(x - f->xPos) < 2 && abs(y - f->yPos) < 2) {
        return 1;
    }
    int xDir = (x < f->xPos) ? -1 : 1, rookX = xDir == -1 ? 0 : 7;
    if (abs(x - f->xPos) == 2 && f->yPos == y
            && f->moves == 0 && board(rookX, y).moves == 0
            && !board(x, y) && !board(f->xPos + xDir, y)
            && !board.isUnderAttack(f->xPos + xDir, y, f->color) && !board.isKInCheck(f->color)) {
        if (!board((xDir == -1) ? 1 : 6, y)) {
            if (board.inMove) {
                board(f->xPos + xDir, y) = board(rookX, y);
                board(rookX, y).reset();
                board(f->xPos + xDir, y).updatePos(f->xPos + xDir, y);
            }
            return 1;
        }
    }

    return 0;
}
bool (*canMoveArr[6])(int, int, Figure*) = {p_cm, n_cm, b_cm, r_cm, q_cm, k_cm};
Figure::Figure(ChessBoard* board, int type, int x, int y) : board(board), xPos(x), yPos(y), type(type) {
    canMove = canMoveArr[type / 2];
    color = type % 2;
}
void Figure::updatePos(int x, int y) {
    lastX = xPos;
    lastY = yPos;
    xPos = x;
    yPos = y;
    ++moves;
    if ((type == WHITE_PAWN && yPos == 0) || (type == BLACK_PAWN && yPos == 7)) {
        updateType(WHITE_QUEEN + color);
    }
}
void Figure::updateType(int newType) {
    type = newType;
    canMove = canMoveArr[type / 2];
}
