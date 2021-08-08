#include "Figures.h"
#include <assert.h>
#pragma once
using namespace sf;
class Chess {
private:
	int moves = 0, lastX = 0, lastY = 0, bKingX = 4, bKingY = 0, wKingX = 4, wKingY = 7;
	Figure* board[8][8];
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
	int initialPosition[8][8] = {
		7, 3, 5, 9, 11, 5, 3, 7, 
		1, 1, 1, 1, 1, 1, 1, 1,
		12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12,
		0, 0, 0, 0, 0, 0, 0, 0,
		6, 2, 4, 8, 10, 4, 2, 6
	};
	void loadBoard() {
		for (int x = 0; x < 8; ++x) {
			for (int y = 0; y < 8; ++y) {
				setFigure(initialPosition[y][x], x, y);
			}
		}
	}
	void setFigure(int figure, int x, int y, int moves = 0) {
		if (figure == WHITE_PAWN)
			board[x][y] = new Pawn(WHITE_PAWN, x, y, lastX, lastY, board, moves);
		if (figure == BLACK_PAWN)
			board[x][y] = new Pawn(BLACK_PAWN, x, y, lastX, lastY, board, moves);
		if (figure == WHITE_KNIGHT)
			board[x][y] = new Knight(WHITE_KNIGHT, x, y, lastX, lastY, board, moves);
		if (figure == BLACK_KNIGHT)
			board[x][y] = new Knight(BLACK_KNIGHT, x, y, lastX, lastY, board, moves);
		if (figure == WHITE_ROOK)
			board[x][y] = new Rook(WHITE_ROOK, x, y, lastX, lastY, board, moves);
		if (figure == BLACK_ROOK)
			board[x][y] = new Rook(BLACK_ROOK, x, y, lastX, lastY, board, moves);
		if (figure == WHITE_BISHOP)
			board[x][y] = new Bishop(WHITE_BISHOP, x, y, lastX, lastY, board, moves);
		if (figure == BLACK_BISHOP)
			board[x][y] = new Bishop(BLACK_BISHOP, x, y, lastX, lastY, board, moves);
		if (figure == WHITE_QUEEN)
			board[x][y] = new Queen(WHITE_QUEEN, x, y, lastX, lastY, board, moves);
		if (figure == BLACK_QUEEN)
			board[x][y] = new Queen(BLACK_QUEEN, x, y, lastX, lastY, board, moves);
		if (figure == WHITE_KING)
			board[x][y] = new King(WHITE_KING, x, y, lastX, lastY, board, moves);
		if (figure == BLACK_KING)
			board[x][y] = new King(BLACK_KING, x, y, lastX, lastY, board, moves);
		if (figure == NOTHING)
			board[x][y] = nullptr;
	}
	void move(int x1, int y1, int x2, int y2, bool addMove = true) {
		if (!board[x1][y1] || (x1 == x2 && y1 == y2))
			return;
		setFigure(board[x1][y1]->figure, x2, y2, board[x1][y1]->moves + addMove);
		board[x1][y1] = nullptr;
		if ((y2 == 0 && board[x2][y2]->figure == WHITE_PAWN) || (y2 == 7 && board[x2][y2]->figure == BLACK_PAWN)) {
			setFigure(WHITE_QUEEN + board[x2][y2]->color, x2, y2, 1);
		}
		if (board[x2][y2]->figure == BLACK_KING) {
			bKingX = x2; bKingY = y2;
		}
		if (board[x2][y2]->figure == WHITE_KING) {
			wKingX = x2; wKingY = y2;
		}
		if (addMove) {
			lastX = x2; lastY = y2;
		}
	}
	int isUnderAttack(int x, int y) {
		int count = 0;
		for (int x1 = 0; x1 < 8; ++x1) {
			for (int y1 = 0; y1 < 8; ++y1) {
				if (board[x1][y1] && board[x1][y1]->color != board[x][y]->color && board[x1][y1]->canMoveAt(x, y, 0)) {
					++count;
				}
			}
		}
		return count;
	}
	pair<int, int> kingChecked() {
		return { isUnderAttack(bKingX, bKingY), isUnderAttack(wKingX, wKingY) };
	}
public:
	Chess() {
		loadBoard();
	}
	bool updateNextFrame = true;
	void update() {
		
	}
	Figure*& operator()(int x, int y) {
		assert(x >= 0 && x <= 7 || y >= 0 || y <= 7);
		return board[x][y];
	}
	void moveFromTo(int x, int y, int toX, int toY) {
		if (canMove(x, y, toX, toY)) {
			move(x, y, toX, toY);
			++moves;
		}
	}
	bool canMove(int x, int y, int toX, int toY, bool moveAfter = 1) {
		if (board[x][y] && (!board[toX][toY] || board[toX][toY]->color != board[x][y]->color) && moves % 2 == board[x][y]->color && board[x][y]->canMoveAt(toX, toY, moveAfter)) {
			int figureOld = board[toX][toY] ? board[toX][toY]->figure : NOTHING;
			int movesOld = board[toX][toY] ? board[toX][toY]->figure : 0;
			pair<int, int> before = kingChecked();
			move(x, y, toX, toY, 0);
			pair<int, int> after = kingChecked();
			move(toX, toY, x, y, 0);
			setFigure(figureOld, toX, toY, movesOld);
			if ((before.first && after.first) || (before.second && after.second) || (after.first && moves % 2) || (after.second && !(moves % 2)))
				return 0;
			return 1;
		}
		return 0;
	}
};