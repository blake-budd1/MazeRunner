#include "Graph.h"
#include <cmath>
#include <stack>
#include "Random.h"
#include <algorithm>
#include <thread>
#include <set>
#include "TextureManager.h"
#include <queue>
#include "SimpleTimer.h"
#include <ctime>
//for getting the time

#include <chrono> //for the timing
using namespace chrono;


//Vertex class
Vertex::Vertex(int x, int y, int id)
{
	xCor = x;
	yCor = y;
	vID = id;
}

Vertex::Vertex()
{
	xCor = -1;
	yCor = -1;
	vID = -1;
}

Vertex::Vertex(int id)
{
    xCor = -1;
    yCor = -1;
    vID = id;
}

bool Vertex::operator==(const Vertex& other) const
{

	if (this->vID == other.vID)
		return true;
	return false;
}

bool Vertex::operator<(const Vertex& other) const
{
	if (this->vID < other.vID)
		return true;
	return false;
}

int Vertex::getX() const
{
	return this->xCor;
}

int Vertex::getY() const
{
	return this->yCor;
}


//Graph class
//private methods

bool Graph::removeEdgeHelper(Vertex& from, Vertex& to)
{
	auto vIter = adjList.find(from);
	if (vIter == adjList.end())
		return false;
	auto vAdjIter = vIter->second.begin();  //iterate over "from's" adjacent vertices
	while (vAdjIter != vIter->second.end())
	{
		if (vAdjIter->first == to)  // the vertex "to" is found
		{
			vIter->second.erase(vAdjIter);  //erase "to" from "from's" adjacent vertices
			return true;
		}
		vAdjIter++;
	}
	return false;
}

Graph::Graph()
{

}

Vertex Graph::getEntry()
{
    return this->entry;
}

Vertex Graph::getExit()
{
    return this->exit;
}

void Graph::insertEdge(Vertex& from, Vertex& to, int wt)
{
	//inserts edges in both directions
	if (!isEdge(from, to))
	{
		adjList[from].push_back(make_pair(to, wt));
	}
		
	if (!isEdge(to, from))
	{
		adjList[to].push_back(make_pair(from, wt));
	}
}

bool Graph::isEdge(Vertex from, Vertex to)
{
	auto vFrom = adjList.find(from);

	//check if the vertex from is in graph
	if (vFrom == adjList.end())
		return false;

	//vertex from is in the graph so check if to is adjacent
	auto vAdjIter = vFrom->second.begin();
	while (vAdjIter != vFrom->second.end())
	{
		if (vAdjIter->first == to)
			return true;
		vAdjIter++;
	}
	return false;
}

vector<Vertex> Graph::getAdjacent(Vertex v)
{
	auto vIter = adjList.find(v);
	if (vIter == adjList.end())
		return {}; //return empty vector if vertex is not in graph
	vector<Vertex> adj;

	//declare iterator to beginning of v's adjacent vertices
	auto adjIter = vIter->second.begin();
	while (adjIter != vIter->second.end())
	{
		adj.push_back(adjIter->first); //push all adjacent vertices to vector
		adjIter++;
	}
	return adj;
}


int Graph::numVertices()
{
	return adjList.size();
}

void Graph::createPath(string side)
{
	//pick random cell on left size as entry point
	int vIdOfStart, vIdOfEnd;
    int n = sqrt(numVertices());
    int randStart = Random::Int(0, n - 1);
    int randEnd = Random::Int(0, n - 1);

	if (side == "left")
    {
        vIdOfStart = n * randStart;
        vIdOfEnd = (randEnd * n) + n - 1;  //check this but should be right
    }

	if (side == "top")
    {
	    vIdOfStart = randEnd;
	    vIdOfEnd = numVertices() - randEnd;
    }

	this->exit = adjList.find(Vertex(vIdOfEnd))->first;

	auto iter = adjList.find(Vertex(vIdOfStart));
	setColorState(iter->first.vID, "QueueSmall");
	this->entry = iter->first; //set map entry to the randomly found vertex on left border

	stack<int> s;
	set<int> visited;
	s.push(this->entry.vID);
	visited.insert(this->entry.vID);

	while (!s.empty())
    {
	    int v = s.top();
	    s.pop();

	    vector<Vertex> adjacentV = getAdjacent(Vertex(v));
	    while (!adjacentV.empty())
        {
	        int randIndex = Random::Int(0,adjacentV.size() - 1);

	        //set color of vertex at random index to unvisited
	        auto iter = adjList.find(adjacentV[randIndex]);

	        if (iter->first == this->exit)
            {
	            setColorState(iter->first.vID, "VisitedSmall");
	            return;
            }

	        if (visited.find(iter->first.vID) == visited.end())  //this vertex has not been visited yet
            {
                changeWeight(Vertex(v),iter->first,1);
                setColorState(v, "UnvisitedSmall");
                visited.insert(iter->first.vID);
                s.push(iter->first.vID);
            }
	        adjacentV.erase(adjacentV.begin() + randIndex);
        }
    }
}

