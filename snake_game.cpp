#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>


struct SnakeSegment {
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y){}
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Snake Game");

    std::vector<SnakeSegment> snake;
    snake.push_back(SnakeSegment(5, 5));  // Начальная позиция змейки

    const int blockSize = 20;  // Размер блока змейки
    int dx = 1, dy = 0;  // Направление движения
    int score = 0;

    sf::Clock clock;
    float timer = 0, delay = 0.1;

    // Генерация фрукта
    srand(time(0));
    int fruitX = rand() % (800 / blockSize);
    int fruitY = rand() % (600 / blockSize);

    // Загрузка шрифта
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    // Настройка текстового объекта для отображения очков
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 10);

    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        timer += time;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Локальные переменные для направления
        int new_dx = dx, new_dy = dy;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dy !=1) { new_dx = 0; new_dy = -1; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dy != -1) { new_dx = 0; new_dy = 1; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dx != 1) { new_dx = -1; new_dy = 0; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dx != -1) { new_dx = 1; new_dy = 0; }

        if (timer > delay) {
            timer = 0;

            dx = new_dx;
            dy = new_dy;

            for (int i = snake.size() - 1; i > 0; i--) {
                snake[i].x = snake[i - 1].x;
                snake[i].y = snake[i - 1].y;
            }
            snake[0].x += dx;
            snake[0].y += dy;

            // Проверка на столкновение со стенками
            if (snake[0].x < 0 || snake[0].x >= 800 / blockSize || snake[0].y < 0 || snake[0].y >= 600 / blockSize) {
                window.close();
            }

            // Проверка на столкновение с самой собой
            for (int i = 1; i < snake.size(); ++i) {
                if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                    window.close();
                    }
                }

            // Проверка на поедание фрукта
            if (snake[0].x == fruitX && snake[0].y == fruitY) {
                // Добавление нового сегмента в хвост змейки
                snake.push_back(SnakeSegment(snake.back().x, snake.back().y));
                fruitX = rand() % (800 / blockSize);
                fruitY = rand() % (600 / blockSize);
                score ++;
                std::cout << "Score: " << score << std::endl;
            }
        }

        window.clear();

        // Обновление текстового объекта с очками
        text.setString("Score: " + std::to_string(score));

        // Отрисовка змейки
        for (auto& segment : snake) {
            sf::RectangleShape rectangle(sf::Vector2f(blockSize, blockSize));
            rectangle.setFillColor(sf::Color::Green);
            rectangle.setPosition(segment.x * blockSize, segment.y * blockSize);
            window.draw(rectangle);
        }

        // Отрисовка фрукта
        sf::RectangleShape fruit(sf::Vector2f(blockSize, blockSize));
        fruit.setFillColor(sf::Color::Red);
        fruit.setPosition(fruitX * blockSize, fruitY * blockSize);
        window.draw(fruit);

        // Отрисовка текста с очками
        window.draw(text);

        window.display();
    }

    return 0;
}