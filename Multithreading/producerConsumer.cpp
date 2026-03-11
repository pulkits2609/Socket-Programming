//in this program we will be simulating the producer consumer problem
//the producer is going to produce jobs (here they are just numbers 1 2 3 4...)
//and the consumer is going to process them

#include<iostream>
#include<thread>
#include<mutex>
#include<queue>
#include<vector>
using namespace std;

mutex MUTEX;
queue<int> jobQueue;
bool jobPushComplete = false;

void processJob(){
    while(1){
        lock_guard<mutex> guard(MUTEX);
        if(!jobQueue.empty()){
            cout<<"Processed Job : "<<jobQueue.front()<<"\n";
            jobQueue.pop();
        }
        if(jobQueue.empty() && jobPushComplete) return;
        //no need to manually unlock guard because in the next loop iteration it will be unlocked automatically
    }
}
void produceJob(){
    for(int i=0;i<1000;i++){
        lock_guard<mutex> guard(MUTEX);
        jobQueue.push(i);
        if(i==999) jobPushComplete = true;
    }
}

int main(){
    vector<thread> workers;
    workers.push_back(thread(produceJob));
    for(int i=0;i<5;i++){
        workers.push_back(thread(processJob));
    }
    for(int i=0;i<6;i++){
        workers[i].join();
    }
    return 0;
}

//right now in this program workers do this : 
//lock -> check -> unlock ->repeat

//therefore if the queue is empty then locking and unlocking causes busy spinning (busy spinning) to avoid this we introduce condition_variable !!!