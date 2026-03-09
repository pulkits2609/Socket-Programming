//testing threads

#include<iostream>
#include<thread>
using namespace std;

void printStuff(){
    cout<<"Worker Thread ID : "<<this_thread::get_id()<<"\n";
    for(int i=0;i<1000;i++){
        cout<<" "<<i;
    }
    
    cout<<"\nWorker Thread Closing\n";
}

int main(){
    thread worker(printStuff);
    worker.detach();

    cout<<"\nMain Thread Closed\n";
    return 0; 
}

//here what we aim to see is that if the main thread stops and
//a working thread is working , does it complete execution or stop midway ?

//so when running this we observe that sometimes the function executes completely , or doesnt execute at all
//this is because the threads are not synchronized and its just a gamble whether the worker thread will get CPU Time or not! 