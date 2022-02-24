#include <string>

template <typename T>
std::string
format_integer (T pNum)
{

    T           cpy {pNum};
    int32_t     len {};

    std::string res;

    while (cpy) {
        ++len, cpy /= 10;
    }

    for (int32_t i = 0, d; i < len; ++i) {

        d = pNum % 10;
        pNum /= 10;

        res += (char) (d + '0');
        if (i % 3 == 2 and i != len - 1) {
            res += ',';
        }
    }

    std::reverse (res.begin (), res.end ());

    return res;
}