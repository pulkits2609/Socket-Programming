//race condition test

#include<iostream>
#include<thread>
#include<vector>
using namespace std;

int counter = 0;

void incrementCounter(){
    for(int i=1;i<=10000;i++){
        counter+=1;
    }
}

int main(){
    //what we aim to test here is that , we are going to make 5 different worker threads
    //each thread will be trying to change the same variable, we observe race condition here where different threads will try to access and change a single variable at the same time 
    // thread worker1(incrementCounter);
    // worker1.join();
    // thread worker2(incrementCounter);
    // worker2.join();
    // thread worker3(incrementCounter);
    // worker3.join();
    // thread worker4(incrementCounter);
    // worker4.join();
    // thread worker5(incrementCounter);
    // worker5.join(); 

    //why the above code wont run like race condition , because join function makes the main thread wait for worker thread to finish , therefore each thread will execute sequencially !

    vector<thread> workers;
    // create 5 threads
    for(int i = 0; i < 5; i++) {
        workers.emplace_back(incrementCounter);
    }
// wait for all threads 
    for(auto &t : workers){
        t.join(); 
    }
    cout << "Final Counter Value: " << counter << endl;
    return 0;
}

//when running this code we will be seeing values like 3001 4052 etc because this is race condition