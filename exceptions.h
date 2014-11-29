#include <cassert>
#include <sstream>

template<typename TypeValue>
std::string convertToString(TypeValue number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}


class ImpossibleKeyException: public std::exception
{
    std::string message;
public:
    ui32 whichKey;
    explicit ImpossibleKeyException(ui32 key) : whichKey(key)
    {
        message = "Key " + convertToString<ui32>(key) + " is impossible\n";
    }
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
};

class EqualKeysException: public std::exception
{
    std::string message;
public:
    ui32 whichKey;
    explicit EqualKeysException(ui32 key) : whichKey(key)
    {
        message = "Key " + convertToString<ui32>(key) + " is in set more than one time\n";
    }
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
};
