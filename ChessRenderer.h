#pragma once
#include <SFML/Graphics.hpp>
#include "ChessBoard.h"
#include <algorithm>
#include <vector>
#include <cmath>
class ChessRendererTextures {
public:
    std::vector<sf::Texture> textures;
    const int n = 12;
    ChessRendererTextures() {
        textures.resize(n);
        for (int i = 0; i < n; ++i) {
            textures[i].loadFromFile("Textures/" + std::to_string(i) + ".png");
            textures[i].setRepeated(1);
        }
    }
    sf::Texture& getTexture(int figure) {
        return textures[figure];
    }
};
class ChessRenderer {
public:
	ChessRenderer(sf::RenderWindow& window, ChessBoard* board = nullptr);
	~ChessRenderer();
	void draw(int mouseX, int mouseY);
	sf::Color lightBGColor = sf::Color(225, 225, 200), DarkBGColor = sf::Color(245, 110, 110);
	void LMBPressed(int x, int y);
    void LMBReleased(int x, int y);
    void flipBoard() {
        isBoardFlipped = !isBoardFlipped;
        sf::View t = window.getView(); t.rotate(180); window.setView(t);
    }
    void resetBoard() {
        delete board;
        board = new ChessBoard;
        pinned = nullptr;
        animations.clear();
    }
    void makeRandomMove() {
        int a[4];
        int i = 10000;
        do {
            for (int i = 0; i < 4; ++i)
                a[i] = rand() % boardSize;
            --i;
        } while (!board->move(a[0], a[1], a[2], a[3]) && i);
    }
private:
    void animateMove(int x, int y, int toX, int toY);
    struct Animation {
        Animation(int x, int y, int toX, int toY, int figureSize) : toX(toX), toY(toY), x(x), y(y) {
            xOffset = double(x - toX) * figureSize / frames;
            yOffset = double(y - toY) * figureSize / frames;
            frame = frames;
        }
        double xOffset, yOffset;
        int toX, toY, x, y, frame, frames = 16;
    };
    std::vector<Animation> animations;
    bool isValid(int x, int y) {
        return x >= 0 && x < boardSize && y >= 0 && y < boardSize;
    }
    bool isBoardFlipped = false;
	const int boardSize = 8;
    int figureSize;
    Figure* pinned;
    bool hPin = false;
	ChessRendererTextures textures;
    sf::Texture circleT, dotT;
    sf::Sprite circle, dot;
	ChessBoard* board;
	sf::RenderWindow& window;
};

