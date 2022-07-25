#include <SFML/Graphics.hpp>
#include "ChessRenderer.h"
using namespace sf;

int main() {
    RenderWindow window(VideoMode(768, 768), "Chess");
    window.setFramerateLimit(60);
    Vector2f mouse;
    ChessRenderer chess(window);
    while (window.isOpen()) {
        mouse = window.mapPixelToCoords(Mouse::getPosition(window));
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                exit(0);
            if (event.type == event.MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                chess.LMBPressed(mouse.x, mouse.y);
            }
            else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                chess.LMBReleased(mouse.x, mouse.y);
            }
            else if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                case Keyboard::F:
                    chess.flipBoard();
                    break;
                case Keyboard::R:
                    chess.resetBoard();
                    break;
                case Keyboard::A:
                    chess.autoFlip = !chess.autoFlip;
                    break;
                }
            }
        }
        window.clear(Color(0, 0, 0));
        chess.draw(mouse.x, mouse.y);
        window.display();
    }
}