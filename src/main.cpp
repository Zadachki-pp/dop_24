#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>


enum class VectorDirection {up, down};


float SHAPE_RADIUS = 30;
float C = 100;  // Коэфицент соправтивление воздуху
float G = 9.8f;  // px на sec
int WIGHT = 800;
int HEIGHT = 800;


int main() {
    sf::RenderWindow window(sf::VideoMode(WIGHT, HEIGHT), "Test");


    sf::CircleShape shape(SHAPE_RADIUS);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition((float)WIGHT / 2 - SHAPE_RADIUS, 10);
    float v = 0;
    VectorDirection direction = VectorDirection::down;


    window.display();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        sf::Vector2f position = shape.getPosition();
        float next_y;
        if (direction == VectorDirection::down) {
            v += G;
            next_y = position.y + v;

            if (next_y > (float)HEIGHT - shape.getRadius()*2) {  // Касание пола
                next_y = (float)HEIGHT - shape.getRadius()*2;
                direction = VectorDirection::up;
            }
        } else {
            v -= (G + C);

            if (v < 0) {  // Обнуление скорости
                direction = VectorDirection::down;
                v = 0;
            }

            next_y = position.y - v;
        }
        shape.setPosition(position.x, next_y);
        std::cout << "V: " << v << " px/s"  << std::endl;

        window.draw(shape);

        std::this_thread::sleep_for(std::chrono::seconds(1));
        window.display();
    }

    return 0;
}