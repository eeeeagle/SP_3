#ifndef SQUARE_MATRIX
#define SQUARE_MATRIX

#include "check.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cmath>
#include <cstring>

using T = double;

template <typename T>
class SquareMatrix
{
private:
    std::vector<T> data;
    size_t order;
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
        if(a.order != b.order)
            throw std::logic_error("SquareMatrix order mismatch:\n"
                                   "Order of matrix A = " + std::to_string(a.order) + '\n' +
                                   "Order of matrix B = " + std::to_string(b.order));

        SquareMatrix c(a.order);

        for (int i = 0; i < c.order; i++)
        {
            for (int j = 0; j < c.order; j++)
            {
                for (int k = 0; k < c.order; k++)
                    c(i, j) += a(i, k) * b(k, j);
            }
        }

        return c;
    }

    friend std::ostream& operator<< (std::ostream& out, const SquareMatrix& matrix)
    {
        for(size_t i = 0; i < matrix.order; i++)
        {
            for (size_t j = 0; j < matrix.order; j++)
                out << matrix(i, j) << ' ';

            out << '\n';
        }
        return out;
    }

    void read_file(const char* filename)
    {
        struct stat statistic{};
        stat(filename, &statistic);

        order = static_cast<size_t>(sqrt(statistic.st_size / sizeof(T)));
        data.resize(order * order);

        int fp = check(open(filename, O_RDONLY));
        if (!fp)
            throw std::invalid_argument("Error opening file: " + std::string(filename));

        check(read(fp, data.data(), statistic.st_size));
        close(fp);
    }

    void write_file(const char* filename) const
    {
        int fp = check(open(filename, O_CREAT | O_WRONLY));
        if (!fp)
            throw std::invalid_argument("Error opening file: " + std::string(filename));

        check(write(fp, data.data(), data.size() * sizeof(T)));
        close(fp);
    }
};

#endif