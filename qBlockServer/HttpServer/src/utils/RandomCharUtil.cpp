#include "../../include/utils/RandomCharUtil.h"


namespace http
{
    const std::string RandomChar::chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()`~-_=+[{]{|;:,<.>?";


    RandomChar::RandomChar() : rng(std::random_device{}()) {}
    std::string RandomChar::rand_str(int length) 
    {
        std::string output;
        output.reserve(length);

        while(length>0)
        {
            auto randNumb = rng();
            int charLength = chars.size();
            while(randNumb > charLength && length--)
            {
                output.push_back(chars[randNumb%charLength]);
                randNumb/=charLength;
            }
        }
        return output;
    }
} // namespace http
