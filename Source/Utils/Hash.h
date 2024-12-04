#ifndef UTILS_HASH_H
#define UTILS_HASH_H

#include <string>
#include <string_view>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>

namespace Hash
{   
    // fnv1a hashing algorithm
    constexpr uint64_t FastHash(const std::string_view &data)
    {
        std::size_t prime;
        std::size_t offset_basis;

        if constexpr (sizeof(std::size_t) == 8)
        {
            prime		 = 1099511628211ULL;
            offset_basis = 14695981039346656037ULL;
        }
        else
        {
            prime		 = 16777619U;
            offset_basis = 2166136261U;
        }

        uint64_t hash{offset_basis};
        for (auto &c : data)
        {
            hash ^= c;
            hash *= prime;
        }

        return hash;
    }
}

constexpr uint64_t operator"" _fh(const char *str, std::size_t len)
{
	return Hash::FastHash(std::string_view{str, len});
}

#endif	// UTILS_HASH_H