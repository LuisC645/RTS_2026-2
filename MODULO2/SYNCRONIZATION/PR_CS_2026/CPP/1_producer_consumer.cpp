#include <thread>
#include <iostream>

using namespace std;

int buffer = 0;
void produce(const int threadId)
{
    cout << "Producer started on thread: " << threadId << endl;
    while(1)
    {
        // do stuff
        ++buffer;
        cout << buffer << endl;
    }
}
void consume(const int threadId)
{
    cout << "Consumer started on thread: " << threadId << endl;
    while(1)
    {
        // do stuff
        --buffer;
        cout << buffer << endl;
    }
}
int main()
{
    cout << "buffer address: " << &buffer << endl;
    thread producer(produce, 0);
    thread consumer(consume, 1);
    producer.join();
    consumer.join();
    return 0;
}


