#include "Render.h"
#include <string>

Render::Render() : currentState(GameState::MENU) {
    window.create(sf::VideoMode(450, 550), "15 Puzzle - Coursework");
    window.setFramerateLimit(60);

    // Загрузка шрифта с проверкой
    if (!font.loadFromFile("arial.ttf")) {
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }
}

void Render::drawBoard(const GameLogic& game) {
    window.clear(sf::Color(45, 45, 48));

    // Динамический расчет размера плитки в зависимости от выбранной сложности (3, 4 или 5)
    float currentTileSize = 450.0f / game.getSize(); 

    for (int i = 0; i < game.getSize() * game.getSize(); ++i) {
        int val = game.getTile(i);
        if (val == 0) continue;

        // Отрисовка плитки
        sf::RectangleShape tile(sf::Vector2f(currentTileSize - margin, currentTileSize - margin));
        float posX = static_cast<float>(i % game.getSize()) * currentTileSize + margin / 2.0f;
        float posY = static_cast<float>(i / game.getSize()) * currentTileSize + margin / 2.0f;
        
        tile.setPosition(posX, posY);
        tile.setFillColor(sf::Color(0, 122, 204));

        // Отрисовка цифры (размер шрифта подстраивается под количество плиток)
        int fontSize = (game.getSize() >= 5) ? 24 : 36;
        sf::Text text(std::to_string(val), font, fontSize);
        text.setFillColor(sf::Color::White);
        
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(posX + (currentTileSize - margin) / 2.0f, posY + (currentTileSize - margin) / 2.0f);

        window.draw(tile);
        window.draw(text);
    }

    // Информационная панель внизу
    sf::Text info("Moves: " + std::to_string(game.getMoves()) + "  [ESC] to Level Select", font, 20);
    info.setPosition(20.0f, 500.0f);
    window.draw(info);

    if (game.isSolved() && game.getMoves() > 0) {
        sf::Text win("YOU WIN!", font, 50);
        win.setFillColor(sf::Color::Green);
        win.setPosition(110.0f, 220.0f);
        window.draw(win);
    }
}

void Render::run(GameLogic& game) {
    Menu menu(450.0f, 550.0f, font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // Координаты мыши для кликов
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // 1. ЛОГИКА ГЛАВНОГО МЕНЮ
            if (currentState == GameState::MENU) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) menu.moveUp(currentState);
                    if (event.key.code == sf::Keyboard::Down) menu.moveDown(currentState);
                    if (event.key.code == sf::Keyboard::Enter) {
                        int choice = menu.getPressedItem();
                        if (choice == 0) { currentState = GameState::DIFFICULTY_SELECT; menu.resetSelection(); }
                        if (choice == 1) currentState = GameState::ABOUT;
                        if (choice == 2) window.close();
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    int click = menu.getClickedItem(mousePos, currentState);
                    if (click == 0) { currentState = GameState::DIFFICULTY_SELECT; menu.resetSelection(); }
                    else if (click == 1) currentState = GameState::ABOUT;
                    else if (click == 2) window.close();
                }
            }
            
            // 2. ЛОГИКА ВЫБОРА СЛОЖНОСТИ
            else if (currentState == GameState::DIFFICULTY_SELECT) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) menu.moveUp(currentState);
                    if (event.key.code == sf::Keyboard::Down) menu.moveDown(currentState);
                    if (event.key.code == sf::Keyboard::Enter) {
                        int choice = menu.getPressedItem();
                        if (choice == 0) { game.init(3); game.shuffle(); currentState = GameState::PLAYING; }
                        else if (choice == 1) { game.init(4); game.shuffle(); currentState = GameState::PLAYING; }
                        else if (choice == 2) { game.init(5); game.shuffle(); currentState = GameState::PLAYING; }
                        else if (choice == 3) { currentState = GameState::MENU; menu.resetSelection(); }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    int click = menu.getClickedItem(mousePos, currentState);
                    if (click == 0) { game.init(3); game.shuffle(); currentState = GameState::PLAYING; }
                    else if (click == 1) { game.init(4); game.shuffle(); currentState = GameState::PLAYING; }
                    else if (click == 2) { game.init(5); game.shuffle(); currentState = GameState::PLAYING; }
                    else if (click == 3) { currentState = GameState::MENU; menu.resetSelection(); }
                }
            }

            // 3. ЛОГИКА САМОЙ ИГРЫ
            else if (currentState == GameState::PLAYING) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    float curTileSize = 450.0f / game.getSize();
                    int x = static_cast<int>(event.mouseButton.x / curTileSize);
                    int y = static_cast<int>(event.mouseButton.y / curTileSize);
                    if (x < game.getSize() && y < game.getSize()) game.move(y * game.getSize() + x);
                }
                // Назад к выбору уровней по ESC
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::DIFFICULTY_SELECT;
                    menu.resetSelection();
                }
            }

            // 4. ЭКРАН "О ПРОГРАММЕ"
            else if (currentState == GameState::ABOUT) {
                if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
                    currentState = GameState::MENU;
            }
        }

        // Рендеринг в зависимости от состояния
        window.clear(sf::Color(45, 45, 48));
        if (currentState == GameState::MENU || currentState == GameState::DIFFICULTY_SELECT) {
            menu.draw(window, currentState);
        } else if (currentState == GameState::PLAYING) {
            drawBoard(game);
        } else if (currentState == GameState::ABOUT) {
            sf::Text t("Coursework: 15 Puzzle\nAuthor: Student\n\nClick to return...", font, 30);
            t.setPosition(50, 150);
            window.draw(t);
        }
        window.display();
    }
}