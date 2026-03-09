//exercise 1 : creating first thread

#include<iostream>
#include<thread>
//this gives us std::thread
//thread constructor
//join()
//detach
using namespace std;

//we create a function that will be used by the thread 
void printSomething(){
    cout<<"Worker Thread ID : "<<this_thread::get_id()<<"\n";
    for(int i=0;i<5;i++){
        cout<<"Hello from thread\n";
    }
}
int main(){
    thread worker(printSomething);
    worker.detach();
    //what is happening internally : 
    //os creates thread
    //start executing function
    //return control to caller (this process)

    //thread START IMMEDIATELY after construction 

    //when thread worker(printSomething) is executed there are 2 threads : main thread and worker thread

    //now if we do worker.join(); 
    //main thread waits till worker thread finishes executing
    //worker.detach()
    //main thread continues execution and does not wait for worker thread to finish
    
    //one thing to keep in mind is that if thread is detached and the main thread exits (returns) then all worker threads will stop midway
    return 0;
}


//after program execution we get something like this : 
// Worker Thread ID : 130880565671616
// Hello from thread
// Hello from thread
// Hello from thread
// Hello from thread
// Hello from thread
