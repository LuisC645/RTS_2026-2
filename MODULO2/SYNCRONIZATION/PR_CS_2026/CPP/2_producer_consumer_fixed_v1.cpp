#include <thread>
#include <mutex>
#include <iostream>

int buffer = 0;
std::mutex mutex;

void produce(const int threadId)
{
    std::cout << "Producer started on thread: " << threadId << std::endl;
    while(1)
    {
        mutex.lock();
        ++buffer;
        std::cout << buffer << std::endl;
        mutex.unlock();
    }
}
void consume(const int threadId)
{
    std::cout << "Consumer started on thread: " << threadId << std::endl;
    while(1)
    {
        mutex.lock();
        --buffer;
        std::cout << buffer << std::endl;
        mutex.unlock();
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

