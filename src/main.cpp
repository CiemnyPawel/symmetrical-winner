#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <iostream>

#include "Vector.h"
#include "LinkedList.h"

namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;

void perfomTest(size_t amount)
{
    /*LinearCollection<std::string> collection;
    collection.append("TODO");*/
    using namespace std::chrono;
    if(amount > 100000000) amount = 100000000;
    else if(amount < 10) amount = 10;

    aisdi::LinkedList<std::string> list;
    aisdi::Vector<std::string> vector;

    std::string element  = "testujemy";

    //appending
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(size_t count = 0; count <= amount; count++)
    {
        vector.append(element);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto vector_time = duration_cast<milliseconds>( t2 - t1 ).count();

    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    for(size_t count = 0; count <= amount; count++)
    {
        list.append(element);
    }
    high_resolution_clock::time_point t4 = high_resolution_clock::now();

    auto list_time = duration_cast<milliseconds>( t4 - t3 ).count();

    std::cout<<"Appending "<<amount<<" elements took"<<std::endl<<
             vector_time<<" in vector"<<std::endl<<
             list_time<<" in list"<<std::endl;

    //inserting at possition begin +2
    t1 = high_resolution_clock::now();
    vector.insert(vector.begin()+2, element);
    t2 = high_resolution_clock::now();

    vector_time = duration_cast<milliseconds>( t2 - t1 ).count();

    t3 = high_resolution_clock::now();
    list.insert(list.begin()+2, element);
    t4 = high_resolution_clock::now();

    list_time = duration_cast<milliseconds>( t4 - t3 ).count();

    std::cout<<"Inserting 2 positions after begin took"<<std::endl<<
             vector_time<<" in vector"<<std::endl<<
             list_time<<" in list"<<std::endl;

    //inserting at possition end -2
    t1 = high_resolution_clock::now();
    vector.insert(vector.end()-2, element);
    t2 = high_resolution_clock::now();

    vector_time = duration_cast<milliseconds>( t2 - t1 ).count();

    t3 = high_resolution_clock::now();
    list.insert(list.end()-2, element);
    t4 = high_resolution_clock::now();

    list_time = duration_cast<milliseconds>( t4 - t3 ).count();

    std::cout<<"Inserting 2 positions before end took"<<std::endl<<
             vector_time<<" in vector"<<std::endl<<
             list_time<<" in list"<<std::endl;

    //erasing
    t1 = high_resolution_clock::now();
    vector.erase(vector.begin(), vector.end());
    t2 = high_resolution_clock::now();

    vector_time = duration_cast<milliseconds>( t2 - t1 ).count();

    t3 = high_resolution_clock::now();
    list.erase(list.begin(),list.end());
    t4 = high_resolution_clock::now();

    list_time = duration_cast<milliseconds>( t4 - t3 ).count();

    std::cout<<"Erasing from begin to end took"<<std::endl<<
             vector_time<<" in vector"<<std::endl<<
             list_time<<" in list"<<std::endl;

    //prepending
    amount /= 10;
    t1 = high_resolution_clock::now();
    for(size_t count = 0; count <= amount; count++)
    {
        vector.prepend(element);
    }
    t2 = high_resolution_clock::now();

    vector_time = duration_cast<milliseconds>( t2 - t1 ).count();

    t3 = high_resolution_clock::now();
    for(size_t count = 0; count <= amount; count++)
    {
        list.prepend(element);
    }
    t4 = high_resolution_clock::now();

    list_time = duration_cast<milliseconds>( t4 - t3 ).count();

    std::cout<<"Prepending "<<amount<<" elements took"<<std::endl<<
             vector_time<<" in vector"<<std::endl<<
             list_time<<" in list"<<std::endl<<std::endl<<std::endl<<std::endl;

}

} // namespace

int main(int argc, char** argv)
{
    /*const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
    for (std::size_t i = 0; i < repeatCount; ++i)
        perfomTest();
    return 0;*/
    for(size_t amount=1000; amount<=1000000 ; amount*=10)
        perfomTest(amount);
    return 0;
}
