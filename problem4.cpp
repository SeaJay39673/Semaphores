/*
    Date: 10/15/2024
    Last Updated: 10/18/2024
    Description:
        Physical Plant Problem.
        2 Clients, 1 Help Desk, And 5 Physical Plant Technicians.
        Clients:
            Clients do something for some amount of time, something breaks, and they need it fixed.
            They signal the help desk to submit a ticket and wait until the plant techs signal that the problem is fixed

        Help Desk:
            All the help desk does is wait for clients to submit a ticket and relay it to 3 of the plant techs (union rules that 3 techs need to work at a time)

        Plant Technicians:
            They stand around and drink coffee in the break room for some amount of time, and then wait in their office to work.
            There must be 3 technicians before work can begin.
            Once signaled for work (and there's enough techs) they work for some time and let the client know the work is done.

*/
//====| STL Includes |====//
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <queue>
#include <mutex>

//====| Namespaces |====//
using namespace std;

//====| Global Variables |====//
const int techs = 5;
sem_t helpDesk, client, plant, tq, notReady;
mutex tex;

queue<int> techQueue = queue<int>();

//====| Function Declarations |====//
void clientProcess(int);
void helpDeskProcess(int);
void physicalPlantTechProcess(int);

//====| Problem 4 |====//
int main()
{

    sem_init(&helpDesk, 0, 0);
    sem_init(&client, 0, 0);
    sem_init(&plant, 0, 0);
    sem_init(&tq, 0, 1);
    sem_init(&notReady, 0, 0);

    thread plantThreads[techs];
    thread hd, cl1, cl2;
    cl1 = thread(clientProcess, 1);
    cl2 = thread(clientProcess, 2);
    hd = thread(helpDeskProcess, 1);

    for (int i = 0; i < techs; i++)
    {
        plantThreads[i] = thread(physicalPlantTechProcess, i + 1);
    }

    for (int i = 0; i < techs; i++)
    {
        plantThreads[i].join();
    }

    cl1.join();
    cl2.join();
    hd.join();
}

void clientProcess(int ID)
{
    printf("Client %i: Starting\n", ID);
    int t;
    while (true)
    {
        // Do something
        t = rand() % 31;
        printf("Client %i: Doing Something for %i seconds\n", ID, t);
        sleep(t);

        // Something breaks, ask for help. How to tell help desk which process is blocked?
        printf("Client %i: Something broke\nCalling help desk\n", ID);

        sem_post(&helpDesk);

        // Wait for it to techs to fix it
        sem_wait(&client);
        printf("Client %i: Back to work\n", ID);
    }
}

void helpDeskProcess(int ID)
{
    printf("Help Desk %i: Starting\n", ID);
    while (true)
    {
        // Wait for ticket submission
        sem_wait(&helpDesk);
        printf("Help Desk %i: Ticket Received, Informing Plant Techs\n", ID);

        for (int i = 0; i < 3; i++)
        {
            sem_post(&plant);
        }
    }
}

void physicalPlantTechProcess(int ID)
{
    printf("Plant Tech %i: Starting\n", ID);
    int t;
    while (true)
    {
        // Break Room
        t = rand() % 31;
        printf("Plant Tech %i: Drinking Coffee for %i seconds\n", ID, t);
        sleep(t);

        // Add current process to readyQueue
        sem_wait(&tq);
        techQueue.push(ID);
        sem_post(&tq);

        // wait until needed
        sem_wait(&plant);
        printf("Plant Tech %i: Work Order Received\n", ID);

        sem_wait(&tq);
        if (techQueue.size() < 3) // If not enough techs to work, wait until there are
        {
            printf("Plant Tech %i: Waiting on others\n", ID);
            sem_post(&tq);
            sem_wait(&notReady);
        }
        else // One tech to do the work and 2 others to supervise
        {
            int techWorkTime = rand() % 31;
            int techs[3];
            for (int i = 0; i < 3; i++)
            {
                techs[i] = techQueue.front();
                printf("Plant Tech %i: Working for %i seconds\n", techs[i], techWorkTime);
                techQueue.pop();
            }
            sleep(techWorkTime);
            for (int i = 0; i < 3; i++)
            {
                printf("Plant Tech %i, Work Done\n", techs[i]);
            }

            // Tell two other waiting techs to be done working
            sem_post(&notReady);
            sem_post(&notReady);

            // Tell client work is done
            sem_post(&client);

            sem_post(&tq);
        }
    }
}
