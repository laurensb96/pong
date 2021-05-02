#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <linux/input.h>

#define NUM_OF_BARS 2
#define BAR_LENGTH 200.f
#define BAR_WIDTH 20.f

void updateWindow(Bars *b);

class Bars {
    public:
        Bars();
        void setPosition(float, float);
        sf::RectangleShape get();
    private:
        sf::RectangleShape rect;
        float pos_x, pos_y;
};

Bars::Bars ()
{
    rect = sf::RectangleShape(sf::Vector2f(BAR_LENGTH, BAR_WIDTH));
    rect.rotate(90.f);
}

void Bars::setPosition(float x, float y)
{
    this->pos_x = x+BAR_WIDTH;
    this->pos_y = y;
    rect.setPosition(this->pos_x, this->pos_y);
}

sf::RectangleShape Bars::get()
{
    return this->rect;
}

int main()
{
    clock_t t;
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pong");

    Bars pongBars[2];
    pongBars[0].setPosition(0.f, 360.f-BAR_LENGTH/2);
    pongBars[1].setPosition(1280.f-BAR_WIDTH, 360.f-BAR_LENGTH/2);

    t = clock();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if((float)((clock() - t))/CLOCKS_PER_SEC > 0.02f)
        {
            updateWindow(pongBars);
            t = clock();
        }
        window.clear();
        for (int i = 0; i < NUM_OF_BARS; i++)
        {
            window.draw(pongBars[i].get());
        }
        window.display();
    }

    return 0;
}

void updateWindow(Bars *b)
{
    b[0].setPosition(b[0])
}