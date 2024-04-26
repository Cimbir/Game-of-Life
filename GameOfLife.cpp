#include <iostream>
#include <vector>
#include <queue>
#include <conio.h>
#include <Windows.h>

#define N 20
#define point pair<int,int>

using namespace std;

vector<int> ofs = {1,0,-1,-1,1,-1,0,1,1};

bool inBounds(int i, int j){
    return min(i,j) >= 0 && max(i,j) < N; 
}
struct GridQueue{
    queue<point> q;
    vector<vector<bool>> inQueue;

    GridQueue(){
        inQueue = vector<vector<bool>>(N, vector<bool>(N, false));
    }

    point dequeue(){
        point res = q.front();
        q.pop();
        inQueue[res.first][res.second] = false;
        return res;
    }
    void enqueue(point p, bool state){
        if(inQueue[p.first][p.second]) return;
        int x = p.first;
        int y = p.second;
        inQueue[x][y] = true;
        q.push(p);

        if(!state) return;
        for(int i = 0; i < 8; i++){
            int nx = x + ofs[i];
            int ny = y + ofs[i+1];
            if(!inBounds(nx,ny) || inQueue[nx][ny]) continue;
            inQueue[nx][ny] = true;
            q.push({nx, ny});
        }
    }
};

void drawGrid(vector<vector<bool>>& grid){
    system("cls");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cout << (grid[i][j] ? 'X' : '.');
        }
        cout << endl;
    }
}
void addPointList(vector<point>& points, vector<vector<bool>>& grid, GridQueue& gq){
    for(int i = 0; i < points.size(); i++){
        grid[points[i].first][points[i].second] = true;
        gq.enqueue({ points[i].first, points[i].second }, true);
    }
}
int main(){

    vector<vector<bool>> grid(N, vector<bool>(N, false));
    GridQueue gq;

    vector<point> points = {
        {6,5}, {7,5}, {8,5}, {8,6}, {7,7}, {7,8}, {6,8}, {5,9}, {5,10}, {6,10}, {7,10}, {9,7}, {9,8},
        {8,11}, {9,10}, {9,12}, {10,11}, {11,11}, {11,10}, {11,12}, {11,5}, {11,6}, {12,5}, {12,6},
        {13,6}
    };
    addPointList(points,grid,gq);

    while(true){
        drawGrid(grid);

        int l = gq.q.size();
        vector<vector<bool>> newGrid = grid;
        while(l--){
            point cur = gq.dequeue();
            int i = cur.first;
            int j = cur.second;

            bool isOn = grid[i][j];
            int onAroundAmount = 0;
            for(int k = 0; k < 8; k++){
                int ni = i + ofs[k];
                int nj = j + ofs[k+1];
                if(inBounds(ni,nj)) onAroundAmount += grid[ni][nj];
            }
            //cout << isOn << " " << onAroundAmount << " | ";

            if(isOn && (onAroundAmount < 2 || onAroundAmount > 3)){
                newGrid[i][j] = false;
                if(onAroundAmount > 0) gq.enqueue(cur, false);
            }else if(isOn || (!isOn && onAroundAmount == 3)){
                newGrid[i][j] = true;
                gq.enqueue(cur, true);
            }else if(!isOn && onAroundAmount > 0){
                gq.enqueue(cur, false);
            }
        }
        Sleep(100);
        grid = newGrid;
    }

    return 0;
}