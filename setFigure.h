#pragma once
#include "Figures.h"
class setFigure {
public:
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
	setFigure() {

	}
	void operator()(int figure, int x, int y, int moves, Figure* board[8][8]) {
		if (figure == WHITE_PAWN)
			board[x][y] = new Pawn(WHITE_PAWN, x, y, board, moves);
		if (figure == BLACK_PAWN)
			board[x][y] = new Pawn(BLACK_PAWN, x, y, board, moves);
		if (figure == WHITE_KNIGHT)
			board[x][y] = new Knight(WHITE_KNIGHT, x, y, board, moves);
		if (figure == BLACK_KNIGHT)
			board[x][y] = new Knight(BLACK_KNIGHT, x, y, board, moves);
		if (figure == WHITE_ROOK)
			board[x][y] = new Rook(WHITE_ROOK, x, y, board, moves);
		if (figure == BLACK_ROOK)
			board[x][y] = new Rook(BLACK_ROOK, x, y, board, moves);
		if (figure == WHITE_BISHOP)
			board[x][y] = new Bishop(WHITE_BISHOP, x, y, board, moves);
		if (figure == BLACK_BISHOP)
			board[x][y] = new Bishop(BLACK_BISHOP, x, y, board, moves);
		if (figure == WHITE_QUEEN)
			board[x][y] = new Queen(WHITE_QUEEN, x, y, board, moves);
		if (figure == BLACK_QUEEN)
			board[x][y] = new Queen(BLACK_QUEEN, x, y, board, moves);
		if (figure == WHITE_KING)
			board[x][y] = new King(WHITE_KING, x, y, board, moves);
		if (figure == BLACK_KING)
			board[x][y] = new King(BLACK_KING, x, y, board, moves);
		if (figure == NOTHING)
			board[x][y] = nullptr;
		return 1;
	}
};