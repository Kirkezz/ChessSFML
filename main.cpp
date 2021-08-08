#include <SFML/Graphics.hpp>
#include "Drawer.h"
#include "Board.h"
#include "Figures.h"
#include <iostream>
using namespace std;
using namespace sf;
RenderWindow* window;
int main() { 
    window = new RenderWindow(VideoMode(768, 768), "Chess");
    window->setFramerateLimit(60);
    Vector2f mouse;
    Chess chess;
    Drawer drawer(8, chess);
    while (window->isOpen()) {
        mouse = window->mapPixelToCoords(Mouse::getPosition(*window));
        Event event;
        while (window->pollEvent(event)) {
            if (event.type == Event::Closed)
                window->close();
            if (event.type == event.MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                drawer.LMBPressed(mouse.x, mouse.y);
            }
            if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                drawer.LMBReleased(mouse.x, mouse.y);
            }
            if (event.type == event.MouseButtonPressed && event.mouseButton.button == Mouse::Right) {
                
            }
            if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) {

            }
            if (event.type == Event::KeyPressed) {

            }
        }
        window->clear(Color(0, 0, 0));
        drawer.drawBoard();
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (chess(i, j))
                    drawer.drawFigure(chess(i, j)->figure, chess(i, j)->xPos, chess(i, j)->yPos);
            }
        }
        drawer.drawPinnedFigure(mouse.x, mouse.y);
        window->display();
    }
}