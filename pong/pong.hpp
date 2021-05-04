#ifndef PONG_HPP
#define PONG_HPP

#include <SFML/Graphics.hpp>

#define NUM_OF_RACKETS 2
#define RACKET_LENGTH 150.f
#define RACKET_WIDTH 20.f
#define BALL_RADIUS 15.f

enum GameState
{
    play, pause
};

class Button
{
    public:
        Button(sf::String text, sf::Font *font);
        void setPosition(sf::Vector2f pos);
        void draw(sf::RenderWindow *w);
        bool clicked(sf::Vector2i mousePosition);
    private:
        sf::Vector2f pos;
        sf::RectangleShape button;
        sf::Text text;
};


void newRound(sf::RectangleShape *r, sf::CircleShape *b, sf::Vector2f *velocity, sf::Text *t, int *score);
int checkCollision(sf::RectangleShape *r, sf::CircleShape *b, sf::Vector2f *velocity, int *score);
void moveBall(sf::CircleShape *b, sf::Vector2f velocity);
void movePlayer(sf::RectangleShape *racket, int dir);
void moveNPC(sf::RectangleShape *r, sf::CircleShape *c);
void gameMenu(sf::Text *t, sf::RenderWindow *w, sf::Event *e, int *score);

#endif