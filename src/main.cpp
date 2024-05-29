#include <iostream>
#include <utility>
#include <vector>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>


enum class VectorDirection {up, down};

class Shape {
public:
    sf::CircleShape draw_shape;
    float C;  // Коэффициент сопративления воздуху
    VectorDirection direction = VectorDirection::down;
    float v = 0;

    Shape(sf::CircleShape draw_shape, sf::Color color, float C) {
        this->draw_shape = std::move(draw_shape);
        this->draw_shape.setFillColor(color);
        this->C = C;
    }
};



float SHAPES_RADIUS = 30;
int WIGHT = 800;
int HEIGHT = 800;

float G = 9.8f;  // px на sec
int dt = 1;  // Изменение времени в сек.


int main() {
    sf::RenderWindow window(sf::VideoMode(WIGHT, HEIGHT), "Tost");

    std::vector<Shape> shapes {
        Shape(sf::CircleShape(SHAPES_RADIUS), sf::Color::Red,  0.1),
        Shape(sf::CircleShape(SHAPES_RADIUS), sf::Color::Green, 0.06),
        Shape(sf::CircleShape(SHAPES_RADIUS), sf::Color::Blue, 0.01),
    };

    float total_width = (float)WIGHT - SHAPES_RADIUS * 2;
    float distance_between_shapes = total_width / ((float)shapes.size() - 1);

    float current_x = SHAPES_RADIUS;
    float current_y = 10;
    for (auto& item : shapes) {
        item.draw_shape.setPosition(current_x - SHAPES_RADIUS, current_y);
        current_x += distance_between_shapes;
    }

    for (auto& item: shapes) {
        window.draw(item.draw_shape);
    }

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

        for (auto& item: shapes) {
            sf::Vector2f position = item.draw_shape.getPosition();
            float a;
            float next_y;
            if (item.direction == VectorDirection::down) {
                a = G;
                item.v += a*(float)dt - item.C * item.v * (float)dt;
                next_y = position.y+item.v;

                if (next_y > (float)HEIGHT - item.draw_shape.getRadius()*2) {
                    next_y = (float)HEIGHT - item.draw_shape.getRadius()*2;
                    item.direction = VectorDirection::up;
                }
            } else {
                a = -G;
                item.v = std::max(item.v + a*(float)dt - item.C * item.v * (float)dt, 0.0f);

                if (item.v == 0.0f) {
                    item.direction = VectorDirection::down;
                }

                next_y = position.y-item.v;
            }
            std::cout << item.v << " px/s for " << item.draw_shape.getFillColor().toInteger() << std::endl;

            item.draw_shape.setPosition(position.x, next_y);
        }

        for (const auto& item: shapes) {
            window.draw(item.draw_shape);
        }

        std::this_thread::sleep_for(std::chrono::seconds(dt));
        window.display();
    }

    return 0;
}