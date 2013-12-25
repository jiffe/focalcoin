#include <cstdlib>
#include <iostream>
#include <stdint.h>

#include "uint256.h"


// Bitwise Operators
template <typename T> T operator&(T lhs, uint256_t rhs){
    T out = lhs & (T) rhs.lower();
    return out;
}

template <typename T> T operator|(T lhs, uint256_t rhs){
    T out = lhs | (T) rhs.lower();
    return out;
}

template <typename T> T operator^(T lhs, uint256_t rhs){
    T out = lhs ^ (T) rhs.lower();
    return out;
}

template <typename T> T operator&=(T & lhs, uint256_t rhs){
    lhs &= (T) rhs.lower();
    return lhs;
}

template <typename T> T operator|=(T & lhs, uint256_t rhs){
    lhs |= (T) rhs.lower();
    return lhs;
}

template <typename T> T operator^=(T & lhs, uint256_t rhs){
    lhs ^= (T) rhs.lower();
    return lhs;
}

// Comparison Operators
template <typename T> bool operator==(T lhs, uint256_t rhs){
    return (!rhs.upper() && (uint128_t) lhs == rhs.lower());
}

template <typename T> bool operator!=(T lhs, uint256_t rhs){
    return (rhs.upper() | ((uint128_t) lhs != rhs.lower()));
}

template <typename T> bool operator>(T lhs, uint256_t rhs){
    if (rhs.upper())
        return false;
    return ((uint128_t) lhs > rhs.lower());
}

template <typename T> bool operator<(T lhs, uint256_t rhs){
    if (rhs.upper())
        return true;
    return ((uint128_t) lhs < rhs.lower());
}

template <typename T> bool operator>=(T lhs, uint256_t rhs){
    if (rhs.upper())
        return false;
    return ((uint128_t) lhs >= rhs.lower());
}

template <typename T> bool operator<=(T lhs, uint256_t rhs){
    if (rhs.upper())
        return true;
    return ((uint128_t) lhs <= rhs.lower());
}

// Arithmetic Operators
template <typename T> T operator+(T lhs, uint256_t rhs){
    return (T) (rhs + lhs);
}

template <typename T> T & operator+=(T & lhs, uint256_t rhs){
    lhs = (T) (rhs + lhs);
    return lhs;
}

template <typename T> T operator-(T lhs, uint256_t rhs){
    return (T) (rhs - lhs);
}

template <typename T> T & operator-=(T & lhs, uint256_t rhs){
    lhs = (T) (rhs - lhs);
    return lhs;
}

template <typename T> T operator*(T lhs, uint256_t rhs){
    return lhs * rhs.lower();
}

template <typename T> T & operator*=(T & lhs, uint256_t rhs){
    lhs = (T) (rhs.lower() * lhs);
    return lhs;
}

template <typename T> T operator/(T lhs, uint256_t rhs){
    return (T) (uint256_t(lhs) / rhs);
}

template <typename T> T & operator/=(T & lhs, uint256_t rhs){
    lhs = (T) (uint256_t(lhs) / rhs);
    return lhs;
}

template <typename T> T operator%(T lhs, uint256_t rhs){
    return (T) (uint256_t(lhs) % rhs);
}

template <typename T> T & operator%=(T & lhs, uint256_t rhs){
    lhs = (T) (uint256_t(lhs) % rhs);
    return lhs;
}

// IO Operator

std::ostream & operator<<(std::ostream & stream, uint256_t rhs){
    std::string out = "";
    if (rhs == 0)
        out = "0";
    else {
        int div;
        if (stream.flags() & stream.oct)
            div = 8;
        if (stream.flags() & stream.dec)
            div = 10;
        if (stream.flags() & stream.hex)
            div = 16;
        while (rhs > 0){
            out = "0123456789abcdef"[size_t(rhs % div)] + out;
            rhs /= div;
        }
    }
    stream << out;
    return stream;
}
