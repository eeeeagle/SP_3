#ifndef FIND_VALUE
#define FIND_VALUE

#include "Common.hpp"
#include <random>
#include <algorithm>

pthread_mutex_t mutex;
volatile int ret_index = -1;

struct FindValueArgs
{
    int const* array;
    int search_value;
    int start;
    int end;
};

void* find_value(void *ptr_args)
{
    auto *args = (struct FindValueArgs *) ptr_args;

    double start_time = get_time();

    for(int i = args->start; i < args->end; i++)
    {
        if(i % 100 == 0)
        {
            if (ret_index >= args->end)
                return nullptr;
        }
        if(args->array[i] == args->search_value)
        {
            pthread_mutex_lock(&mutex);
            if (ret_index < i)
                ret_index = i;
            pthread_mutex_unlock(&mutex);
        }
    }

    printf("Time of one thread working: %fs\n", get_time() - start_time);
    pthread_exit(nullptr);
}

void find_value_main(const std::vector<int>& array, const int search_value, const int threads_num = 1)
{
    size_t size = array.size();

    std::vector<FindValueArgs> args(threads_num);

    pthread_t threads[threads_num];
    pthread_attr_t attr;

    double start_time = get_time();

    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, nullptr);

    int step = static_cast<int>(size) / threads_num;
    int pos = 0;

    for(int k = 0; k < threads_num; k++)
    {
        args[k].array = array.data();
        args[k].search_value = search_value;
        args[k].start = pos;
        pos += step;
        args[k].end = (k == threads_num - 1) ? size : pos;

        if(pthread_create(&threads[k], &attr, find_value, (void *)&args[k]))
        {
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

    pthread_attr_destroy(&attr);

    for(int k = 0; k < threads_num; k++)
        pthread_join(threads[k], nullptr);

    std::cout << "All time search: " << get_time() - start_time << std::endl;
    pthread_mutex_destroy(&mutex);

    if(ret_index != -1)
        std::cout << "Finded index: " << ret_index << std::endl;
    else
        std::cout << "Not found" << std::endl;
}
#endif