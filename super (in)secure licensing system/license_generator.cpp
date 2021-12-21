#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <time.h>

using namespace std;

#define NO_WALL_RIGHT 1
#define NO_WALL_UP 2
#define NO_WALL_LEFT 4
#define NO_WALL_DOWN 8

int a [21][21];
int v [21][21];

int r [61][61];

int fin[41][41];

int dx [] = {0, -1, 0, 1};
int dy [] = {1,  0, -1, 0};

int Q[65 * 65];

void print_hex_nr(int x)
{
	printf("%x", x);
}

void print_hex_nr_fixed(int x)
{
	printf("%02x", x);
}

__attribute__((always_inline)) void dfs(int startx, int starty)
{
    //iterative dfs, start by pushing start on stack
    //stack<pair<int, int> > Q;
    //Q.push(make_pair(startx, starty));
    v[startx][starty] = 1;

	int sp = 0;
	Q[sp] = (startx << 16) | starty;
	++sp;
	
    //while(!Q.empty())
	while(sp)
    {
        //randomize dfs
        int ord[4] = {0, 1, 2, 3};
        for(int i = 0; i < 8; ++i)
        {
            int s1 = rand() % 4;
            int s2 = rand() % 4;
            int aux = ord[s1];
            ord[s1] = ord[s2];
            ord[s2] = aux;
        }

        //int x = Q.top().first;
        //int y = Q.top().second;
        //Q.pop();
		--sp;
		int x = Q[sp] >> 16;
		int y = Q[sp] & 0xFFFF;
		
        //branching factor too low, randomize a false dead end
        if(rand() % 8 == 0)
            continue;

        for(int i = 0; i < 4; ++i)
        {
            int newx = x + dx[ord[i]];
            int newy = y + dy[ord[i]];

            //check out of bounds
            if(newx < 1 || newy < 1 || newx > 20 || newy > 20)
                continue;

            //check visited
            if(v[newx][newy])
                continue;

            //visit
            v[newx][newy] = 1;
            //a[x][y] |= (1<<ord[i]);
            //a[newx][newy] |= (1<<ord[i + 4 % 4]);
            switch(ord[i])
            {
            case 0:
                a[x][y] |= NO_WALL_RIGHT;
                a[newx][newy] = NO_WALL_LEFT;
                break;
            case 1:
                a[x][y] |= NO_WALL_UP;
                a[newx][newy] = NO_WALL_DOWN;
                break;
            case 2:
                a[x][y] |= NO_WALL_LEFT;
                a[newx][newy] = NO_WALL_RIGHT;
                break;
            case 3:
                a[x][y] |= NO_WALL_DOWN;
                a[newx][newy] = NO_WALL_UP;
                break;
            }

            //push new value in stack
            //Q.push(make_pair(newx, newy));
			Q[sp] = (newx << 16) | newy;
			++sp;
        }
    }
}

__attribute__((always_inline)) int bfs(int startx, int starty, int endx, int endy)
{
	int f = 0, b = 0;
	
	fin[startx][starty] = 2;
	Q[b] = (startx << 16) | starty;
	++b;
	
	bool solved = 0;
	while(b > f)
	{
		int x = Q[f] >> 16;
		int y = Q[f] & 0xFFFF;
		
		++f;
		
		for(int i = 0; i < 4; ++i)
        {
            int newx = x + dx[i];
            int newy = y + dy[i];
			
			if (newx == endx && newy == endy)
				solved = true;
			
            //check out of bounds
            if(newx < 1 || newy < 1 || newx > 40 || newy > 40)
                continue;

            //check visited
            if(fin[newx][newy])
                continue;

            //visit
            fin[newx][newy] = fin[x][y] + 1;
			
			Q[b] = (newx << 16) | newy;
			++b;
		}
	}
	
	if(!solved)
	{
		puts("An error has occured while generating license!\n");
		return 0;
	}
	
	int path_size = fin[endx][endy] - fin[startx][starty];
	//cout<<"Key size: "<<path_size<<"\n";
	int x = endx;
	int y = endy;
	
	//backtrack
	while(x != startx || y != starty)
	{
		//cout<<"X = "<<x<<", Y = "<<y<<" ";
		for(int i = 0; i < 4; ++i)
        {
            int newx = x + dx[i];
            int newy = y + dy[i];
			
			//check out of bounds
            if(newx < 1 || newy < 1 || newx > 40 || newy > 40)
                continue;
			
			if(fin[newx][newy] == fin[x][y] - 1)
			{
				x = newx;
				y = newy;
				//cout<<fin[newx][newy] - 1<<"\n";
				Q[fin[newx][newy] - 1] = i;
				break;
			}
		}
	}
	
	for(int i = path_size + 1; i < path_size + 5; ++i)
		Q[i] = 0;
		
	puts("License code: ");
	for(int i = 1; i <= path_size; i += 4)
		print_hex_nr_fixed(((Q[i] & 3) << 6) | ((Q[i + 1] & 3) << 4) | ((Q[i + 2] & 3) << 2) | (Q[i + 3] & 3));
	puts("\n");
	
	return path_size;
}

int main()
{
    srand(time(0));
    //try to generate a 20x20 maze

    //start at 1,1, end at tbd
    dfs(1, 1);

    //render
    for(int i = 1; i < 61; ++i)
        for(int j = 1; j < 61; ++j)
        {
            r[i][j] = 1;
        }

    // 1 -> 1 2 3; 2 -> 4 5 6; 3 -> 7 8 9

    for(int i = 1; i < 21; ++i)
    {
        for(int j = 1; j < 21; ++j)
        {
            r[i * 3 - 1][j * 3 - 1] = 0;
            if(a[i][j] & NO_WALL_DOWN)
            {
                r[i * 3][j * 3 - 1] = 0;
            }
            if(a[i][j] & NO_WALL_LEFT)
            {
                r[i * 3 - 1][j * 3 - 2] = 0;
            }
            if(a[i][j] & NO_WALL_RIGHT)
            {
                r[i * 3 - 1][j * 3 ] = 0;
            }
            if(a[i][j] & NO_WALL_UP)
            {
                r[i * 3 - 2][j * 3 - 1] = 0;
            }
        }
    }

    int desti = 1;
    int destj = 1;

    for(int i = 1; i < 61; ++i)
    {
        if(i % 3 == 0)
            continue;

        for(int j = 1; j < 61; ++j)
        {
            if(j % 3 == 0)
                continue;

            if(r[i][j])
            {
                //cout<<"\xDB";
                //cout<<"X";
                //img<<"1 ";
                fin[desti][destj] = 1;
                ++destj;
            }
            else
            {
                //cout<<" ";
                //img<<"0 ";
                fin[desti][destj] = 0;
                ++destj;
            }
        }
        //cout<<"\n";
        //img<<"\n";
        ++desti;
        destj = 1;
    }


	int val = 0;
	int buffered_chrs = 0;
	
	puts("License name: ");
    for(int i = 1; i < 41; ++i)
    {
        for(int j = 1; j < 41; ++j)
        {
            if(fin[i][j])
            {
				val = (val << 1) | 1;
				++buffered_chrs;
				
				if(buffered_chrs == 4)
				{
					print_hex_nr(val);
					val = 0;
					buffered_chrs = 0;
				}
            }

            else
            {
				val = val << 1;
				++buffered_chrs;
				
				if(buffered_chrs == 4)
				{
					print_hex_nr(val);
					val = 0;
					buffered_chrs = 0;
				}
            }
        }
    }
	puts("\n");
	
	bfs(2, 2, 40, 40);
	return 0;
}
