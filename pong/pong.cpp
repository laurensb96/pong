#include "pong.hpp"

Button::Button(sf::String text, sf::Font *font)
{
    this->button = sf::RectangleShape(sf::Vector2f(200.f, 100.f));
    this->text.setFont(*font);
    this->text.setString(text);
    this->text.setCharacterSize(32);
    this->text.setFillColor(sf::Color(0, 0, 0));
}

void Button::draw(sf::RenderWindow *w)
{
    w->draw(this->button);
    w->draw(this->text);
}

void Button::setPosition(sf::Vector2f pos)
{
    sf::FloatRect buttonBounds, textBounds;
    float textX, textY;
    this->button.setPosition(pos);
    buttonBounds = this->button.getGlobalBounds();
    textBounds = this->text.getGlobalBounds();
    textX = buttonBounds.left + buttonBounds.width/2 - textBounds.width/2;
    textY = buttonBounds.top + buttonBounds.height/2 - 20.f;
    this->text.setPosition(textX, textY);
}

bool Button::clicked(sf::Vector2i mousePosition)
{
    sf::IntRect buttonBounds;
    buttonBounds = (sf::IntRect) this->button.getGlobalBounds();
    if(buttonBounds.contains(mousePosition)) return true;
    else return false;
}

void newRound(sf::RectangleShape *r, sf::CircleShape *b, sf::Vector2f *velocity, sf::Text *t, int *score)
{
    r[0].setPosition(sf::Vector2f(15.f, 300.f-RACKET_LENGTH/2));
    r[1].setPosition(sf::Vector2f(785.f-RACKET_WIDTH, 300.f-RACKET_LENGTH/2));
    b->setPosition(400.f, 500.f);
    *velocity = sf::Vector2f(8.f, (float) ((rand() % 1000) / 200 - 2.5f));
    for (int i = 0; i < NUM_OF_RACKETS; i++)
    {
        t[i].setString(std::to_string(score[i]));
    }
}

int checkCollision(sf::RectangleShape *r, sf::CircleShape *b, sf::Vector2f *velocity, int *score)
{
    sf::Vector2f playerPos, npcPos, ballPos;
    playerPos = r[0].getPosition();
    npcPos = r[1].getPosition(); 
    ballPos = b->getPosition();

    if(ballPos.x <= (playerPos.x + RACKET_WIDTH))
    {
        if((ballPos.y + 2*BALL_RADIUS) >= playerPos.y && ballPos.y <= (playerPos.y + RACKET_LENGTH))
        {
            *velocity = sf::Vector2f(8.f, ((ballPos.y + BALL_RADIUS) - (playerPos.y + RACKET_LENGTH/2))/10.f);
        }
        else
        {
            score[1]++;
            return 1;
        }
    }
    else if((ballPos.x + 2*BALL_RADIUS) >= npcPos.x)
    {
        if((ballPos.y + 2*BALL_RADIUS) >= npcPos.y && ballPos.y <= (npcPos.y + RACKET_LENGTH))
        {
            *velocity = sf::Vector2f(-8.f, ((ballPos.y + BALL_RADIUS) - (npcPos.y + RACKET_LENGTH/2))/10.f);
        }
        else
        {
            score[0]++;
            return 1;
        }
    }

    if(ballPos.y <= 0.f || (ballPos.y + 2*BALL_RADIUS) >= 600.f)
    {
        velocity->y = -velocity->y;
    }

    return 0;
}

void moveBall(sf::CircleShape *b, sf::Vector2f velocity)
{
    b->move(velocity);
}

void movePlayer(sf::RectangleShape *r, int dir)
{
    r[0].move(0.f, 8.f*dir);
}

void moveNPC(sf::RectangleShape *r, sf::CircleShape *b)
{
    sf::Vector2f npcPos, ballPos;
    npcPos = r[1].getPosition() + sf::Vector2f(RACKET_WIDTH/2, RACKET_LENGTH/2); 
    ballPos = b->getPosition() + sf::Vector2f(BALL_RADIUS, BALL_RADIUS);

    if(npcPos.y > ballPos.y) r[1].move(0.f, -5.f);
    else if(npcPos.y < ballPos.y) r[1].move(0.f, 5.f);
    else r[1].move(0.f, 0.f);
}

void gameMenu(sf::Text *t, sf::RenderWindow *w, sf::Event *e, int *score)
{
    sf::FloatRect bounds;
    bounds = t->getGlobalBounds();
    t->setPosition(400.f-bounds.width/2, 200.f-bounds.height/2);
}