#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <queue>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>

#include <openssl/sha.h>

using namespace std;

#include "genom.h"

const int maxg = 5483;
const int admin_score = 4452980;    //real solution is 4452981

const char * good_flag_sha256 = "c03ad8901b4705458d66978eea642cd501083f7df7ea71cbdaa84907df2d1235";

int G [] = {80, 15, 35, 2, 9, 69, 82, 57, 11, 85, 26, 52, 18, 39, 21, 7, 17, 7, 80, 22, 9, 16, 37, 29, 78, 45, 36, 41, 40, 27, 74, 8, 81, 29, 11, 14, 83, 44, 3, 28, 53, 27, 54, 12, 15, 76, 19, 36, 84, 42, 53, 9, 11, 9, 29, 83, 27, 33, 34, 5, 75, 58, 84, 72, 33, 26, 1, 62, 23, 17, 10, 44, 67, 58, 34, 1, 17, 71, 52, 75, 79, 10, 12, 33, 56, 57, 60, 2, 56, 75, 13, 48, 16, 17, 23, 13, 75, 56, 12, 54, 22, 53, 82, 80, 55, 48, 33, 11, 51, 29, 85, 65, 28, 36, 29, 36, 25, 60, 27, 19, 58, 29, 7, 15, 37, 77, 36, 85, 79, 49, 15, 34, 44, 76, 22, 18, 35, 47, 35, 4, 20, 27, 42, 44, 30, 81, 38, 65, 68, 72, 51, 20, 25, 62, 28, 31, 11, 47, 35, 26, 21, 29, 12, 20, 52, 15, 19, 16, 76, 14, 23, 66, 77, 50, 21, 23, 35, 64, 28, 14, 59, 4, 81, 15, 46, 17, 11, 81, 52, 58, 14, 80, 74, 71, 2, 1, 65, 47, 17, 78, 72, 10, 47, 26, 26, 48, 47, 40, 66, 24, 24, 38, 28, 56, 6, 3, 76, 40, 40, 36, 59, 79, 78, 33, 42, 61, 2, 7, 77, 53, 11, 1, 58, 19, 58, 27, 66, 42, 76, 21, 78, 46, 37, 42, 84, 77, 2, 64, 31, 80, 37, 47, 54, 30, 16, 59, 51, 43, 24, 1, 42, 83, 8, 15, 44, 63, 36, 19, 85, 82, 84, 15, 48, 47, 11, 84, 45, 74, 18, 59, 54, 50, 3, 8, 52, 60, 50, 56, 52, 70, 4, 4, 76, 1, 56, 69, 25, 75, 6, 35, 11, 45, 73, 83, 34, 29, 50, 27, 50, 10, 50, 51, 53, 72, 26, 16, 7, 80, 65, 1, 61, 41, 22, 19, 69, 85, 73, 32, 47, 85, 23, 9, 29, 68, 65, 44, 63, 68, 23, 31, 66, 11, 3, 68, 57, 8, 5, 84, 79, 2, 19, 64, 67, 38, 6, 28, 75, 12, 82, 71, 85, 23, 47, 64, 68, 79, 56, 30, 58, 34, 12, 61, 74, 85, 83, 7, 76, 37, 25, 6, 82, 4, 30, 81, 60, 23, 36, 7, 82, 77, 32, 51, 26, 77, 31, 3, 19, 11, 85, 23, 39, 70, 41, 30, 33, 84, 44, 28, 38, 4, 23, 68, 59, 64, 31, 22, 26, 28, 2, 25, 59, 16, 28, 4, 77, 21, 45, 48, 79, 13, 76, 33, 48, 35, 35, 37, 31, 82, 12, 24, 83, 6, 1, 17, 69, 14, 21, 13, 16, 58, 23, 45, 40, 6, 16, 45, 75, 80, 45, 59, 39, 9, 16, 38, 69, 79, 28, 7, 39, 24, 77, 64, 78, 56, 47, 41, 71, 37, 54, 13, 66, 57, 4, 59, 85, 74, 55, 34, 77, 7, 45, 78, 75, 50, 43, 59, 22, 28, 23, 34, 77, 1, 10, 84, 65, 43, 23, 67, 34, 64, 14, 40, 41, 44, 11, 76, 84, 29, 33, 44, 27, 4, 39, 53, 79, 44, 13, 35, 60, 76, 18, 15, 4, 56, 84, 19, 45, 34, 11, 56, 71, 1, 36, 50, 51, 81, 4, 37, 77, 6, 39, 77, 47, 2, 8, 42, 72, 72, 40, 43, 40, 43, 40, 31, 44, 31, 77, 3};
int P [] = {16137, 20643, 9019, 13121, 19262, 13095, 21671, 15248, 3709, 714, 17368, 9068, 19586, 17249, 2067, 13746, 930, 27064, 3056, 6700, 12078, 11772, 2276, 20752, 20601, 25527, 6740, 13573, 6298, 8857, 26377, 16037, 20000, 23354, 1639, 9950, 24451, 11393, 23536, 12584, 20733, 14932, 26569, 7002, 9484, 23506, 23133, 18324, 25868, 17077, 26043, 25181, 2055, 12770, 18030, 2407, 8983, 10196, 18514, 8983, 21762, 11200, 10107, 26190, 1459, 17439, 22071, 8727, 2414, 1657, 5106, 8911, 5586, 9713, 25367, 16199, 1896, 1398, 3930, 16081, 2048, 22938, 22099, 5682, 19943, 2975, 25865, 19818, 7153, 21921, 20635, 23733, 11887, 6347, 854, 219, 27362, 18690, 5040, 26001, 16485, 7808, 7425, 9481, 13220, 793, 21679, 7352, 2383, 22353, 15043, 22772, 2519, 5008, 17406, 16571, 16451, 22627, 748, 9541, 16420, 1777, 10355, 20830, 9681, 22790, 7491, 25188, 14688, 5845, 13795, 17936, 12034, 424, 11300, 9953, 13415, 9453, 15226, 13427, 19394, 8648, 16323, 19137, 5428, 12563, 15538, 27182, 19776, 1076, 2377, 18239, 4425, 3512, 22963, 23757, 12791, 13828, 1834, 23719, 4038, 316, 21464, 21790, 14333, 10424, 2129, 19342, 15245, 13168, 13848, 26848, 12991, 25473, 3191, 23965, 4116, 10169, 15880, 18201, 18872, 10475, 11848, 23479, 12727, 13418, 24786, 12100, 7982, 9425, 17075, 24171, 20767, 2071, 17105, 1597, 6990, 1986, 5818, 21277, 23868, 16485, 26832, 6256, 3022, 8094, 24126, 2584, 14125, 20364, 13484, 460, 8857, 2516, 10288, 24363, 7184, 26264, 21426, 11434, 6610, 6459, 13560, 25522, 10493, 12829, 17327, 13581, 13221, 18500, 8101, 781, 1346, 22137, 25749, 9057, 26647, 20598, 21404, 25493, 23827, 10234, 20198, 25795, 19154, 8445, 1847, 16488, 6130, 23174, 15437, 24045, 14962, 13046, 9194, 19998, 14783, 8650, 19228, 8878, 7721, 11624, 15685, 19920, 17744, 7733, 25946, 26968, 23222, 5689, 8241, 13344, 12169, 25513, 27247, 25048, 17993, 17982, 14090, 6747, 15489, 4679, 17856, 17664, 24263, 1217, 3924, 21216, 13281, 3514, 5903, 12297, 8383, 1466, 15552, 15937, 6778, 13533, 22903, 22726, 15250, 7896, 1755, 8588, 13400, 22609, 26787, 8523, 4768, 24514, 2736, 19227, 7297, 2541, 19468, 12142, 9262, 6030, 18772, 27511, 11816, 5916, 11692, 8904, 15880, 19236, 23063, 4863, 5278, 15147, 23068, 5922, 23716, 24785, 23712, 18839, 23485, 8988, 26448, 25205, 257, 9710, 15845, 2103, 11980, 26909, 11031, 17386, 25097, 10486, 14574, 7114, 8599, 15738, 25754, 2333, 5103, 20932, 7051, 19510, 25785, 22578, 23637, 24309, 11929, 24810, 20360, 26758, 11829, 23319, 14041, 1237, 23851, 12954, 6869, 25528, 14175, 11675, 22095, 16689, 3315, 21897, 27327, 23671, 7077, 661, 23898, 13146, 15104, 6717, 13211, 1344, 10742, 841, 26278, 12545, 21980, 2766, 4293, 22801, 12552, 8453, 21662, 18682, 19708, 3539, 4148, 1962, 9709, 16537, 13003, 22481, 17908, 25909, 16027, 25650, 1828, 3768, 26315, 20072, 23518, 7654, 15908, 23159, 13698, 20788, 22109, 13291, 17488, 9880, 26372, 14739, 7019, 10428, 18578, 20110, 17755, 4580, 1385, 6870, 15641, 879, 14905, 15174, 14608, 12778, 13014, 13440, 5476, 1338, 21590, 18623, 13090, 3493, 7792, 15710, 4176, 6604, 22789, 26579, 904, 12684, 27080, 21489, 4707, 3205, 20689, 21707, 14335, 422, 12206, 23882, 18290, 25145, 20473, 3180, 16704, 25977, 18536, 10098, 18278, 11279, 8283, 27099, 5731, 22695, 10404, 2762, 24495, 3885, 23169, 2506, 769, 19669, 26098, 1280, 4039, 24507, 22291, 4654, 13505, 22374, 1242, 18770, 18615, 18630, 26272, 24678, 12009, 17711, 19728, 18832, 6920, 25119, 12757, 9943, 8258, 24169, 17342, 3476, 5996, 8825, 22278, 1338, 7100, 7997, 24019, 7817, 1806, 13463, 23371, 6933, 4692, 19571, 22026, 21986, 5664, 17509, 9362, 1213, 4232, 12214, 23885, 8896, 13794, 18028, 5197, 20433, 27064, 25915, 13103, 15827, 1197, 1616, 6651, 5565, 26237, 11297, 1707, 19432, 14666, 26193, 17901, 27224, 22618, 15508, 14499, 1934};

