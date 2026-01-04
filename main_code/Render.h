//Render.h — Графический интерфейс

#pragma once
#include <SFML/Graphics.hpp>
#include "GameLogic.h"
#include "Menu.h"

class Render {
private:
    sf::RenderWindow window;
    sf::Font font;
    GameState currentState;
    const int tileSize = 100;
    const int margin = 10;

public:
    Render();
    void run(GameLogic& game);
    void drawBoard(const GameLogic& game);
};