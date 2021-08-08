#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;
class Figure {
public:
    int isUnderAttack(int x, int y) {
        int count = 0;
        for (int x1 = 0; x1 < 8; ++x1) {
            for (int y1 = 0; y1 < 8; ++y1) {
                if (board[x1][y1] && board[x1][y1]->color != color && board[x1][y1]->canMoveAt(x, y, 0)) {
                    ++count;
                }
            }
        }
        return count;
    }
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
    int figure = 0, xPos = 0, yPos = 0, moves = 0, color = 0;
    int& lastX, & lastY;
    Figure* (&board)[8][8];
    Figure(int figure, int x, int y, int& lastX, int& lastY, Figure* (&board)[8][8], int moves = 0) : figure(figure), moves(moves), xPos(x), yPos(y), board(board), lastX(lastX), lastY(lastY) {
        color = figure % 2;
    }
    virtual bool canMoveAt(int x, int y, bool moveAfterCheck = 1) { return 1; }
};
class Pawn : public Figure {
public:
    using Figure::Figure;
    virtual bool canMoveAt(int x, int y, bool moveAfterCheck = 1) {
        if (!color) {
            if (((y - yPos == -1) || (yPos == 6 && y - yPos == -2 && !board[x][y + 1])) && !board[x][y] && x == xPos)
                return 1;
            if (abs(x - xPos) == 1 && y - yPos == -1 && board[x][y])
                return 1;
        }
        else {
            if (((y - yPos == 1) || (yPos == 1 && y - yPos == 2 && !board[x][y - 1])) && !board[x][y] && x == xPos)
                return 1;
            if (abs(x - xPos) == 1 && y - yPos == 1 && board[x][y])
                return 1;
        }
        if (yPos == (!color ? 3 : 4) && abs(x - xPos) == 1 && y - yPos == (!color ? -1 : 1) && !board[x][y] && board[x][y + (!color ? 1 : -1)] && board[x][y + (!color ? 1 : -1)]->figure < 2 && board[x][y + (!color ? 1 : -1)]->moves == 1 && lastX == x && lastY == (y + (!color ? 1 : -1))) {
            if (moveAfterCheck)
                board[x][y + (!color ? 1 : -1)] = nullptr;
            return 1;
        }
        return 0;
    }
};
class Bishop : public Figure {
public:
    using Figure::Figure;
    virtual bool canMoveAt(int x, int y, bool moveAfterCheck = 1) {
        if (abs(x - xPos) == abs(y - yPos)) {
            if (xPos > x && yPos > y)
                for (int i = 1; i < 8; ++i)
                    if (board[xPos - i][yPos - i] || xPos - i == 0 || yPos - i == 0)
                        return xPos - i <= x && yPos - i <= y;
            if (xPos > x && yPos < y)
                for (int i = 1; i < 8; ++i)
                    if (board[xPos - i][yPos + i] || xPos - i == 0 || yPos + i == 7)
                        return xPos - i <= x && yPos + i >= y;
            if (xPos < x && yPos > y)
                for (int i = 1; i < 8; ++i)
                    if (board[xPos + i][yPos - i] || xPos + i == 7 || yPos - i == 0)
                        return xPos + i >= x && yPos - i <= y;
            if (xPos < x && yPos < y)
                for (int i = 1; i < 8; ++i)
                    if (board[xPos + i][yPos + i] || xPos + i == 7 || yPos + i == 7)
                        return xPos + i >= x && yPos + i >= y;
        }
        return 0;
    }
};
class Rook : public Figure {
public:
    using Figure::Figure;
    virtual bool canMoveAt(int x, int y, bool moveAfterCheck = 1) {
        if (x == xPos && y < yPos)
            for (int i = yPos - 1; i > -1; --i)
                if (board[xPos][i] || i == 0)
                    return y >= i;
        if (x == xPos && y > yPos)
            for (int i = yPos + 1; i < 8; ++i)
                if (board[xPos][i] || i == 7)
                    return y <= i;
        if (x < xPos && y == yPos)
            for (int i = xPos - 1; i > -1; --i)
                if (board[i][yPos] || i == 0)
                    return x >= i;
        if (x > xPos && y == yPos)
            for (int i = xPos + 1; i < 8; ++i)
                if (board[i][yPos] || i == 7)
                    return x <= i;
        return 0;
    }
};
class Knight : public Figure {
private:
    const pair<int, int> moves[8] = { {-1, 2}, {-1, -2}, {1, 2}, {1, -2}, {-2, 1}, {-2, -1}, {2, 1}, {2, -1} };
public:
    using Figure::Figure;
    virtual bool canMoveAt(int x, int y, bool moveAfterCheck = 1) {
        x -= xPos; y -= yPos;
        for (int i = 0; i < 8; ++i) {
            if (x == moves[i].first && y == moves[i].second)
                return 1;
        }
        return 0;
    }
};
class Queen : public Figure {
public:
    using Figure::Figure;
    virtual bool canMoveAt(int x, int y, bool moveAfterCheck = 1) {
        if (x == xPos && y < yPos)
            for (int i = yPos - 1; i > -1; --i)
                if (board[xPos][i] || i == 0)
                    return y >= i;
        if (x == xPos && y > yPos)
            for (int i = yPos + 1; i < 8; ++i)
                if (board[xPos][i] || i == 7)
                    return y <= i;
        if (x < xPos && y == yPos)
            for (int i = xPos - 1; i > -1; --i)
                if (board[i][yPos] || i == 0)
                    return x >= i;
        if (x > xPos && y == yPos)
            for (int i = xPos + 1; i < 8; ++i)
                if (board[i][yPos] || i == 7)
                    return x <= i;
        if (abs(x - xPos) == abs(y - yPos)) {
            if (xPos > x && yPos > y)
                for (int i = 1; i < 8; ++i)
                    if (board[xPos - i][yPos - i] || xPos - i == 0 || yPos - i == 0)
                        return xPos - i <= x && yPos - i <= y;
            if (xPos > x && yPos < y)
                for (int i = 1; i < 8; ++i)
                    if (board[xPos - i][yPos + i] || xPos - i == 0 || yPos + i == 7)
                        return xPos - i <= x && yPos + i >= y;
            if (xPos < x && yPos > y)
                for (int i = 1; i < 8; ++i)
                    if (board[xPos + i][yPos - i] || xPos + i == 7 || yPos - i == 0)
                        return xPos + i >= x && yPos - i <= y;
            if (xPos < x && yPos < y)
                for (int i = 1; i < 8; ++i)
                    if (board[xPos + i][yPos + i] || xPos + i == 7 || yPos + i == 7)
                        return xPos + i >= x && yPos + i >= y;
        }
        return 0;
    }
};
class King : public Figure {
public:
    using Figure::Figure;
    virtual bool canMoveAt(int x, int y, bool moveAfterCheck = 1) {
        if (abs(x - xPos) < 2 && abs(y - yPos) < 2)
            return 1;
        if (abs(x - xPos) == 2 && yPos == y && moves == 0 && !isUnderAttack(xPos, yPos) && !isUnderAttack(x, y)) {
            if (x == 2 && !isUnderAttack(3, yPos)) {
                if (!board[1][yPos] && !board[2][yPos] && !board[3][yPos] && board[0][yPos] && board[0][yPos]->figure == (WHITE_ROOK + figure % 2) && board[0][yPos]->moves == 0) {
                    if (moveAfterCheck) {
                        board[x + 1][y] = new Rook(WHITE_ROOK + figure % 2, x + 1, y, lastX, lastY, board, 1);
                        board[0][yPos] = nullptr;
                    }
                    return 1;
                }
            }
            if (x == 6 && !isUnderAttack(5, yPos)) {
                if (!board[6][yPos] && !board[5][yPos] && board[7][yPos] && board[7][yPos]->figure == (WHITE_ROOK + figure % 2) && board[7][yPos]->moves == 0) {
                    if (moveAfterCheck) {
                        board[x - 1][y] = new Rook(WHITE_ROOK + figure % 2, x - 1, y, lastX, lastY, board, 1);
                        board[7][yPos] = nullptr;
                    }
                    return 1;
                }
            }
        }
        return 0;
    }
};