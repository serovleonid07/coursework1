#include "Render.h"
#include <string>

Render::Render() {
    // Создаем окно (ширина 450, высота 550 для места под счетчик)
    window.create(sf::VideoMode(450, 550), "Coursework");
    window.setFramerateLimit(60);

    // Загрузка шрифта. Файл должен лежать в папке с .exe или в корне проекта
    if (!font.loadFromFile("arial.ttf")) {
        // Если шрифт не найден, можно вывести ошибку в консоль
        // Для курсовой лучше добавить проверку
    }
}

void Render::drawBoard(const GameLogic& game) {
    window.clear(sf::Color(45, 45, 48)); // Темно-серый фон (стиль VS)

    float fTileSize = static_cast<float>(tileSize);
    float fMargin = static_cast<float>(margin);

    for (int i = 0; i < game.getSize() * game.getSize(); ++i) {
        int val = game.getTile(i);
        
        // Пропускаем отрисовку пустой клетки (ноль)
        if (val == 0) continue;

        // Рисуем плитку
        sf::RectangleShape tile(sf::Vector2f(fTileSize - fMargin, fTileSize - fMargin));
        float posX = static_cast<float>(i % game.getSize()) * fTileSize + fMargin / 2.0f;
        float posY = static_cast<float>(i / game.getSize()) * fTileSize + fMargin / 2.0f;
        
        tile.setPosition(posX, posY);
        tile.setFillColor(sf::Color(0, 122, 204)); // Синий цвет плиток
        tile.setOutlineThickness(2.0f);
        tile.setOutlineColor(sf::Color::White);

        // Настраиваем текст (номер плитки)
        sf::Text text(std::to_string(val), font, 36);
        text.setFillColor(sf::Color::White);
        
        // Центрирование текста внутри плитки
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(posX + (fTileSize - fMargin) / 2.0f, posY + (fTileSize - fMargin) / 2.0f);

        window.draw(tile);
        window.draw(text);
    }

    // Отрисовка интерфейса (счетчик ходов)
    sf::Text info("Moves: " + std::to_string(game.getMoves()), font, 24);
    info.setPosition(20.0f, 480.0f);
    info.setFillColor(sf::Color::Yellow);
    window.draw(info);

    sf::Text hint("Press 'R' to Restart", font, 18);
    hint.setPosition(20.0f, 515.0f);
    hint.setFillColor(sf::Color(200, 200, 200));
    window.draw(hint);

    // Сообщение о победе
    if (game.isSolved() && game.getMoves() > 0) {
        sf::RectangleShape overlay(sf::Vector2f(450.0f, 550.0f));
        overlay.setFillColor(sf::Color(0, 0, 0, 150)); // Полупрозрачный фон
        window.draw(overlay);

        sf::Text winText("VICTORY!", font, 60);
        winText.setFillColor(sf::Color::Green);
        sf::FloatRect tr = winText.getLocalBounds();
        winText.setOrigin(tr.left + tr.width / 2.0f, tr.top + tr.height / 2.0f);
        winText.setPosition(225.0f, 250.0f);
        window.draw(winText);
    }

    window.display();
}

void Render::run(GameLogic& game) {
    game.shuffle(); // Перемешиваем при старте

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Обработка клика мыши
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int px = event.mouseButton.x / tileSize;
                    int py = event.mouseButton.y / tileSize;
                    
                    if (px < game.getSize() && py < game.getSize()) {
                        game.move(py * game.getSize() + px);
                    }
                }
            }

            // Перезапуск на клавишу R
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    game.shuffle();
                }
            }
        }

        drawBoard(game);
    }
}