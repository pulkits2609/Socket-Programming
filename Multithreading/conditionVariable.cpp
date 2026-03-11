//what condition variable does : 
//instead of workers constantly checking the queue , they do : 
//sleep -> producer produces jobs -> producer signals condition_variable -> worker wakes up -> worker processes job

//using this workflow the CPU Usage becomes almost zero when idle

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>

using namespace std;

mutex MUTEX;
condition_variable cv;

queue<int> jobQueue;
bool producerFinished = false;

void worker(){
    while (true){
        unique_lock<mutex> lock(MUTEX);

        // wait until there is work OR producer finished
        cv.wait(lock, []{
            return !jobQueue.empty() || producerFinished;
        });

        // if no jobs and producer finished → exit thread
        if(jobQueue.empty() && producerFinished){
            return;
        }
        int job = jobQueue.front();
        jobQueue.pop();

        lock.unlock(); // release mutex before doing work

        cout<<"Thread "<<this_thread::get_id()<<" processed job " << job << endl;
    }
}

void producer(){
    for (int i = 0; i < 1000; i++)
    {
        lock_guard<mutex> guard(MUTEX);
        jobQueue.push(i);

        cv.notify_one(); // wake one worker
    }
    // producer finished generating jobs
    lock_guard<mutex> guard(MUTEX);
    producerFinished = true;
    cv.notify_all(); // wake all workers so they can exit
}

int main()
{
    vector<thread> workers;

    // create worker threads
    for(int i = 0; i < 5; i++)
        workers.emplace_back(worker);

    // create producer thread
    thread prod(producer);

    prod.join();

    for(auto &t : workers)
        t.join();

    return 0;
}
