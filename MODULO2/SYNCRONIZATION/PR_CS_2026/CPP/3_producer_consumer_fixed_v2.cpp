#include <thread>
#include <mutex>
#include <iostream>
#include <condition_variable>

int buffer = 0;
const int MAX = 100;
std::mutex mymutex;
std::condition_variable full, empty;


void produce(const int threadId)
{
    std::cout << "Producer started on thread: " << threadId << std::endl;
    while(1)
    {
        std::unique_lock<std::mutex> mylock1(mymutex);
        if (buffer == MAX) {
            std::cout << "Producer waiting for space..." << std::endl;
            full.wait(mylock1);
        }
        ++buffer;
        std::cout << "Buffer size (prod):" << buffer << std::endl;
        empty.notify_one();
        mylock1.unlock();
    }
}
void consume(const int threadId)
{
    std::cout << "Consumer started on thread: " << threadId << std::endl;
    while(1)
    {
        std::unique_lock<std::mutex> mylock2(mymutex);
        if (buffer == 0) {
            std::cout << "Consumer waiting for elements..." << std::endl;
            empty.wait(mylock2);
        }
        --buffer;
        std::cout << "Buffer size (cons):" << buffer << std::endl;
        full.notify_one();
        mylock2.unlock();
    }
}
int main()
{
    std::cout << "buffer address: " << &buffer << std::endl;
    std::thread producer(produce, 0);
    std::thread consumer(consume, 1);
    producer.join();
    consumer.join();
    return 0;
}
