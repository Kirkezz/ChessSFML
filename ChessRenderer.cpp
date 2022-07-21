#include "ChessRenderer.h"
using namespace std;
using namespace sf;
ChessRenderer::ChessRenderer(RenderWindow& window, ChessBoard* board) : window(window) {
	if (board == nullptr) {
		board = new ChessBoard();
	}
	this->board = board;
	figureSize = window.getSize().x / boardSize;
	circleT.loadFromFile("Textures/circle.png");
	dotT.loadFromFile("Textures/dot.png");
	circle.setTexture(circleT);
	dot.setTexture(dotT);
	circle.setScale(0.75, 0.75);
	dot.setScale(0.75, 0.75);
}
ChessRenderer::~ChessRenderer() {
	delete board;
}
void ChessRenderer::draw(int mouseX, int mouseY) {
	RectangleShape r(Vector2f(96, 96));
	Sprite f;
	f.setScale(Vector2f(0.375, 0.375)); // (window size / board size) / original size of texture (256)
	f.setOrigin(128, 128);
	for (int x = 0; x < boardSize; ++x) {
		for (int y = 0; y < boardSize; ++y) {
			r.setPosition(x * 96, y * 96);
			r.setFillColor(((x + y) % 2 != 0) ? lightBGColor : DarkBGColor);
			window.draw(r);
			if ((*board)(x, y) && (&(*board)(x, y) != pinned) && find_if(animations.begin(), animations.end(), [x, y](Animation& a) { return a.toX == x && a.toY == y; }) == animations.end()) {
				f.setTexture(textures.getTexture((*board)(x, y).type));
				f.setPosition(x * figureSize + 48, y * figureSize + 48);
				f.setRotation(180 * isBoardFlipped);
				window.draw(f);
			}
		}
	}
	if (pinned) {
		auto t = board->cached.at({ pinned->xPos, pinned->yPos });
		for (int x = 0; x < boardSize; ++x) {
			for (int y = 0; y < boardSize; ++y) {
				if (!t[x][y]) {
					continue;
				}
				Sprite& t1 = (*board)(x, y) ? circle : dot;
				t1.setPosition(x * figureSize, y * figureSize);
				window.draw(t1);
			}
		}
		f.setTexture(textures.getTexture(pinned->type));
		if (!hPin) {
			f.setPosition(mouseX, mouseY);
		}
		else
			f.setPosition(pinned->xPos * figureSize + 48, pinned->yPos * figureSize + 48);
		window.draw(f);
	}
	for (auto it = animations.begin(); it != animations.end();) {
		f.setTexture(textures.getTexture((*board)(it->toX, it->toY).type));
		f.setPosition(it->x * figureSize - it->xOffset * (it->frames - it->frame) + 48, it->y * figureSize - it->yOffset * (it->frames - it->frame) + 48);
		--it->frame;
		f.setRotation(180 * isBoardFlipped);
		window.draw(f);
		if (!it->frame) {
			it = animations.erase(it);
		}
		else {
			++it;
		}
	}
}
void ChessRenderer::LMBPressed(int x, int y) {
	x /= figureSize;
	y /= figureSize;
	if ((!pinned || hPin) && isValid(x, y) && (*board)(x, y) && (*board)(x, y).color == board->moves % 2) {
		pinned = &(*board)(x, y);
		board->generatePossibleMoves(pinned->xPos, pinned->yPos);
		hPin = 0;
	}
}
void ChessRenderer::LMBReleased(int x, int y) {
	x /= figureSize;
	y /= figureSize;
	if (pinned && isValid(x, y)) {
		if (board->move(pinned->xPos, pinned->yPos, x, y)) {
			animations.clear();
			if (hPin) {
				animateMove(pinned->xPos, pinned->yPos, x, y);
			}
			pinned = &(*board)(x, y);
			auto lastMove = board->movesList.back();
			if (pinned->type == WHITE_KING || pinned->type == BLACK_KING && abs(lastMove.first.x - lastMove.second.x) == 2) {
				int rookY = 7 - 7 * pinned->color;
				if (lastMove.second.x == 2) {
					cout << (*board)(3, rookY).type << endl;
					animateMove(0, rookY, 3, rookY);
				} else if(lastMove.second.x == 6) {
					cout << (*board)(5, rookY).type << endl;
					animateMove(7, rookY, 5, rookY);
				}
			}
			pinned = nullptr;
		}
		hPin = 1;
	}
}
void ChessRenderer::animateMove(int x, int y, int toX, int toY) {
	animations.push_back(Animation(x, y, toX, toY, figureSize));
}

