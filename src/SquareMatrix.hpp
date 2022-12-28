#ifndef SQUARE_MATRIX
#define SQUARE_MATRIX

#include "Common.hpp"
#include <cmath>
#include <fcntl.h>

using T = double;

template <typename T>
class SquareMatrix
{
private:
    std::vector<T> data;
    size_t order;

    struct MulMatrixArgs
    {
        std::vector<T> matrix_a;
        std::vector<T> matrix_b;
        std::vector<T> matrix_res;
        size_t start;
        size_t end;
        size_t order = 0;
    };
public:
    static int threads_num;

private:
    static void* mul_parallel(void *ptr_args)
    {
        auto *args = (struct MulMatrixArgs *) ptr_args;
        double start_time = get_time();

        T tmp = 0;
        for(size_t i = args->start; i < args->end; i++)
        {
            int count = 0;
            for(size_t k = 0; k < args->order; k++)
            {
                int inc_str = 0;
                for(size_t j = 0; j < args->order; j++)
                    tmp += args->matrix_a[i * args->order + inc_str++] * args->matrix_b[j + k * args->order];

                args->matrix_res[i * args->order + count++] = tmp;
                tmp = 0;
            }
        }
        printf("Time of one thread working: %fs\n", get_time() - start_time);
        pthread_exit(nullptr);
    }

public:
    explicit SquareMatrix(const char* filename)
        :
        order(0),
        data()
    {
        read_file(filename);
    }

    explicit SquareMatrix(const size_t order)
        :
        order(order),
        data(order * order, 0)
    {}

    T operator() (const size_t row, const size_t column) const
    {
        if (row >= order)
            throw std::out_of_range("Row is out of range: " + std::to_string(row));

        if (column >= order)
            throw std::out_of_range("Column is out of range: " + std::to_string(column));

        return data[row * order + column];
    }

    T& operator() (const size_t row, const size_t column)
    {
        if (row >= order)
            throw std::out_of_range("Row is out of range: " + std::to_string(row));

        if (column >= order)
            throw std::out_of_range("Column is out of range: " + std::to_string(column));

        return data[row * order + column];
    }

    friend SquareMatrix operator*(const SquareMatrix& a, const SquareMatrix& b)
    {
        if(threads_num < 1)
            throw std::runtime_error("Threads must be > 0:\n"
                                     "threads_num = " + std::to_string(threads_num));
        if(a.order != b.order)
            throw std::logic_error("SquareMatrix order mismatch:\n"
                                   "Order of matrix A = " + std::to_string(a.order) + '\n' +
                                   "Order of matrix B = " + std::to_string(b.order));

        SquareMatrix c(a.order);
        double start_time = get_time();

        /*
        if(threads_num == 1)
        {
            for (int i = 0; i < c.order; i++)
            {
                for (int j = 0; j < c.order; j++)
                {
                    for (int k = 0; k < c.order; k++)
                        c(i, j) += a(i, k) * b(k, j);
                }
            }
        }
        else
        {
         */
            std::vector<pthread_t> threads(threads_num);
            pthread_attr_t attr;
            struct MulMatrixArgs args;

            pthread_attr_init(&attr);

            args.matrix_a = a.data;
            args.matrix_b = b.data;
            args.matrix_res = c.data;
            args.order = a.order;

            size_t step = a.order / threads_num;
            size_t pos = 0;
            for(size_t k = 0; k < threads_num; k++)
            {
                args.start = pos;
                pos += step;
                args.end = (k == threads_num - 1) ? a.order : pos;

                if (pthread_create(&threads[k], &attr, mul_parallel, (void *)&args) != 0)
                {
                    fprintf(stderr, "Error creating thread\n");
                    exit(-1);
                }
            }
            pthread_attr_destroy(&attr);

            for(size_t k = 0; k < threads_num; k++)
                pthread_join(threads[k], nullptr);
        //}
        std::cout << "All time: " << get_time() - start_time << std::endl;
        return c;
    }

    friend std::ostream& operator<< (std::ostream& out, const SquareMatrix& matrix)
    {
        const size_t demo_size = (matrix.order > 10) ? 10 : matrix.order;
        for(size_t i = 0; i < demo_size; i++)
        {
            for (size_t j = 0; j < demo_size; j++)
                out << matrix(i, j) << ' ';

            out << '\n';
        }
        return out;
    }

    void read_file(const char* filename)
    {
        struct stat statistic{};
        stat(filename, &statistic);

        order = static_cast<size_t>(sqrt(static_cast<double>(statistic.st_size) / static_cast<double>(sizeof(T))));
        data.resize(order * order);

        int fp = check(open(filename, O_RDONLY));
        if (!fp)
            throw std::invalid_argument("Error opening file: " + std::string(filename));

        check(read(fp, data.data(), statistic.st_size));
        close(fp);
    }
};

template <typename T>
int SquareMatrix<T>::threads_num = 1;

void matrix_mul_main(const char* file_matrix_a, const char* file_matrix_b, const size_t threads_num = 1)
{
    SquareMatrix<T>::threads_num = threads_num;

    SquareMatrix<T> a(file_matrix_a);
    SquareMatrix<T> b(file_matrix_b);

    std::cout << "Matrix A:\n" << a << "\n\n";
    std::cout << "Matrix B:\n" << b << "\n\n";

    std::cout << "Performing Matrix C = A * B ...\n";
    SquareMatrix<T> c = a * b;
    std::cout << "Matrix C:\n" << c << "\n\n";
}
#endif