#ifndef GENOM_H_INCLUDED
#define GENOM_H_INCLUDED

#include <iostream>
#include <time.h>

using namespace std;

class genom
{
    private:
        int problem_size;
        int fitness;
        char * gene;

    public:
        static int nr_instances;

        genom(int problem_size);
        genom(char *);

        void mutation(int);
        void random_init();

        int get_problem_size()
        {
            return this->problem_size;
        }

        void set_fitness(int new_fitness)
        {
            this->fitness = new_fitness;
        }

        void increase_fitness(int with)
        {
            this->fitness += with;
        }

        int get_fitness() const
        {
            return this->fitness;
        }

        const char * get_gene() const
        {
            return gene;
        }

        bool operator < (const genom & rhs) const
        {
            //std::cout<<"Comparing "<<this->fitness<<" with "<<rhs.fitness<<endl;
            return this->fitness < rhs.fitness;
        }

        virtual ~genom();

        void friend crossover(genom * a, genom * b, genom * c);
        void friend crossover2(genom * a, genom * b, genom * c);
        bool friend cmp(const genom * a, const genom * b);
};

#endif // GENOM_H_INCLUDED
