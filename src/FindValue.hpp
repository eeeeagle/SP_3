#ifndef FIND_VALUE
#define FIND_VALUE

#include "Common.hpp"
#include <random>
#include <algorithm>

pthread_mutex_t mutex;
int ret_index = -1;

struct FindValueArgs
{
    std::vector<int> array;
    int search_value;
    int start;
    int end;
};

void* find_value(void *ptr_args)
{
    auto *args = (struct FindValueArgs *) ptr_args;

    double start_time = get_time();

    pthread_mutex_lock(&mutex);
    for(int i = args->start; i < args->end; i++)
    {
        if(args->array[i] == args->search_value)
        {
            if (ret_index < i)
                ret_index = i;
        }
    }
    pthread_mutex_unlock(&mutex);

    printf("Time of one thread working: %fs\n", get_time() - start_time);
    pthread_exit(nullptr);
}

void find_value_main(const int search_value, const int threads_num = 1)
{
    std::vector<int> array = {};
    for (int i = -5000; i <= 5000; array.push_back(i++));
    std::random_device rd;
    std::mt19937 mt(rd());
    std::shuffle(array.begin(), array.end(), mt);
    size_t size = array.size();

    FindValueArgs args;

    pthread_t threads[threads_num];
    pthread_attr_t attr;

    double start_time = get_time();

    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, nullptr);

    int step = static_cast<int>(size) / threads_num;
    int pos = 0;

    args.array = array;
    args.search_value = search_value;

    for(int k = 0; k < threads_num; k++)
    {
        args.start = pos;
        pos += step;
        args.end = (k == threads_num - 1) ? size : pos;

        if(pthread_create(&threads[k], &attr, find_value, (void *)&args))
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