#include "pong.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

    clock_t t;
    int playerDir;
    int score[2];
    sf::Vector2f ballVelocity;
    GameState pongState;
    sf::Vector2i mousePosition;

    pongState = play;

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

    sf::Text menuText;

    Button quitButton("QUIT", &font);
    Button replayButton("PLAY AGAIN", &font);
    quitButton.setPosition(sf::Vector2f(100, 300));
    replayButton.setPosition(sf::Vector2f(500, 300));

    menuText.setFont(font);
    menuText.setCharacterSize(48);

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

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                menuText.setString("PAUSE");
                gameMenu(&menuText, &window, &event, score);
                pongState = pause;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                pongState = play;
            }

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && pongState == pause)
            {
                mousePosition = sf::Mouse::getPosition(window);
                if(quitButton.clicked(mousePosition)) window.close();
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && pongState == pause)
            {
                mousePosition = sf::Mouse::getPosition(window);
                if(replayButton.clicked(mousePosition))
                {
                    for (int i = 0; i < NUM_OF_RACKETS; i++)
                    {
                        score[i] = 0;
                    }
                    newRound(racket, &ball, &ballVelocity, scoreText, score);
                    pongState = play;
                }
            }
        }

        if((float)((clock() - t))/CLOCKS_PER_SEC > 0.02f)
        {
            if (score[0] >= 2 || score[1] >= 2)
            {
                pongState = pause;
                if(score[0] >= 2) menuText.setString("GAME WON");
                else menuText.setString("GAME OVER");
                gameMenu(&menuText, &window, &event, score);
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) break;
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    for (int i = 0; i < NUM_OF_RACKETS; i++)
                    {
                        score[i] = 0;
                    }
                    newRound(racket, &ball, &ballVelocity, scoreText, score);
                    pongState = play;
                }
            }
            if(pongState == play)
            {
                if(checkCollision(racket, &ball, &ballVelocity, score) != 0) 
                {
                    newRound(racket, &ball, &ballVelocity, scoreText, score);
                }
                moveBall(&ball, ballVelocity);
                movePlayer(racket, playerDir);
                moveNPC(racket, &ball);
            }
            window.clear();
            for (int i = 0; i < NUM_OF_RACKETS; i++)
            {
                window.draw(racket[i]);
                window.draw(scoreText[i]);
            }
            window.draw(ball);
            if(pongState == pause)
            {
                window.draw(menuText);
                quitButton.draw(&window);
                replayButton.draw(&window);
            }
            window.display();
            t = clock();
        }
    }

    return 0;
}