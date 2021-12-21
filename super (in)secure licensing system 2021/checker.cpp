#include <iostream>
#include <iomanip>
#include <queue>

using namespace std;


char lic_name[512];
char lic_code[512];

int fin[41][41];

bool IsBitSet(uint8_t num, int bit)
{
    return 1 == ( (num >> bit) & 1);
}

int dx [] = {0, -1, 0, 1};
int dy [] = {1,  0, -1, 0};

int main()
{
	cout<<"License name: ";
	cin>>lic_name;
	cout<<"License code: ";
	cin>>lic_code;
	
	//cout<<"Echo________: "<<lic_name<<"\n";
	
	int p = 0;
	for(int i = 1; i < 41; ++i)
	{
		for(int j = 1; j < 41; ++j)
		{
			int nibble = p / 4;
			int bit = 3 - p % 4;
			
			//cout<<"lname: "<<(int)lic_name[nibble]<<", ";
			int intval = (lic_name[nibble] >= 'a') ? (lic_name[nibble] - 'a' + 10) : (lic_name[nibble] - '0');
			
			//cout<<"Intval = "<<intval<<", ";
			//cout<<"Bit = "<<bit<<"\n";
			fin[i][j] = IsBitSet(intval, bit);
			
			if(fin[i][j] == 1)
				cout<<"X";
			else
				cout<<" ";
			
			++p;
		}
		cout<<"\n";
	}
	
	
	queue<int> Q;
	for(int i = 0; lic_code[i] && i < 256; i += 2)
	{
		int intval = (lic_code[i] >= 'a') ? (lic_code[i] - 'a' + 10) : (lic_code[i] - '0');
		intval <<= 4;
		intval |= (lic_code[i + 1] >= 'a') ? (lic_code[i + 1] - 'a' + 10) : (lic_code[i + 1] - '0');
		
		Q.push((intval >> 6) & 3);
		Q.push((intval >> 4) & 3);
		Q.push((intval >> 2) & 3);
		Q.push((intval >> 0) & 3);
	}
	
	int x = 2;
	int y = 2;
	fin[x][y] = 2;
	
	while(!Q.empty())
	{
		int op = (Q.front() + 2) % 4;
		Q.pop();
		
		if(op == 0)
			cout<<"RIGHT\n";
		else if(op == 1)
			cout<<"UP\n";
		else if(op == 2)
			cout<<"LEFT\n";
		else if(op == 3)
			cout<<"DOWN\n";
		else
			cout<<"Invalid OP!\n";
		
		int newx = x + dx[op];
        int newy = y + dy[op];
		
		//check out of bounds
        if(newx < 1 || newy < 1 || newx > 40 || newy > 40)
        {
			cout<<"Invalid license code! (out of bounds)\n";
			break;
		}
		
		//check visited
        if(fin[newx][newy])
		{
			cout<<"Invalid license code! (hit wall)\n";
			break;
		}	
		fin[newx][newy] = fin[x][y] + 1;
		
		x = newx;
		y = newy;
	}
	
	if(fin[40][40])
		cout<<"License is valid!\n";
	else
		cout<<"License is not valid!\n";
	
	for(int i = 1; i < 41; ++i)
    {
        for(int j = 1; j < 41; ++j)
        {
			if(fin[i][j] >= 2)
			{
				cout<<fixed<<setw(4)<<fin[i][j];
			}
			
            else if(fin[i][j] == 1)
            {
                cout<<"XXXX";
            }

            else
            {
                cout<<"    ";
            }
        }
        cout<<"\n";
    }
	
	return 0;
}

//ffffffffff8800002a02bfbbaaebba8220aa00a0afbbfeabfea22280a828efbaeffaee82a0280202eeeefebeae8028a8a2a2eaeeaffbee8a2802a882fefeeeaeee8008880282fabfeeeeeea28a888a02bbbaeeebaeaa002082a2afaafefaee8aaa202020eafefafeeaa0202288aaaefafbeebb822280a828befbebafae8820a80202bffbbfafeeaaa00aa28aaababaeeeb800a802080aaefaafafbaa82aa0a22bffafebefa80a0a28820abbeafbefea8a808082afbafbebfea808228aaa0bbbbaeeaaea00a000002