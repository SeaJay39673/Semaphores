/*
    Date: 10/15/2024
    Last Updated: 10/15/2024
    Description:
        Dining savages problem. 10 savages and 1 cook.
        Each savage takes turns eating from a communal pot, if the pot is empty, they wake up the cook and wait until the pot is full
*/

//====| STL Includes |====//
#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <map>

//====| Namespaces |====//
using namespace std;

//====| Globals |====//
const int M = 20; // Capacity of pot
int servings = M; // Current servings
sem_t pot, cookSem, sleepCook;

//====| Function Declarations |====//
void savage(int);
void eat(int);
void digest(int);
void startCook();
void fillPot();

//====| Problem 2 |====//
int main()
{
    sem_init(&pot, 0, 1);
    sem_init(&cookSem, 0, 0);
    sem_init(&sleepCook, 0, 0);

    int savageNum = 10;
    thread savages[savageNum];

    for (int i = 0; i < savageNum; i++)
    {
        savages[i] = thread(savage, i + 1);
    }

    thread cook(startCook);
    cook.join();

    for (int i = 0; i < savageNum; i++)
    {
        savages[i].join();
    }
    return 0;
}

void savage(int ID)
{
    printf("Savage %i: Starting\n", ID);
    while (true)
    {
        eat(ID);
        digest(ID);
    }
}

void eat(int ID)
{
    sem_wait(&pot);
    printf("Savage %i: Eating\n", ID);
    if (servings == 0)
    {
        printf("Savage %i: Waking Up Cook\n", ID);
        sem_post(&sleepCook);
        sem_wait(&cookSem);
    }
    servings--;
    sem_post(&pot);
}

void digest(int ID)
{
    int val = rand() % 31;
    printf("Savage %i: Digesting for %i seconds\n", ID, val);
    sleep(val);
}

void startCook()
{
    while (true)
    {
        sem_wait(&sleepCook);
        fillPot();
        sem_post(&cookSem);
    }
}

void fillPot()
{
    int val = rand() % 31;
    printf("Cook: Cooking for %i seconds\n", val);
    sleep(val);
    servings = M;
    printf("Cook: Cooking complete\n");
}