#include "SquareMatrix.hpp"
#include <getopt.h>
#include <cstring>

int main(int argc, char* argv[])
{
    int threads = 1;
    int value = 0;
    std::string filename[2] = {"", ""};

    const option options[] = {{"matrix_a", required_argument, nullptr, 'a'},
                              {"matrix_b", required_argument, nullptr, 'b'},
                              {   "value", required_argument, nullptr, 'v'},
                              { "threads", required_argument, nullptr, 't'},
                              {    "help", required_argument, nullptr, 'h'},
                              {   nullptr,       no_argument, nullptr,  0 }};

    for(int i; (i = getopt_long(argc, argv, "n:c:", options, nullptr)) != -1; )
    {
        switch(i)
        {
            case 'a':
                filename[0] = optarg;
                break;
            case 'b':
                filename[1] = optarg;
                break;
            case 'v':
                value = std::stoi(optarg);
                break;
            case 't':
                threads = std::stoi(optarg);
                break;
            case 'h':
                std::cout   << "Arguments list:\n"
                            << "    --matrix_a (-a) [filename]    (optional)\n"
                            << "    --matrix_b (-b) [filename]    (optional)\n"
                            << "    --value    (-v) [value]       (optional)\n"
                            << "    --threads  (-t) [value]       (optional)\n\n"
                            << "matrix_a, matrix_b\n"
                            << "    Locate binary file with matrix\n\n"
                            << "value\n"
                            << "    Set value to find in array\n\n"
                            << "threads\n"
                            << "    Set number of threads for parallel calculations\n\n"
                            << "EXAMPLE:\n"
                            << "    ./SP_3 -a mat1.bin --matrix_b long_long_path/mat2.bin -v 50 --threads 4\n";
                exit(EXIT_FAILURE);
            default:
                std::cout   << "Unknown argument detected\n"
                            << "Use [--help] or [-h] argument to get launch information\n";
                exit(EXIT_FAILURE);
        }
    }

    SquareMatrix<T> a(filename[0].c_str());
    SquareMatrix<T> b(filename[1].c_str());

    std::cout << "Matrix A:\n" << a << '\n';
    std::cout << "Matrix B:\n" << b << '\n';

    SquareMatrix<T> c = a * b;

    return 0;
}
