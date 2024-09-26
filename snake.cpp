#include <iostream>
#include <conio.h> // Для _kbhit() и _getch()
#include <windows.h> // Для Sleep()
#include <vector>
#include <cstdlib> // Для rand() и srand()
#include <ctime> // Для time()
#include <array>

using namespace std;

const int width = 20;
const int height = 20;
bool gameOver;
int x, y, fruitX, fruitY, score;
vector<pair<int, int>> snake; // Хранит координаты змейки

enum class Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void Setup() {
    gameOver = false;
    dir = Direction::STOP;
    x = width / 2;
    y = height / 2;
    score = 0;
    snake.push_back(make_pair(x, y)); // Начальная позиция змейки
    fruitX = rand() % width;
    fruitY = rand() % height;
}

void Draw() {
    system("cls"); // Очистка консоли
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#"; // Левая граница
            if (i == y && j == x)
                cout << "O"; // Змейка
            else if (i == fruitY && j == fruitX)
                cout << "F"; // Фрукт
            else {
                bool print = false;
                for (const auto& s : snake) {
                    if (s.first == j && s.second == i) {
                        cout << "o"; // Тело змейки
                        print = true;
                        break;
                    }
                }
                if (!print)
                    cout << " ";
            }
            if (j == width - 1)
                cout << "#"; // Правая граница
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            dir = Direction::LEFT;
            break;
        case 'd':
            dir = Direction::RIGHT;
            break;
        case 'w':
            dir = Direction::UP;
            break;
        case 's':
            dir = Direction::DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void GenerateFruit() {
    do {
        fruitX = rand() % width;
        fruitY = rand() % height;
    } while (find(snake.begin(), snake.end(), make_pair(fruitX, fruitY)) != snake.end());
}

void Logic() {
    int prevX = snake[0].first;
    int prevY = snake[0].second;
    int prev2X, prev2Y;
    snake[0].first = x;
    snake[0].second = y;

    for (size_t i = 1; i < snake.size(); i++) {
        prev2X = snake[i].first;
        prev2Y = snake[i].second;
        snake[i].first = prevX;
        snake[i].second = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case Direction::LEFT:
        x--;
        break;
    case Direction::RIGHT:
        x++;
        break;
    case Direction::UP:
        y--;
        break;
    case Direction::DOWN:
        y++;
        break;
    default:
        break;
    }

    // Проверка на столкновение с границами
    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    // Проверка на столкновение с фруктом
    if (x == fruitX && y == fruitY) {
        score += 10;
        GenerateFruit();
        snake.push_back(make_pair(-1, -1)); // Увеличиваем длину змейки
    }

    // Проверка на столкновение с телом змейки
    for (size_t i = 1; i < snake.size(); i++) {
        if (snake[i].first == x && snake[i].second == y) {
            gameOver = true;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // Задержка для управления скоростью игры
    }
    return 0;
}