
#pragma once

#include <cassert>

template <typename INT_TYPE = std::uint32_t>
struct ModularArithmetic {
    // Implementation of modular arithmetic
    static INT_TYPE combinations(const INT_TYPE& m, const INT_TYPE& n, const INT_TYPE& modulus);
    static INT_TYPE multiply(const INT_TYPE& lhs, const INT_TYPE& rhs, const INT_TYPE& modulus);
    static INT_TYPE sum(const INT_TYPE& lhs, const INT_TYPE& rhs, const INT_TYPE& modulus);
    };

/* Arithmetics specialization for type 2^32 */
// Using 2^64 to store the results, I will always be able to multiply two numbers: (2^32-1)^2 < (2^64 -1)    
template <>
std::uint32_t ModularArithmetic<std::uint32_t>::multiply(const std::uint32_t& lhs, const std::uint32_t& rhs, const std::uint32_t& modulo) {
    return (lhs*rhs)%modulo;
    }

template <>
std::uint32_t ModularArithmetic<std::uint32_t>::sum(const std::uint32_t& lhs, const std::uint32_t& rhs, const std::uint32_t& modulo) {
    return (lhs%modulo)+(rhs%modulo);
    }
    
template <>
std::uint32_t ModularArithmetic<std::uint32_t>::combinations(const std::uint32_t& m, const std::uint32_t& n, const std::uint32_t& modulo) {
    // Combinations of n elements from m::  C_{m,n} = m!/(n!(m-n)!)
    assert(m>=n);
    std::uint64_t ret = 1LL;
    if (n!=0) {
        auto max_ = std::max(n, m-n);
        for(auto i=m; i>max_; --i) {
            ret = ModularArithmetic<std::uint32_t>::multiply(ret, i, modulo);
            }
        for(auto i=std::min(n, m-n); i>1; --i) {
            ret /= i;
            }
        }
    return ret;
    }