char flag[128];
char set_bits[576];
genom * generatie [2060];

string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

int main()
{

    cout<<"Admin score: "<<admin_score<<"\nIf you beat it you win. Enter your lucky seed: ";
    cin>>flag;

    if(strlen(flag) > 71)
    {
        cout<<"Error! Seed too long!\n";
        return 0;
    }

    //set flags
    for(int i = 0; i <= 70; ++i)
    {
        unsigned char c = flag[i];
        unsigned char mask = 0x80;

        for(int j = 0; j < 8; ++j)
        {
            if(c & (mask >> j))
            {
                set_bits[i * 8 + j] = '1';
            }
            else
                set_bits[i * 8 + j] = '0';
        }
    }

    //cout<<"To binary: "<<set_bits<<"\n\n";

    int problem_size = sizeof(G) / sizeof(int);

    //initialize genetic alg
    srand(time(0));

    generatie[0] = new genom(set_bits);

    for(int i = 1; i < 2060; ++i)
    {
        generatie[i] = new genom(problem_size);
        generatie[i]->random_init();
    }

    int gen_count = 0;
    int newb_count = 0;
    int maxim = 0;
    int last_update = 0;
    while (gen_count - last_update <= 1024)
    {
        //evaluate individuals
        for(int i = 0; i < 2000; ++i)
        {
            generatie[i]->set_fitness(0);

            int gtemp = 0;
            for(int j = 0; j < problem_size; ++j)
                if(generatie[i]->get_gene()[j] == 1)
                {
                    gtemp += G[j];
                    generatie[i]->increase_fitness(P[j]);
                }

            if (gtemp > maxg)
                generatie[i]->set_fitness(maxg - gtemp);

        }

        //cout<<"Sorting...\n";
        //sort(generatie, generatie + 2000, [ ](const genom * lhs, const genom * rhs ) {return lhs->get_fitness() > rhs->get_fitness();});  //sort individuals by fitness to simulate natural selection

        //replace std::sort using lambdas with a bubble sort; it's easier to reverse and slower thus reducing the chance the algorithm will
        //actually compute the flag
        bool bubble_sorted = false;
        while(!bubble_sorted)
        {
            bubble_sorted = true;
            for(int i = 0; i < 2000; ++i)
            {
                if(generatie[i]->get_fitness() < generatie[i + 1]->get_fitness())
                {
                    bubble_sorted = false;
                    swap(generatie[i], generatie[i + 1]);
                }
            }
        }

        if(generatie[0]->get_fitness() > maxim)
        {
            maxim = generatie[0]->get_fitness();
            last_update = gen_count;
            cout<<"Iteration "<<gen_count<<" - "<<maxim<<" ";

            /*int gtemp = 0;
            for(int j = 0; j < problem_size; ++j)
                if(generatie[0]->get_gene()[j] == 1)
                {
                    gtemp += G[j];
                }

            cout<<"G = "<<gtemp;*/
            cout<<endl;
        }

        if(generatie[0]->get_fitness() > admin_score)
        {
			//if(gen_count > 1)
			//{
			//	asm("mov rax, 0xDEADBEEF\n\tjmp rax\n\t");	//this will crash the program
			//}
			
			if(strcmp(sha256(flag).c_str(), good_flag_sha256))
			{
				cout<<"Your gene might have beaten the admin, but since this contest is *fair*™ we do not grant you the rank of winner.\n";
			}
			
			else
			{
				cout<<"You won.\n";
			}
			
			//cout<<"SHA256 of your flag is: "<<sha256(flag)<<endl;
            return 0;
        }

        ++gen_count;    //increment gen count
        newb_count = 0;
        for(int i = 0; i < 32; ++i)
            for(int j = 0; j < 32; ++j) //cross breed the first 32 genomes
            {
                crossover(generatie[i], generatie[j], generatie[1000 + newb_count]);
                ++newb_count;
            }

    }
    cout<<"No more improvement detected. Program terminated. You lose.\n";
    return 0;
}

