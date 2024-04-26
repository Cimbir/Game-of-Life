// g++ -c .\GOL.cpp -ID:\Workspace\SFML-2.6.1\include
// g++ .\GOL.o -o GOL -LD:\Workspace\SFML-2.6.1\lib -lsfml-graphics -lsfml-window -lsfml-system

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <queue>
#include <set>

using namespace std;
using namespace sf;

int width = 1000;
int height = 1000;
int fps = 60;

int randomGenAmount = 10000;
vector<int> ofs = {1,0,-1,-1,1,-1,0,1,1};

int cellSize = 10;

struct Cell{
    int x;
    int y;
    Cell(int nx, int ny){ x = nx, y = ny; }
    bool operator<(const Cell &a) const{
        if(y != a.y) return y < a.y;
        return x < a.x;
    }
    bool operator==(const Cell &a) const{
        return x == a.x && y == a.y;
    }
}typedef Cell;
struct GridQueue{
    queue<Cell> q;
    set<Cell> inQueue;

    GridQueue(){}

    Cell dequeue(){
        Cell res = q.front();
        q.pop();
        inQueue.erase(res);
        return res;
    }
    void enqueue(Cell c, bool state){
        if(inQueue.count(c)) return;
        inQueue.insert(c);
        q.push(c);

        if(!state) return;
        for(int i = 0; i < 8; i++){
            Cell next(c.x + ofs[i], c.y + ofs[i+1]);
            if(inQueue.count(next)) continue;
            inQueue.insert(next);
            q.push(next);
        }
    }
}typedef GridQueue;

void addInitialPoints(vector<int>& points, set<Cell>& grid, GridQueue& gq){
    for(int i = 0; i < points.size(); i+=2) {
        Cell cur(points[i], points[i+1]);
        gq.enqueue(cur, true);
        grid.insert(cur);
    }
}
void randomGeneration(set<Cell>& grid, GridQueue& gq){
    for(int i = 0; i < randomGenAmount; i++){
        Cell cur((rand()%(width/cellSize)) - (width/cellSize)/2, (rand()%(height/cellSize)) - (height/cellSize)/2);
        gq.enqueue(cur, true);
        grid.insert(cur);
    }
}
void displaySingleCell(Cell& c, RenderWindow& window){
    RectangleShape cRect;
    cRect.setSize(Vector2f(cellSize,cellSize));
    cRect.setPosition(width/2 + c.x * cellSize, height/2 + c.y * cellSize);
    //cout << cRect.getPosition().x << " " << cRect.getPosition().y << endl;
    window.draw(cRect);
}
void displayGrid(set<Cell>& grid, RenderWindow& window){
    for(Cell c : grid){
        //cout << c.x << " " << c.y << endl;
        displaySingleCell(c, window);
    }
}
int main(){
    srand((int)time(0));

    set<Cell> grid;
    GridQueue gq;

    vector<int> initialPoints = {
        6,5, 7,5, 8,5, 8,6, 7,7, 
        7,8, 6,8, 5,9, 5,10, 6,10, 
        7,10, 9,7, 9,8, 8,11, 9,10, 
        9,12, 10,11, 11,11, 11,10, 11,12, 
        11,5, 11,6, 12,5, 12,6, 13,6
    };
    //addInitialPoints(initialPoints, grid, gq);
    randomGeneration(grid, gq);

    // create the window
    RenderWindow window(VideoMode(width, height), "My window");
    window.setFramerateLimit(fps);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(Color::Black);

        // draw everything here...
        // window.draw(...);

        displayGrid(grid, window);

        int l = gq.q.size();
        set<Cell> newGrid;
        while(l--){
            Cell cur = gq.dequeue();

            bool isOn = grid.count(cur);
            int onAroundAmount = 0;
            for(int i = 0; i < 8; i++){
                onAroundAmount += grid.count(Cell(cur.x+ofs[i], cur.y+ofs[i+1]));
            }

            if(isOn && (onAroundAmount < 2 || onAroundAmount > 3)){
                newGrid.erase(cur);
                if(onAroundAmount > 0) gq.enqueue(cur, false);
            }else if(isOn || (!isOn && onAroundAmount == 3)){
                newGrid.insert(cur);
                gq.enqueue(cur, true);
            }else if(!isOn && onAroundAmount > 0){
                gq.enqueue(cur, false);
            }
        }
        grid.clear();
        grid = newGrid;

        // end the current frame
        window.display();
    }

    return 0;
}