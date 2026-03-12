#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

/*
Thread Pool Architecture 

A Thread pool maintains a fixes number of worker threads, that continuously wait
for work(jobs) to arrive

instead of creating a new thread every time a task arrives, jobs are pushed into
a shared queue, worker threas wake up, process the job, and then go back into waiting

this avoids expensive thread creation and desctruction!

Workflow:

Producer Thread (main server)
        ↓
push job into queue
        ↓
condition_variable.notify_one()
        ↓
Sleeping worker wakes up
        ↓
Worker processes job
        ↓
Worker sleeps again

This design is the foundation for scalable servers.

===========================================================
*/

class ThreadPool
{
private:

    /*
    Worker Threads

    A fixed number of threads created when the thread pool
    starts. These threads run a loop where they wait for
    jobs to arrive in the queue.
    */
    std::vector<std::thread> workers;


    /*
    Job Queue

    Shared queue storing pending tasks.
    Worker threads pop jobs from this queue.

    For the HTTP server later this will store:
        socket file descriptors
    */
    std::queue<int> jobs;


    /*
    Queue Mutex

    Protects access to the shared job queue.

    Since multiple threads may push or pop jobs
    simultaneously, all operations on the queue
    must be synchronized using this mutex.
    */
    std::mutex queueMutex;


    /*
    Condition Variable

    Used to avoid busy waiting.

    Worker threads sleep when the queue is empty
    and wake up when the producer adds a new job.

    notify_one() wakes a single worker.
    notify_all() wakes all workers (used during shutdown).
    */
    std::condition_variable cv;


    /*
    Shutdown Flag

    Indicates when the thread pool is shutting down.
    Workers check this flag to know when they should exit.
    */
    bool shutdown = false;


public:

    /*
    Constructor

    Creates the worker threads.

    Each thread immediately starts running a loop
    that waits for jobs to appear in the queue.
    */
    ThreadPool(size_t numThreads);


    /*
    enqueue()

    Adds a new job to the queue.

    After pushing the job, a worker thread is notified
    using condition_variable so it can wake up and
    process the task.
    */
    void enqueue(int job);


    /*
    Destructor

    Gracefully shuts down the thread pool by:
    1. Setting shutdown flag
    2. Waking all workers
    3. Joining all threads
    */
    ~ThreadPool();
};

