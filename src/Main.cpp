#include "SquareMatrix.hpp"
#include "FindValue.hpp"
#include <getopt.h>



int main(int argc, char* argv[])
{
    int threads_num = 0;
    int value = -1;
    std::string file_matrix[2] = {"", ""};

    const option options[] = {{"matrix_a", required_argument, nullptr, 'a'},
                              {"matrix_b", required_argument, nullptr, 'b'},
                              {   "value", required_argument, nullptr, 'v'},
                              { "threads", required_argument, nullptr, 't'},
                              {    "help", required_argument, nullptr, 'h'},
                              {   nullptr,       no_argument, nullptr,  0 }};

    for (int i; (i = getopt_long(argc, argv, "a:b:v:t:h:", options, nullptr)) != -1; )
    {
        switch(i)
        {
            case 'a':
                file_matrix[0] = optarg;
                break;
            case 'b':
                file_matrix[1] = optarg;
                break;
            case 'v':
                value = std::stoi(optarg);
                break;
            case 't':
                threads_num = std::stoi(optarg);
                break;
            case 'h':
                std::cout   << "Arguments list:\n"
                            << "    --matrix_a (-a) [file_matrix]    (optional)\n"
                            << "    --matrix_b (-b) [file_matrix]    (optional)\n"
                            << "    --value    (-v) [value]       (optional)\n"
                            << "    --threads_num  (-t) [value]       (optional)\n\n"
                            << "matrix_a, matrix_b\n"
                            << "    Locate binary file with matrix\n\n"
                            << "value\n"
                            << "    Set value to find in array\n\n"
                            << "threads_num\n"
                            << "    Set number of threads_num for parallel calculations\n\n"
                            << "EXAMPLE:\n"
                            << "    ./SP_3 -a mat1.bin --matrix_b long_long_path/mat2.bin -v 50 --threads_num 4\n";
                exit(EXIT_SUCCESS);
            default:
                std::cout   << "Unknown argument detected\n"
                            << "Use [--help] or [-h] argument to get launch information\n";
                exit(EXIT_FAILURE);
        }
    }

    int buf;
    for(bool flag = true; flag; )
    {

        std::cout   << "[1] Matrix sequential multiplication\n"
                    << "[2] Matrix parallel multiplication\n"
                    << "[3] Find value sequentially\n"
                    << "[4] Find value parallely\n"
                    << "[0] Exit\n\n"
                    << "Enter field: ";
        std::cin >> buf;

        system("clear");
        switch(buf)
        {
            case 0:
                flag = false;
                break;
            case 1:
                matrix_mul_main(file_matrix[0].c_str(), file_matrix[0].c_str());
                break;
            case 2:
                matrix_mul_main(file_matrix[0].c_str(), file_matrix[0].c_str(), threads_num);
                break;
            case 3:
                find_value_main(value);
                break;
            case 4:
                find_value_main(value, threads_num);
                break;
            default:
                std::cout << "Wrong option! Try again.\n";
                break;
        }
    }

    return 0;
}
