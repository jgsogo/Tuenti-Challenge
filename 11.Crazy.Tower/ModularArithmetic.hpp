
#pragma once

#include <cassert>
#include <unordered_map>
#include <tuple>


template <typename INT_TYPE = std::uint32_t>
struct ModularArithmetic {
    // Cache for modular inverse
    static std::unordered_map<INT_TYPE, std::unordered_map<INT_TYPE, INT_TYPE>> modular_inverse_cache;
    // Implementation of modular arithmetic
    static INT_TYPE combinations(const INT_TYPE& m, const INT_TYPE& n, const INT_TYPE& modulus);
    static INT_TYPE multiply(const INT_TYPE& lhs, const INT_TYPE& rhs, const INT_TYPE& modulus);
    static INT_TYPE modular_inverse(const INT_TYPE& n, const INT_TYPE& modulus);
    static INT_TYPE division(const INT_TYPE& dividend, const INT_TYPE& divisor, const INT_TYPE& modulus);
    static INT_TYPE sum(const INT_TYPE& lhs, const INT_TYPE& rhs, const INT_TYPE& modulus);
    };
template <typename INT_TYPE>
std::unordered_map<INT_TYPE, std::unordered_map<INT_TYPE, INT_TYPE>> ModularArithmetic<INT_TYPE>::modular_inverse_cache;

/* Arithmetics specialization for type 2^32 
* Using 2^64 to store the results, I will always be able to multiply two numbers: (2^32-1)^2 < (2^64 -1)    
*
*/
template <>
std::uint32_t ModularArithmetic<std::uint32_t>::multiply(const std::uint32_t& lhs, const std::uint32_t& rhs, const std::uint32_t& modulo) {
    auto product = static_cast<std::uint64_t>(lhs)*static_cast<std::uint64_t>(rhs);
    return (product%modulo);
    }

template <>
std::uint32_t ModularArithmetic<std::uint32_t>::sum(const std::uint32_t& lhs, const std::uint32_t& rhs, const std::uint32_t& modulo) {
    auto sum = static_cast<std::uint64_t>(lhs)+static_cast<std::uint64_t>(rhs);
    return (sum%modulo);
    }

template <>
std::uint32_t ModularArithmetic<std::uint32_t>::modular_inverse(const std::uint32_t& n, const std::uint32_t& modulo) {
    /*
    auto modulo_map = modular_inverse_cache.emplace(std::make_pair(modulo, std::unordered_map<std::uint32_t, std::uint32_t>()));
    auto found = modulo_map.first->second.emplace(std::make_pair(n, 0));
    if (!found.second) {
        return found.first->second;
        }
    auto number = n%modulo;
    */
    std::uint32_t inv_divisor = 0;
    // TODO: Check 'n' and 'modulo' to be coprimes (always true if 'modulo' is prime and 'n<modulo')
    // Naïve implementation
    //for(auto i=1; i<modulo; i++) {
    //    if (ModularArithmetic<std::uint32_t>::multiply(number, i, modulo) == 1) {
    //        inv_divisor = i;
    //        break;
    //        }
    //    }
    // Extended_Euclidean_algorithm: http://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Computing_multiplicative_inverses_in_modular_structures
    std::int64_t t =0, r=modulo, newt=1, newr=n; // TODO: Don't need up to 2^64 (it depends on 'modulo' value)
    while  (newr != 0) {
        auto quotient = r / newr;
        std::tie(t, newt) = std::make_pair(newt, t-quotient*newt);
        std::tie(r, newr) = std::make_pair(newr, r-quotient*newr);
        }
    if (r>1) {
        std::cerr << "Number '" << n << "' is not invertible (modulus '" << modulo << "')" << std::endl;
        }
    if (t<0) { t = t + modulo;}
    inv_divisor = t; 

    //found.first->second = inv_divisor;
    return inv_divisor;
    }

template <>
std::uint32_t ModularArithmetic<std::uint32_t>::division(const std::uint32_t& dividend, const std::uint32_t& divisor, const std::uint32_t& modulo) {
    // ! There is no division in modular arithmetics... Need to use "modular inverse"
    auto inv_divisor = ModularArithmetic<std::uint32_t>::modular_inverse(divisor, modulo);
    return ModularArithmetic<std::uint32_t>::multiply(dividend, inv_divisor, modulo);
    }

template <>
std::uint32_t ModularArithmetic<std::uint32_t>::combinations(const std::uint32_t& m, const std::uint32_t& n, const std::uint32_t& modulo) {
    // Combinations of n elements from m::  C_{m,n} = m!/(n!(m-n)!)
    assert(m>=n);
    std::uint32_t ret = 1LL;
    if (n!=0) {
        auto max_ = std::max(n, m-n);
        for(auto i=m; i>max_; --i) {
            ret = ModularArithmetic<std::uint32_t>::multiply(ret, i, modulo);
            }
        for(auto i=std::min(n, m-n); i>1; --i) {
            ret = ModularArithmetic<std::uint32_t>::division(ret, i, modulo);
            }
        }
    return ret;
    }

