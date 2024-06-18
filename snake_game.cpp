#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>


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

    sf::Clock clock;
    float timer = 0, delay = 0.1;

    // Генерация фрукта
    srand(time(0));
    int fruitX = rand() % (800 / blockSize);
    int fruitY = rand() % (600 / blockSize);

    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        timer += time;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { dx = 0; dy = -1; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { dx = 0; dy = 1; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { dx = -1; dy = 0; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { dx = 1; dy = 0; }

        if (timer > delay) {
            timer = 0;

            for (int i = snake.size() - 1; i > 0; i--) {
                snake[i].x = snake[i - 1].x;
                snake[i].y = snake[i - 1].y;
            }
            snake[0].x += dx;
            snake[0].y += dy;

            // Проверка на поедание фрукта
            if (snake[0].x == fruitX && snake[0].y == fruitY) {
                snake.push_back(SnakeSegment(fruitX, fruitY));
                fruitX = rand() % (800 / blockSize);
                fruitY = rand() % (600 / blockSize);
            }
        }

        window.clear();

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

        window.display();
    }

    return 0;
}