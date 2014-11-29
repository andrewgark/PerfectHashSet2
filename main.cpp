#include <iostream>
#include "test.h"

typedef unsigned int ui32;
typedef unsigned long long ui64;

int main()
{
    ui32 typeTest, numTests, maxNumKeys, maxNumQueries;
    std::cout << "Enter type of test, count of tests, maximum of numbers of keys and maximum of number of queries\n";
    std::cin >> typeTest >> numTests >> maxNumKeys >> maxNumQueries;


    switch(typeTest)
    {
        case 0:
            RandomCorrectTest(numTests, maxNumKeys, maxNumQueries);
        break; //case 1:
        //    AllPermutationsTest(numTests, maxNumKeys, maxNumQueries);
        //break;
    }
    return 0;
}
