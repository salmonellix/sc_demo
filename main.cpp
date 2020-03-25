#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <atomic>

// zmienne z podkreślinikiem, clasy duża litera camel, funkcje mała camel
// global function:

void globalFun(std::string add_text){

    std::thread::id threads_id = std::this_thread::get_id();
    std::cout << "Thread id: " << threads_id << " - Text: " << add_text << "\n" << std::endl;;
}

// class method

class ClassMethod
{
public:
    void makeThread(std::string add_text_m){

        std::thread::id threads_id = std::this_thread::get_id();
        std::cout << "Thread id: " << threads_id << " - Text: " << add_text_m << "\n" << std::endl;;
    }
};

// function object

class ObjectFunctor{

public:
    void operator() (std::string add_text_f){

        std::thread::id threads_id = std::this_thread::get_id();
        std::cout << "Thread id: " << threads_id << " - Text: " << add_text_f << "\n" << std::endl;;

    }

};

void firstTask(){

    std::cout << "> ----- FIRST TASK ----- <" << std::endl;
    std::thread first(globalFun, "Global Function");
    first.join();

    auto *t2 = new ClassMethod;
    std::thread second(&ClassMethod::makeThread, t2, "Class Method");
    second.join();
    ObjectFunctor t3;
    std::thread third(t3, "Function Object");
    third.join();

    // lambda function
    std::thread fourth([](std::string add_text_l){
        std::thread::id threads_id = std::this_thread::get_id();
        std::cout << "Thread id: " << threads_id << " - Text: " << add_text_l << "\n" << std::endl;
    },"Lambda Function");
    fourth.join();
}


// zad 2
std::mutex m;

void threadFunction(std::string text_passed) {

    m.lock();
    for (int i = 0; i < 50; i++) {

        std::cout << text_passed << std::endl;
    }
    std::cout << "end of thread" <<  std::endl;
    m.unlock();
}


void secondTask(){

    std::thread t[20];
    std::cout << "> ----- SECOND TASK ----- <" << std::endl;
    for (auto & i : t){
        i = std::thread(threadFunction, "Like it");
    }
    for (auto & i : t){
        i.join();
    }

    std::thread print_msg(threadFunction, "Like it");
    print_msg.join();
}

// zad 3
int var1 = 0;
int incrementMutex(int var){

    for (int i =0; i <10000000; i++){
        var++;
    }
    return var;
}

std::atomic<int> var2;
int incrementAtomic(){

    for (int i =0; i <10000000; i++){
        var2++;
    }
    return var2.load();
}

void thirdTask(){

    std::cout << "> ----- THIRD TASK ----- <" << std::endl;
    // atomic
    auto start = std::chrono::steady_clock::now();
    std::vector<std::thread> threads(10);
    for (int i = 0; i <10; i++){
        threads[i] = std::thread(incrementAtomic);
    }
    for (auto & i : threads){
        i.join();
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> time = end-start;
    std::cout << "time: " << time.count() << "s\n";
    std::cout << "var2 : " << var2 << std::endl;

// mutex
    auto start2 = std::chrono::steady_clock::now();
    std::vector<std::thread> threads2(10);
    for (int i = 0; i <10; i++){
        threads2[i] = std::thread(incrementMutex, var1);
    }
    for (auto & i : threads2){
        i.join();
    }

    auto end2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time2 = end2-start2;
    std::cout << "time: " << time2.count() << "s\n";
    std::cout << "var1 : " << var1 << std::endl;
}



int main() {

    firstTask();
    secondTask();
    thirdTask();
    std::cin.get();
    std::cin.get();
    return 0;
}