//code used to test the genetic alg
/*
#include <fstream>
genom * generatie [2060];
int main()
{
    ifstream in ("rucsac.in");

    int problem_size;
    int maxg;

    int * P = new int [problem_size];
    int * G = new int [problem_size];

    in>>problem_size>>maxg;

    for(int i = 0; i < problem_size;  ++i)
        in>>G[i]>>P[i];

    in.close();

    //initialize genetic alg
    srand(time(0));

    for(int i = 0; i < 2060; ++i)
    {
        generatie[i] = new genom(problem_size);
        generatie[i]->random_init();
    }

    int gen_count = 0;
    int newb_count = 0;
    int maxim = 0;
    while (gen_count < 300000)
    {
        //evaluate individuals
        for(int i = 0; i < 2000; ++i)
        {
            generatie[i]->set_fitness(0);

            int gtemp = 0;
            for(int j = 0; j < problem_size; ++j)
                if(generatie[i]->get_gene()[j] == 1)
                {
                    gtemp += G[j];
                    generatie[i]->increase_fitness(P[j]);
                }

            if (gtemp > maxg)
                generatie[i]->set_fitness(maxg - gtemp);

        }

        //cout<<"Sorting...\n";
        sort(generatie, generatie + 2000, [ ](const genom * lhs, const genom * rhs ) {return lhs->get_fitness() > rhs->get_fitness();});  //sort individuals by fitness to simulate natural selection


        if(generatie[0]->get_fitness() > maxim)
        {
            maxim = generatie[0]->get_fitness();
            cout<<gen_count<<" - "<<maxim<<" ";

            int gtemp = 0;
            for(int j = 0; j < problem_size; ++j)
                if(generatie[0]->get_gene()[j] == 1)
                {
                    gtemp += G[j];
                }

            cout<<"G = "<<gtemp<<"\n";
        }

        ++gen_count;
        newb_count = 0;
        for(int i = 0; i < 32; ++i)
            for(int j = 0; j < 32; ++j)
            {
                if(gen_count % 200 == 0)
                    crossover2(generatie[i], generatie[j], generatie[1000 + newb_count]);
                else
                    crossover(generatie[i], generatie[j], generatie[1000 + newb_count]);
                ++newb_count;
            }

        if(gen_count % 400 == 1)
        {
            for(int i = 5; i < 2000; ++i)
                generatie[i]->random_init();
        }
    }
}*/
