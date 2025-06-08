#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <openssl/sha.h>

namespace http
{
    class RandomChar 
    {
        public:
            explicit RandomChar();
            std::string rand_str(int length);
        public:
            static const std::string chars;
        private:
            std::mt19937 rng;
    };
}
