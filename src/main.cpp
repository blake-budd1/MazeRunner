#include <iostream>
#include <SFML/Graphics.hpp>
#include "Graph.h"
#include "Maze.h"
#include <thread>
#include <future> //testing
#include "TextureManager.h"
#include <vector>

int main() {

    Maze m, m2, m3;

    int xOffset = 712;
    int yOffset = 0;

    TextureManager textures;
    textures.LoadTexture("Start");
    textures.LoadTexture("Flag");
    textures.LoadTexture("DIJ");
    textures.LoadTexture("BFS");
    textures.LoadTexture("DFS");
    textures.LoadTexture("StartButton");
    textures.LoadTexture("ResetButton");
    textures.LoadTexture("Swap");

    sf::Sprite Start, Flag, DIJ, Title, Button, Start2, Flag2, Swap;
    Start.setTexture(textures.GetTexture("Start"));
    Flag.setTexture(textures.GetTexture("Flag"));
    Start2.setTexture(textures.GetTexture("Start"));
    Flag2.setTexture(textures.GetTexture("Flag"));
    DIJ.setTexture(textures.GetTexture("DIJ"));
    Title.setTexture(textures.GetTexture("BFS"));
    Swap.setTexture(textures.GetTexture("Swap"));

    Title.setPosition(sf::Vector2f(344.f,0.f));
    Swap.setPosition(sf::Vector2f(344.f,742.f));
    DIJ.setPosition(sf::Vector2f(1056.f,0.f));
    Button.setPosition(sf::Vector2f(600.f, 717.f));
    Button.scale(sf::Vector2f(0.6f,0.6f));
    Start.scale(sf::Vector2f(0.5f,0.5f));
    Flag.scale(sf::Vector2f(0.5f,0.5f));
    Flag2.scale(sf::Vector2f(0.5f,0.5f));
    Start2.scale(sf::Vector2f(0.5f,0.5f));

    sf::Text text1, text2;
    sf::Font font;
    font.loadFromFile("Images/KGRedHands.ttf");
    text1.setPosition(sf::Vector2f(77.f, 684.f));
    text2.setPosition(sf::Vector2f(789.f, 684.f));
    text1.setFont(font);
    text1.setCharacterSize(24);
    text1.setFillColor(sf::Color::White);

    text2.setFont(font);
    text2.setCharacterSize(24);
    text2.setFillColor(sf::Color::White);
    text2.setString("Total time taken: 0.000000ms");

    int buttonState = 1;  // 0 - reset, 1 - start, 2 - idle
    int swapState = 1; // 1 - bfs , 2 - dfs

    m.buildMaze(77, 50);
    m2 = m;
    m3 = m;

    m2.offsetPixels(xOffset , yOffset);

    Start.setPosition(sf::Vector2f(35.f, m.getStartY()-10.f));
    Start2.setPosition(sf::Vector2f(747.f,m2.getStartY()-10.f));
    Flag.setPosition(sf::Vector2f(700.f,m.getEndY()-10.f));
    Flag2.setPosition(sf::Vector2f(1414.f,m2.getEndY()-10.f));
    Button.setTexture(textures.GetTexture("StartButton"));

    sf::RenderWindow window(sf::VideoMode(1500, 900), "Maze Runner");

    vector<int> path;
    bool bfs = true;
    while (window.isOpen())
    {
        sf::Event event;
        bool bfsDone = false;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)  //check if mouse button was pressed
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (Button.getGlobalBounds().contains(mousePos.x, mousePos.y))  //check if start button was pressed
                    {
                        if (buttonState == 1)
                        {
                            window.setActive(false);

                            thread t1(&Maze::runBFS, &m);
                            thread t3(&Maze::runDFS, &m3);
                            thread t2(&Maze::runDij, &m2);
                            t1.join();
                            t2.join();
                            t3.join();

                            text2.setString("Total time taken: " + to_string(m2.getGraph()->dijTime) + "ms");
                            window.setActive(true);

                            Button.setTexture(textures.GetTexture("ResetButton"));
                            buttonState = 0;
                        }
                        else if (buttonState == 0)
                        {
                            m.buildMaze(77, 50);
                            m2 = m;
                            m3 = m;
                            m2.offsetPixels(xOffset , yOffset);

                            Start.setPosition(sf::Vector2f(35.f, m.getStartY()-10.f));
                            Start2.setPosition(sf::Vector2f(747.f,m2.getStartY()-10.f));
                            Flag.setPosition(sf::Vector2f(700.f,m.getEndY()-10.f));
                            Flag2.setPosition(sf::Vector2f(1414.f,m2.getEndY()-10.f));
                            text2.setString("Total time taken: 0.000000ms");
                            buttonState = 1;
                            bfsDone = true;
                            Button.setTexture(textures.GetTexture("StartButton"));
                            path.clear();
                        }
                    }
                    if (Swap.getGlobalBounds().contains(mousePos.x, mousePos.y))  //check if start button was pressed
                    {
                        if(swapState == 1)
                        {
                            swapState = 2;
                            Title.setTexture(textures.GetTexture("DFS"));
                            bfs = false;
                        }
                        else if(swapState == 2)
                        {
                            swapState = 1;
                            Title.setTexture(textures.GetTexture("BFS"));
                            bfs = true;
                        }
                    }
                }
            }
        }
        sf::Color color(66, 66, 66);
        window.clear(color);
        if(bfs)
        {
            window.clear(color);
            m.DisplayMaze(window);
            text1.setString("Total time taken: " + to_string(m.getGraph()->bfsTime) + "ms");
        }
        else
        {
            window.clear(color);
            m3.DisplayMaze(window);
            text1.setString("Total time taken: " + to_string(m3.getGraph()->dfsTime) + "ms");
        }

        window.draw(Title);
        window.draw(DIJ);
        window.draw(Swap);
        window.draw(Button);
        window.draw(Start);
        window.draw(Start2);
        window.draw(Flag);
        window.draw(Flag2);
        window.draw(text1);
        window.draw(text2);

        m2.DisplayMaze(window);
        window.display();
    }




}

