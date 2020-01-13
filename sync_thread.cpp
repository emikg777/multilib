#include "sync_thread.hpp"
#include <iostream>
#include <chrono>

using std::cout;
using std::endl;
using std::thread;

static object_sync thr_transfer;

static void thr(){
    int test = 5;
    thr_transfer.put<int&, object_sync::ignore, object_sync::same_object>(test);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << test << endl;
}

int main(){
    thread th(thr);
    int& test = thr_transfer.get<int>();
    cout << test << endl;
    ++test;
    cout << test << endl;
    th.join();
    return 0;
}
