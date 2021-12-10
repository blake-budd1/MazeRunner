#include "Maze.h"
#include "Random.h"
#include <cmath>
#include "TextureManager.h"

Maze::Maze()
{

}

void Maze::buildGraph(Graph& graph, int numVertices, int startX, int startY, int cellSize) {

    graph.adjList.clear();
    graph.g1Sprites.clear();

    graph.colorStates = vector<int>(numVertices,4);  //sets all color states to 4 - wall

    int n = sqrt(numVertices);
    vector<vector<Vertex>> maze(n);
    int counter = 0;
    for (int i = 0; i < n; i++)  //iterates over the rows
    {
        for (int j = 0; j < n; j++) //iterates over the columns
        {
            Vertex v(startX + (j * cellSize), startY + (i * cellSize), counter);
            graph.setPosition(graph.g1Sprites[v.vID], v.getX(), v.getY());
            graph.setTexture(v.vID, "VisitedSmall");
            maze[i].push_back(v);
            counter++;
        }
    }

    //2d vector contains all vertices in graph, so insert into graph and set edges accordingly
    for (int i = 0; i < maze.size(); i++)
    {
        for (int j = 0; j < maze[0].size(); j++)
        {
            //check if vertex is on a border of the maze
            if (i == 0)  //in the top row so all vertices will not have a top edge
            {
                if (j == 0)  //top left vertex
                {
                    graph.insertEdge(maze[i][j], maze[i + 1][j], 1000000);
                    graph.insertEdge(maze[i][j], maze[i][j + 1], 1000000);
                }
                else if (j == n - 1)  //top right vertex
                {
                    graph.insertEdge(maze[i][j], maze[i + 1][j], 1000000);
                    graph.insertEdge(maze[i][j], maze[i][j - 1], 1000000);
                }

                //all other vertices with i = 0 will have all edges except a top
                else
                {
                    graph.insertEdge(maze[i][j], maze[i][j - 1], 1000000); //insert edge to vertex to the left
                    graph.insertEdge(maze[i][j], maze[i][j + 1], 1000000);  //insert edge to vertex to the right
                    graph.insertEdge(maze[i][j], maze[i + 1][j], 1000000);  //insert edge to vertex directly below
                }
            }

            else if (i == n - 1)  //in the bottom row, so all vertices will not have a bottom edge
            {
                if (j == 0)  //bottom left vertex
                {
                    graph.insertEdge(maze[i][j], maze[i - 1][j], 1000000); //edge to vertex above
                    graph.insertEdge(maze[i][j], maze[i][j + 1], 1000000); //edge to vertex to the right
                }
                else if (j == n - 1)  //bottom right vertex
                {
                    graph.insertEdge(maze[i][j], maze[i - 1][j], 1000000);  //edge to vertex above
                    graph.insertEdge(maze[i][j], maze[i][j - 1], 1000000);  //edge to vertex to left
                }
                else
                {
                    //all other vertices with i = 0 will have all edges except a bottom
                    graph.insertEdge(maze[i][j], maze[i][j - 1], 1000000); //insert edge to vertex to the left
                    graph.insertEdge(maze[i][j], maze[i][j + 1], 1000000);  //insert edge to vertex to the right
                    graph.insertEdge(maze[i][j], maze[i - 1][j], 1000000);  //insert edge to vertex directly above
                }
            }

            else if (j == 0) //in the first column, so all vertices will not have a left edge
            {
                //shouldnt have to worry about first and last row as its taken care of above
                graph.insertEdge(maze[i][j], maze[i - 1][j], 1000000);  //top edge
                graph.insertEdge(maze[i][j], maze[i][j + 1], 1000000);  //right edge
                graph.insertEdge(maze[i][j], maze[i + 1][j], 1000000);  //bottom edge

            }
            else if (j == n - 1)  //in the last column, so all vertices will not have a right edge
            {
                graph.insertEdge(maze[i][j], maze[i - 1][j], 1000000);  //top edge
                graph.insertEdge(maze[i][j], maze[i + 1][j], 1000000);  //bottom edge
                graph.insertEdge(maze[i][j], maze[i][j - 1], 1000000); //left edge
            }

            else  //in this case, all vertices will have 4 edges
            {
                graph.insertEdge(maze[i][j], maze[i - 1][j], 1000000);  //top edge
                graph.insertEdge(maze[i][j], maze[i + 1][j], 1000000);  //bottom edge
                graph.insertEdge(maze[i][j], maze[i][j - 1], 1000000); //left edge
                graph.insertEdge(maze[i][j], maze[i][j + 1], 1000000);  //right edge
            }
        }
    }
}

void Maze::buildMaze(int startX, int startY)
{
    //100489
    buildGraph(graph1, 100489, startX, startY, 2);
    graph1.createPath("left");
    graph1.createPath("left");
}

void Maze::runDij()
{
    vector<int> dij = graph1.dijkstras(graph1.getEntry());
    graph1.draw(dij, graph1.getEntry(), graph1.getExit());
}

 void Maze::runDFS()
{
    vector<int> dfs = graph1.dfs(graph1.getEntry());
    graph1.drawDFS(dfs);
}

void Maze::runBFS()
{
    vector<int> bfs = graph1.bfs(graph1.getEntry());
    graph1.drawDFS(bfs);
}

void Maze::DisplayMaze(sf::RenderWindow &window)
{
    graph1.drawGraph(window);
}


void Maze::offsetPixels(int x, int y)
{
    auto vIter = graph1.adjList.begin();
    while (vIter != graph1.adjList.end())
    {
        graph1.setPosition(graph1.g1Sprites[vIter->first.vID], vIter->first.getX() + x, vIter->first.getY() + y);
        vIter++;
    }
}

float Maze::getStartY()
{
    return graph1.getEntry().getY();
}

float Maze::getEndY()
{
    return graph1.getExit().getY();
}

Graph* Maze::getGraph()
{
    return &graph1;
}