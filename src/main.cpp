#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>


enum class VectorDirection {up, down};


float SHAPE_RADIUS = 30;
int WIGHT = 800;
int HEIGHT = 800;

float C = 0.1;  // Коэффициент сопративления воздуху
float G = 9.8f;  // px на sec
int dt = 1;  // Изменение времени в сек.


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
        float a;
        float next_y;
        if (direction == VectorDirection::down) {
            a = G;
            v += a*(float)dt - C*v*(float)dt;
            next_y = position.y+v;

            if (next_y > (float)HEIGHT - shape.getRadius()*2) {
                next_y = (float)HEIGHT - shape.getRadius()*2;
                direction = VectorDirection::up;
            }
        } else {
            a = -G;
            v = std::max(v + a*(float)dt - C*v*(float)dt, 0.0f);

            if (v == 0.0f) {
                direction = VectorDirection::down;
            }

            next_y = position.y-v;
        }
        std::cout << v << " px/s" << std::endl;

        shape.setPosition(position.x, next_y);


        window.draw(shape);

        std::this_thread::sleep_for(std::chrono::seconds(dt));
        window.display();
    }

    return 0;
}