#pragma once
#include "Graph.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
using std::unordered_map;

class Maze {

    Graph graph1;

public:

    Maze();
    void buildGraph(Graph& graph, int numVertices, int startX, int startY, int cellSize);
    void buildMaze(int startX, int startY);
    void DisplayMaze(sf::RenderWindow& window);
    void runDij(); //runs the dijkstra algorithm on the graph
    void runDFS(); //runs depth first search on the graph
    void runBFS(); // runs breadth first search on the graph
    void offsetPixels(int x, int y);
    float getStartY();
    float getEndY();
    Graph* getGraph();
};

