#include <random>

typedef unsigned int ui32;
typedef unsigned long long ui64;

std::default_random_engine generator;

class Hash
{
private:
    static const ui64 p = 4294967311;
    ui64 a;
    ui64 b;
    ui32 size;

public:

    void genCoefficients()
    {
        std::uniform_int_distribution<ui64> distributionA(1, p - 1);
        std::uniform_int_distribution<ui64> distributionB(0, p - 1);

        a = distributionA(generator);
        b = distributionB(generator);

        //std::cout << " a = " << a << " b = " << b << std::endl;
    }


    ui32 getSize()
    {
        return size;
    }

    void setSize(ui32 s)
    {
        size = s;
    }

    ui32 operator()(ui32 &key) const
    {
        //std::cout << key << " --" << size << "- > ";
        //std::cout << (((ui64)((a >> 32) * (ui64)key) % p + (ui64)((a & UINT_MAX) * (ui64)key) % p + b) % p) % size << "\n";
        return (((ui64)((a >> 32) * (ui64)key) % p + (ui64)((a & UINT_MAX) * (ui64)key) % p + b) % p) % size;
    }
};
