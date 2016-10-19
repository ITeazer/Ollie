// Wsl_F@ITeazer

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <list>
#include <queue>
#include <deque>
#include <bitset>
#include <numeric>
#include <cassert>
#include <time.h>
#include <ctime>
#include <memory.h>
#include <complex>
#include <utility>
#include <climits>
#include <cctype>


using namespace std;
#pragma comment(linker, "/STACK:1024000000,1024000000")


typedef long long LL;
typedef unsigned long long uLL;
typedef double dbl;
typedef vector<int> vi;
typedef vector<LL> vL;
typedef vector<string> vs;
typedef pair<int, int> pii;
typedef pair<LL, LL> pLL;

#define mp(x,y)  make_pair((x),(y))
#define pb(x)  push_back(x)
#define sqr(x) ((x)*(x))

// maximum size (height & width) of maze
const int MaxN = 500;
// number of lines in current maze
int linesNum = 0;
// number of rows in current maze
int rowsNum = 0;

// current maze. Do NOT modify it!!!
// 1    - road
// 2    - droid start position
// -2   - finish
// -1    - non road
int mazeInput[MaxN][MaxN];

// copy of mazeInput for editing
int maze[MaxN][MaxN];
// height of Ollie in cm
int droidH = 8;
// width of Ollie in cm
int droidW = 12;


void read(string inputName)
{
    ifstream in(inputName.c_str());
    string s;
    vector<string> lines;
    int maxL = 0;
    while (getline(in,s))
    {
        maxL = max(maxL, (int) s.length());
        lines.pb(s);
    }
    in.close();

    rowsNum = maxL;
    linesNum = lines.size();

    memset(maze,0, sizeof(maze));

    for (int i = 0; i < linesNum; i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            if (lines[i][j] == '*') maze[i+1][j+1] = 1;
            else if (lines[i][j] == 'd') maze[i+1][j+1] = 2;
            else if (lines[i][j] == 'f') maze[i+1][j+1] = -2;
            else if (lines[i][j] == ' ') maze[i+1][j+1] = -1;

            mazeInput[i+1][j+1] = maze[i+1][j+1];
        }
    }

    cout<<"read successfully!"<<endl;
}


pii findCenter(int num)
{
    for (int i = droidH/2; i <= linesNum; i++)
    {
        for (int j = droidW/2; j <= rowsNum; j++)
        {
            if (maze[i][j] == num)
            {
                bool flag = 1;
                for (int x = -droidW/2; x <= droidW/2; x++)
                    for (int y = -droidH/2; y <= droidH/2; y++)
                        flag &= (maze[i][j] == num);

                if (flag)
                {
                    cout<<"find "<<i<<" "<<j<<endl;
                    return mp(i,j);
                }
            }
        }
    }
    return mp(0,0);
}

vector<pair<char, int> >  uniteAns(vector<pair<char, int> >  tAns)
{
    vector<pair<char, int> > ans;
    tAns.pb(mp('#',0));
    for (int i = 1; i < tAns.size(); i++)
    {
        if (tAns[i].first == tAns[i-1].first) tAns[i].second = tAns[i-1].second + 1;
        else ans.pb(tAns[i-1]);
    }

    return ans;
}

/* returns vector of pairs <dirrction, length (in cm)>
 directions : {N, E, S, W}
*/
vector<pair<char, int> > findWay()
{
    pii p = findCenter(2);
    int droidX = p.first;
    int droidY = p.second;
    cout<<"droid: "<<droidX<<" "<<droidY<<endl;

    p = findCenter(-2);
    int finishX = p.first;
    int finishY = p.second;
    cout<<"finish: "<<finishX<<" "<<finishY<<endl;




    int maxVal = rowsNum*linesNum*2;
    for (int i = 1; i <= linesNum; i++)
        for (int j = 1; j <= rowsNum; j++)
            if (maze[i][j] != -1) maze[i][j] = maxVal;
            else maze[i][j] = -1;

    maze[droidX][droidY] = 0;
    maze[finishX][finishY] = -2;

    queue<pii> q;
    q.push(mp(droidX, droidY));

    while (!q.empty())
    {
        p = q.front();
        q.pop();
        int x = p.first;
        int y = p.second;

        if (maze[x][y] < 0) continue;
        int nextVal = maze[x][y] + 1;

        if (maze[x-1][y] > nextVal) { maze[x-1][y] = nextVal; q.push(mp(x-1,y)); }
        if (maze[x+1][y] > nextVal) { maze[x+1][y] = nextVal; q.push(mp(x+1,y)); }
        if (maze[x][y-1] > nextVal) { maze[x][y-1] = nextVal; q.push(mp(x,y-1)); }
        if (maze[x][y+1] > nextVal) { maze[x][y+1] = nextVal; q.push(mp(x,y+1)); }
    }



    vector<pair<char, int> > way;
    q.push(mp(finishX,finishY));

    int curX = finishX;
    int curY = finishY;
    int val = 1000000000;
    if (maze[curX-1][curY] > 0) val = min(val, maze[curX-1][curY]);
    if (maze[curX+1][curY] > 0) val = min(val, maze[curX+1][curY]);
    if (maze[curX][curY-1] > 0) val = min(val, maze[curX][curY-1]);
    if (maze[curX][curY+1] > 0) val = min(val, maze[curX][curY+1]);

    maze[curX][curY] = val + 1;
    counter = 0;
    while (curX != droidX || curY != droidY)
    {
        cout <<  curX << " " << curY << endl;
        int nextVal = maze[curX][curY] - 1;
        if (maze[curX-1][curY] == nextVal) {curX--; way.pb(mp('S',1)); continue;}
        else if (maze[curX+1][curY] == nextVal) {curX++; way.pb(mp('N',1)); continue;}
        else if (maze[curX][curY-1] == nextVal) {curY--; way.pb(mp('W',1)); continue;}
        else if (maze[curX][curY+1] == nextVal) {curY++; way.pb(mp('E',1)); continue;}

    }

    vector<pair<char, int> > ans;

    for (int i = way.size()-1; i>=0; i--)
        ans.pb(way[i]);

    return uniteAns(ans);
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    read("secret.txt");
    vector<pair<char, int> > way = findWay();

    cout<<"way:"<<endl;
    for (int i = 0; i <  way.size(); i++)
        cout<<way[i].first<<" "<<way[i].second<<endl;


    return 0;
}
