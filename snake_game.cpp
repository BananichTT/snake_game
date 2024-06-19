#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>


struct SnakeSegment {
	int x, y;
	SnakeSegment(int x, int y) : x(x), y(y) {}
};

void runGame(sf::RenderWindow& window);
void showMainMenu(sf::RenderWindow& window);
void showGameOverScreen(sf::RenderWindow& window, int score);

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Snake Game");
	showMainMenu(window);
	return 0;
}

void runGame(sf::RenderWindow& window) {
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
		return;
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dy != 1) { new_dx = 0; new_dy = -1; }
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
				showGameOverScreen(window, score);
				return;
			}

			// Проверка на столкновение с самой собой
			for (int i = 1; i < snake.size(); ++i) {
				if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
					showGameOverScreen(window, score);
					return;
				}
			}

			// Проверка на поедание фрукта
			if (snake[0].x == fruitX && snake[0].y == fruitY) {
				// Добавление нового сегмента в хвост змейки
				snake.push_back(SnakeSegment(snake.back().x, snake.back().y));
				fruitX = rand() % (800 / blockSize);
				fruitY = rand() % (600 / blockSize);
				score++;
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
}

void showMainMenu(sf::RenderWindow& window) {
	// Загрузка шрифта
	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		std::cerr << "Error loading font\n";
		return;
	}

	// Настройка текстового объекта для названия игры
	sf::Text title;
	title.setFont(font);
	title.setCharacterSize(48);
	title.setFillColor(sf::Color::White);
	title.setString("Snake Game");
	title.setPosition(250, 100);
	title.setPosition((window.getSize().x - title.getLocalBounds().width) / 2, 100);

	// Настройка кнопок меню
	sf::Text newGameButton;
	newGameButton.setFont(font);
	newGameButton.setCharacterSize(24);
	newGameButton.setFillColor(sf::Color::White);
	newGameButton.setString("New Game");
	newGameButton.setPosition(350, 200);
	newGameButton.setPosition((window.getSize().x - newGameButton.getLocalBounds().width) / 2, 200);

	sf::Text highScoresButton;
	highScoresButton.setFont(font);
	highScoresButton.setCharacterSize(24);
	highScoresButton.setFillColor(sf::Color::White);
	highScoresButton.setString("High Scores");
	highScoresButton.setPosition(350, 250);
	highScoresButton.setPosition((window.getSize().x - highScoresButton.getLocalBounds().width) / 2, 250);

	sf::Text exitButton;
	exitButton.setFont(font);
	exitButton.setCharacterSize(24);
	exitButton.setFillColor(sf::Color::White);
	exitButton.setString("Exit");
	exitButton.setPosition(350, 300);
	exitButton.setPosition((window.getSize().x - exitButton.getLocalBounds().width) / 2, 300);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);

					if (newGameButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
						runGame(window);
					}

					if (highScoresButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
						// Заглушка для таблицы рекордов
						std::cout << "High Scores button pressed\n";
					}

					if (exitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
						window.close();
					}
				}
			}
		}

		window.clear();

		// Отрисовка элементов меню
		window.draw(title);
		window.draw(newGameButton);
		window.draw(highScoresButton);
		window.draw(exitButton);

		window.display();
	}

}

void showGameOverScreen(sf::RenderWindow& window, int score) {
	// Загрузка шрифта
	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		std::cerr << "Error loading font\n";
		return;
	}

	// Настройка текстового объекта для сообщения о проигрыше
	sf::Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(48);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setString("Game Over");
	gameOverText.setPosition((window.getSize().x - gameOverText.getLocalBounds().width) / 2, 100);

	// Настройка текстового объекта для отображения очков
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setString("Score: " + std::to_string(score));
	scoreText.setPosition((window.getSize().x - scoreText.getLocalBounds().width) / 2, 200);

	// Настройка кнопок
	sf::Text retryButton;
	retryButton.setFont(font);
	retryButton.setCharacterSize(24);
	retryButton.setFillColor(sf::Color::White);
	retryButton.setString("Retry");
	retryButton.setPosition((window.getSize().x - retryButton.getLocalBounds().width) / 2, 300);

	sf::Text menuButton;
	menuButton.setFont(font);
	menuButton.setCharacterSize(24);
	menuButton.setFillColor(sf::Color::White);
	menuButton.setString("Menu");
	menuButton.setPosition((window.getSize().x - menuButton.getLocalBounds().width) / 2, 350);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);

					if (retryButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
						runGame(window);
						return;
					}
					if (menuButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
						showMainMenu(window);
						return;
					}
				}
			}
		}

		window.clear();

		// Отрисовка элементов окна проигрыша
		window.draw(gameOverText);
		window.draw(scoreText);
		window.draw(retryButton);
		window.draw(menuButton);

		window.display();
	}
}