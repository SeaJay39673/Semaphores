/*
    Date: 10/15/2024
    Last Updated: 10/15/2024
    Description:
        This is the implementation of the Dining Philosophers problem.
        It uses Semaphores and avoids deadlock.
        There are 5 philosophers, and 5 chopsticks. Each philosopher requires 2 chopsticks to eat.
        Each philosopher is it's own thread.
*/

//====| STL Includes |====//
#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <map>
#include <mutex>

//====| Namespaces |====//
using namespace std;

//====| Globals |====//
map<int, sem_t *> chopMap;
mutex tex;

//====| Function Declarations |====//
void think(int);
void eat(int);
void takeChopsticks(int);
void putChopsticks(int);
void start(int *);

//====| Problem 1 |====//
int main()
{
    int philNum = 5;
    int philIDs[philNum];
    thread philosophers[philNum];

    // Create map of philosophers and chopsticks. Current index is right, index - 1 is left.
    for (int i = 0; i < philNum; i++)
    {
        philIDs[i] = i + 1;
        chopMap[i + 1] = new sem_t();
        sem_init(chopMap[i + 1], 0, 1);
    }
    chopMap[0] = chopMap[philNum - 1];

    // Check that leftmost chopstick is same as rightmost chopstick (start = end)
    // cout << chopMap[0] << endl;
    // cout << chopMap[philNum - 1] << endl;

    // Create Philosopher Threads and pass in respective ID
    for (int i = 0; i < philNum; i++)
    {
        philosophers[i] = thread(start, &philIDs[i]);
    }

    // Join Philosopher Threads
    for (int i = 0; i < philNum; i++)
    {
        philosophers[i].join();
    }

    return 0;
}

void start(int *ID)
{
    printf("Philosopher %i: Starting\n", *ID);
    while (true)
    {
        think(*ID);
        takeChopsticks(*ID);
        eat(*ID);
        putChopsticks(*ID);
    }
}

void takeChopsticks(int ID)
{
    printf("Philosopher %i: Taking Chopsticks\n", ID);
    tex.lock();
    sem_wait(chopMap[ID]);     // Wait for right
    sem_wait(chopMap[ID - 1]); // Wait for left
    tex.unlock();
    printf("Philosopher %i: Chopsticks taken\n", ID);
}

void putChopsticks(int ID)
{
    printf("Philosopher %i: Putting Chopsticks\n", ID);
    sem_post(chopMap[ID]);     // Place right
    sem_post(chopMap[ID - 1]); // Place left
    printf("Philosopher %i: Chopsticks placed\n", ID);
}

void think(int ID)
{
    int val = rand() % 31;
    printf("Philosopher %i: Thinking for %i seconds\n", ID, val);
    sleep(val);
}

void eat(int ID)
{
    int val = rand() % 31;
    printf("Philosopher %i: Eating for %i seconds\n", ID, val);
    sleep(val);
}