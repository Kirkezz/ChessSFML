#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Drawer_Textures.h"
using namespace sf;
extern RenderWindow* window;
class Drawer {
private:
	Textures textures;
	Texture circleT, moveT;
	Sprite circle, move;
	Chess& board;
	Sprite pinnedFigure;
	int map[8][8] = { {0} };
	void generateMovesMap() {
		for (int x = 0; x < 8; ++x) {
			for (int y = 0; y < 8; ++y) {
				if (board.canMove(xSel, ySel, x, y, 0)) {
					if (board(x, y)) {
						map[x][y] = 1;
					}
					else {
						map[x][y] = 2;
					}
				}
				else {
					map[x][y] = 0;
				}
			}
		}
	}
public:
	int figureSize, boardSize = 8, xSel = -1, ySel = 0;
	Drawer(int boardSize, Chess& board) : boardSize(boardSize), board(board) {
		figureSize = window->getSize().x / boardSize;
		circleT.loadFromFile("Textures/circle.png");
		moveT.loadFromFile("Textures/move.png");
		circle.setTexture(circleT);
		move.setTexture(moveT);
		circle.setScale(0.75, 0.75);
		move.setScale(0.75, 0.75);
		pinnedFigure.setScale(Vector2f(0.383, 0.383));
		pinnedFigure.setOrigin(128, 128); // original size of texture = 256
	}
	void drawFigure(int figure, int x, int y) {
		if (!(xSel == x && ySel == y)) {
			Sprite temp;
			temp.setTexture(textures.getTexture(figure));
			temp.setScale(Vector2f(0.375, 0.375)); // (window size / board size) / original size of texture
			temp.setPosition(x * figureSize, y * figureSize);
			window->draw(temp);
		}
	}
	void drawBoard() {
		RectangleShape temp(Vector2f(96, 96));
		for (int x = 0; x < boardSize; ++x) {
			for (int y = 0; y < boardSize; ++y) {
				temp.setPosition(x * 96, y * 96);
				if ((x + y) % 2 != 0) {
					temp.setFillColor(Color(225, 225, 200));
				}
				else {
					temp.setFillColor(Color(245, 110, 110));
				}
				window->draw(temp);
			}
		}
	}
	void drawPinnedFigure(double mouseX, double mouseY) {
		if (xSel != -1 && board(xSel, ySel)) {
			for (int i = 0; i < 8; ++i) {
				for (int j = 0; j < 8; ++j) {
					switch (map[i][j]) {
					case 1:
						circle.setPosition(i * figureSize, j * figureSize);
						window->draw(circle);
						break;
					case 2:
						move.setPosition(i * figureSize, j * figureSize);
						window->draw(move);
						break;
					}
				}
			}
			pinnedFigure.setTexture(textures.getTexture(board(xSel, ySel)->figure));
			pinnedFigure.setPosition(mouseX, mouseY);
			window->draw(pinnedFigure);
		}
	}
	void LMBPressed(int x, int y) {
		if (xSel == -1 && x / figureSize >= 0 && x / figureSize < boardSize && y / figureSize >= 0 && y / figureSize < boardSize) {
			xSel = x / figureSize;
			ySel = y / figureSize;
			if (board(xSel, ySel)) {
				generateMovesMap();
			}
		}
	}
	void LMBReleased(int x, int y) {
		if (xSel != -1) {
			board.moveFromTo(xSel, ySel, x / figureSize, y / figureSize);
			xSel = -1;
		}
	}
};
