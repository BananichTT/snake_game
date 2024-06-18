#include <SFML/Graphics.hpp>
#include <vector>


struct SnakeSegment {
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y){}
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Snake Game");

    std::vector<SnakeSegment> snake;
    snake.push_back(SnakeSegment(5, 5));  // Начальная позиция змейки
    const int blockSize = 20;  // Размер блока змейки

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (auto& segment : snake) {
            sf::RectangleShape rectangle(sf::Vector2f(blockSize, blockSize));
            rectangle.setFillColor(sf::Color::Green);
            rectangle.setPosition(segment.x * blockSize, segment.y * blockSize);
            window.draw(rectangle);
        }

        window.display();
    }

    return 0;
}