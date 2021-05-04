#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <SFML/Graphics.hpp>

#define NUM_OF_RACKETS 2
#define RACKET_LENGTH 150.f
#define RACKET_WIDTH 20.f
#define BALL_RADIUS 15.f

void newRound(sf::RectangleShape *r, sf::CircleShape *b, sf::Vector2f *velocity, sf::Text *t, int *score);
int checkCollision(sf::RectangleShape *r, sf::CircleShape *b, sf::Vector2f *velocity, int *score);
void moveBall(sf::CircleShape *b, sf::Vector2f velocity);
void movePlayer(sf::RectangleShape *racket, int dir);
void moveNPC(sf::RectangleShape *r, sf::CircleShape *c);
int gameMenu(sf::RenderWindow *w, int *score);

int main()
{
    clock_t t;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");
    int playerDir;
    int score[2];
    sf::Vector2f ballVelocity;

    sf::RectangleShape racket[NUM_OF_RACKETS];
    for (int i = 0; i < NUM_OF_RACKETS; i++)
    {
        racket[i] = sf::RectangleShape(sf::Vector2f(RACKET_WIDTH, RACKET_LENGTH));
    }
    sf::CircleShape ball(BALL_RADIUS);

    sf::Font font;
    if (!font.loadFromFile("calibri.ttf"))
    {
        // error...
    }
    sf::Text scoreText[2];
    for (int i = 0; i < NUM_OF_RACKETS; i++)
    {
        scoreText[i].setFont(font);
        score[i] = 0;
    }
    scoreText[0].setPosition(300.f, 20.f);
    scoreText[1].setPosition(500.f, 20.f);

    sf::Text gameText;
    sf::FloatRect gameTextBounds;

    newRound(racket, &ball, &ballVelocity, scoreText, score);

    t = clock();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                playerDir = -1;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                playerDir = 1;
            else
                playerDir = 0;
        }

        if((float)((clock() - t))/CLOCKS_PER_SEC > 0.02f)
        {
            if(checkCollision(racket, &ball, &ballVelocity, score) != 0) 
            {
                newRound(racket, &ball, &ballVelocity, scoreText, score);
            }
            moveBall(&ball, ballVelocity);
            movePlayer(racket, playerDir);
            moveNPC(racket, &ball);
            window.clear();
            for (int i = 0; i < NUM_OF_RACKETS; i++)
            {
                window.draw(racket[i]);
                window.draw(scoreText[i]);
            }
            window.draw(ball);
            if(score[0] == 10)
            {
                // PLAYER WINS
                gameText.setFont(font);
                gameText.setCharacterSize(48);
                gameText.setString("VICTORY");
                gameTextBounds = gameText.getGlobalBounds();
                gameText.setPosition(400.f-gameTextBounds.width/2, 200.f-gameTextBounds.height/2);
                window.draw(gameText);
                window.display();
                if(gameMenu(&window, score) == 0)
                    break;
                else
                    newRound(racket, &ball, &ballVelocity, scoreText, score);
            }
            else if(score[1] == 10)
            {
                // NPC WINS
                gameText.setFont(font);
                gameText.setCharacterSize(48);
                gameText.setString("GAME OVER");
                gameTextBounds = gameText.getGlobalBounds();
                gameText.setPosition(400.f-gameTextBounds.width/2, 200.f-gameTextBounds.height/2);
                window.draw(gameText);
                window.display();
                if(gameMenu(&window, score) == 0)
                    break;
                else
                    newRound(racket, &ball, &ballVelocity, scoreText, score);
            }
            else
                window.display();
            
            t = clock();
        }
    }

    return 0;
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

int gameMenu(sf::RenderWindow *w, int *score)
{
    while(1)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            for(int i = 0; i < NUM_OF_RACKETS; i++)
            {
                score[i] = 0;
            }
            return 1;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            return 0;
        }
    }
}