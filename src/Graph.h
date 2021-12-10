#pragma once

#include <iostream>
#include <vector>
#include <queue>  
#include <list>
#include <map> 
#include <iterator>
#include <SFML/Graphics.hpp>
#include <unordered_map>
// csv file for data

using namespace std;

class Vertex
{
    //int colorState; // 0  unvisited, 1 visited drawing, 4 wall
    int xCor; //upper left corner 
    int yCor;

public:

    int vID; //going to need a vertex ID for each Vertex
    Vertex(int x, int y, int id);
    Vertex(int id);
    Vertex();
    bool operator==(const Vertex& other) const;
    bool operator<(const Vertex& other) const;
    int getX() const;
    int getY() const;
};


class Graph
{
    Vertex entry;
    Vertex exit;
    bool removeEdgeHelper(Vertex& from, Vertex& to);

public:

    map<Vertex, vector<pair<Vertex, int>>> adjList;
    unordered_map<int, sf::Sprite> g1Sprites;
    vector<int> colorStates; // 0  unvisited, 1 visited drawing 4 wall , 3 QueueSmall

    Graph();
    Vertex getEntry();
    Vertex getExit();
    void insertEdge(Vertex& from, Vertex& to, int wt);
    bool isEdge(Vertex from, Vertex to);
    vector<Vertex> getAdjacent(Vertex v); //returns a vector of all adjacent vertices. can change return value to any structure if needed
    int numVertices(); //returns the number of vertices in the graph
    void createPath(string side);
    void setColorState(int vID, string state);
    int getColorState(int vID);
    int getAllVertexSize();
    void setTexture(int vID, string texture);
    void setPosition(sf::Sprite& s, float x, float y);
    void drawGraph(sf::RenderWindow& window);
    //ALGORITHMS
    vector<int> dijkstras(Vertex start); //return vector of ordered vertices visited

    vector<int> bfs(Vertex source);
    vector<int> dfs(Vertex source);

    //will be used to draw both of them
    void draw(vector<int> path, Vertex start, Vertex end); //draw vector on graph
    void changeWeight(Vertex from, Vertex to, int val);

    bool edgeConnectsToWall(Vertex from, Vertex to);
    double dijTime;
    double dfsTime;
    double bfsTime;
    int dijSize;
    int dfsSize;
    int bfsSize;
    void drawDFS(vector<int> path);
    //void drawBFS(vector<int>& path, int& from, int& to);
};

