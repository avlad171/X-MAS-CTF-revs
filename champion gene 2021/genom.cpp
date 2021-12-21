#include "genom.h"
#include <string.h>

int genom::nr_instances = 0;

genom::genom(int problem_size)
{
    this->problem_size = problem_size;
    gene = new char [this->problem_size + 10];
    fitness = 0;

    //std::cout<<"Created the "<<++genom::nr_instances<<"th instance!\n";
}

genom::genom(char * input)
{
    this->problem_size = strlen(input);
    gene = new char [this->problem_size + 10];
    fitness = 0;
    for(int i = 0; i < this->problem_size; ++i)
        gene[i] = input[i] - '0';

    //std::cout<<"Created The "<<++genom::nr_instances<<"th instance!\n";
}

genom::~genom()
{
    delete gene;
}

void genom::mutation(int chance)
{
    for(int i = 0; i < this->problem_size; ++i)
        if(rand() % 100 < chance)
            gene[i] = gene[i] ^ 1;
}

void genom::random_init()
{
    /*for(int i = 0; i < this->problem_size; ++i)
        if(rand()%(this->problem_size / 2))
            gene[i] = 0;
        else
            gene[i] = 1;*/

    for(int i = 0; i < this->problem_size; ++i)
        if(rand() % 3)
            gene[i] = 1;
        else
            gene[i] = 0;

    this->fitness = 0;
}

void crossover(genom * a, genom * b, genom * c)
{
    for(int i = 0; i < a->get_problem_size()/2; ++i)
        c->gene[i] = a->gene[i];
    for(int i = a->get_problem_size()/2; i < a->get_problem_size(); ++i)
        c->gene[i] = b->gene[i];
    c->mutation(3);
}

void crossover2(genom * a, genom * b, genom * c)
{
    for(int i = 0; i < a->get_problem_size()/2; ++i)
        c->gene[i] = a->gene[i];
    for(int i = a->get_problem_size()/2; i < a->get_problem_size(); ++i)
        c->gene[i] = b->gene[i];
    c->mutation(70);
}

bool cmp (const genom * a, const genom * b)
{
    return a->fitness > b->fitness;
}