vector<int> Graph::bfs(Vertex source)
{
    SimpleTimer timer;

    queue<int> q;
    set<int> visited;
    q.push(this->entry.vID);
    visited.insert(this->entry.vID);

    vector<int> traversalOrder;
    traversalOrder.push_back(source.vID);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        auto iter = adjList[Vertex(u)].begin();
        while (iter != adjList[Vertex(u)].end()) //go through adjacent vertices
        {
            if (iter->second > 1) //this is an edge to a wall, so dont go here
            {
                iter++;
                continue;
            }

            else if (!visited.count(iter->first.vID))
            {
                traversalOrder.push_back(iter->first.vID);
                if (Vertex(u) == this->exit)
                    return traversalOrder;
                visited.insert(iter->first.vID);
                q.push(iter->first.vID);
            }
            iter++;
        }
    }

    double bfsTestTime = timer.stopTimer();
    bfsTime = bfsTestTime;
    return traversalOrder;
}

vector<int> Graph::dfs(Vertex source)
{
    SimpleTimer timer;

    stack<int> s;
    set<int> visited;
    s.push(this->entry.vID);
    visited.insert(this->entry.vID);

    vector<int> traversalOrder;
    traversalOrder.push_back(source.vID);
    while (!s.empty())
    {
        int u = s.top();
        s.pop();

        auto iter = adjList[Vertex(u)].begin();
        while (iter != adjList[Vertex(u)].end()) //go through adjacent vertices
        {
            if (iter->second > 1) //this is an edge to a wall, so dont go here
            {
                iter++;
                continue;
            }

            else if (visited.count(iter->first.vID) == 0)
            {
                  traversalOrder.push_back(iter->first.vID);
                if (Vertex(u) == this->exit)
                    return traversalOrder;
                visited.insert(iter->first.vID);
                s.push(iter->first.vID);
            }
            iter++;
        }
    }

    double dfsTestTime = timer.stopTimer();
    dfsTime = dfsTestTime;
    return traversalOrder;

}

void Graph::drawDFS(vector<int> path) //change the state of the path of dfs in order, also used for bfs
{
    for(int i = 0; i < path.size()-1; i++)
    {
        setColorState(path[i], "QueueSmall");
    }
}

void Graph::setColorState(int vID, string state)
{
    //colorStates[vID] = state;
    setTexture(vID, state);
    if (state == "UnvisitedSmall")
        colorStates[vID] = 0;
    if (state == "VisitedSmall")
        colorStates[vID] = 1;
    if (state == "QueueSmall")
        colorStates[vID] = 3;
    if (state == "WallSmall")
        colorStates[vID] = 4;
}

int Graph::getColorState(int vID)
{
    return colorStates[vID];
}

void Graph::setTexture(int vID, string texture)
{
    g1Sprites[vID].setTexture(TextureManager::GetTexture(texture));
}

void Graph::setPosition(sf::Sprite &s, float x, float y)
{
    s.setPosition(x, y);
}

void Graph::drawGraph(sf::RenderWindow &window)
{
    auto iter = g1Sprites.begin();
    while (iter != g1Sprites.end())
    {
        window.draw(iter->second);
        iter++;
    }
}
 
vector<int> Graph::dijkstras(Vertex source)
{
    SimpleTimer timer;


	int src = source.vID; // get the id of the source
    int num = numVertices();
    auto time  = 0;
    vector<int> dist(num, INT_MAX);
    vector<int> p(num, -1);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, src));
    dist[src] = 0;
    p[src] = src;
    auto start = std::chrono::high_resolution_clock::now(); //start the clock
    while (!pq.empty())
    {
        pair<int, int> curr = pq.top();
        pq.pop();
        int temp = curr.second;

        for (auto it = adjList[Vertex(temp)].begin(); it != adjList[Vertex(temp)].end(); it++)
        {
            int x = it->first.vID;
            int y = it->second; //adj[temp].second = weight

            if (dist[x] > dist[temp] + y) {
                p[x] = temp;
                dist[x] = dist[temp] + y;
                pq.push(make_pair(dist[x], x));

            }

        }
    }


    double dijTestTime = timer.stopTimer();

    dijTime = dijTestTime;

    return p;
}

void Graph::draw(vector<int> path, Vertex start, Vertex end) //path is the vector of predecessor 
{

	//change to vertex
	vector<Vertex> predVertex; //vector of predecessor vertex objects
    vector<int> finalPathInt(path.size()); //vector of the path of the ID color states
    vector<Vertex> finalPath; //vector of the path of the actual vertex
    //----------------------------------------------------------------//
    //translates the vector of ids to a vector of Vertex
    //---------------------------------------------------------------//
    //create a vector of the path with int
    int i = 0;
    int currID = end.vID;
    finalPathInt[i] = currID;
    i++;
    while(currID != start.vID)
    {

        //----- NEED TO TRY THIS WAY
         //currID = path[i];
         int temp = path[currID];
         currID = temp;
        finalPathInt[i] = currID; //don't know if this will work yet
        i++;
    }

    finalPathInt[i] = currID; //now the vector ends at the start

    reverse(finalPathInt.begin(), finalPathInt.end());
    dijSize = finalPathInt.size();
    //set the color ID's of each vertex in the path:
    //go backwards so it starts correctly
    for(int i = 0; i < finalPathInt.size()-1 ;i++ )
    {
        setColorState(finalPathInt[i], "QueueSmall");
    }
}

void Graph::changeWeight(Vertex from, Vertex to, int val)
{
    auto iter = adjList.find(from);
    auto vAdjIter = iter->second.begin();
    while (vAdjIter != iter->second.end())
    {
        if (vAdjIter->first == to)
        {
            vAdjIter->second = val;
            return;
        }
        vAdjIter++;
    }
}

