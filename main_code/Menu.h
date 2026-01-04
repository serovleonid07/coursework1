#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class GameState { MENU, DIFFICULTY_SELECT, PLAYING, ABOUT, EXIT };

class Menu {
private:
    int selectedItemIndex;
    sf::Font font;
    std::vector<sf::Text> mainMenuItems;
    std::vector<sf::Text> difficultyItems;

public:
    Menu(float width, float height, sf::Font& sharedFont);
    void draw(sf::RenderWindow& window, GameState state); // Теперь принимает текущее состояние
    void moveUp(GameState state);
    void moveDown(GameState state);
    int getPressedItem() const { return selectedItemIndex; }
    int getClickedItem(sf::Vector2f mousePos, GameState state);
    void resetSelection() { selectedItemIndex = 0; }
};