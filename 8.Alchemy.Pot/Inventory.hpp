
#pragma once

#include <algorithm>

#include "RecipeBook.hpp"

struct Inventory {
    const RecipeBook& recipes;

    Inventory(const RecipeBook& recipes) : recipes(recipes) {};

    std::size_t gold_value(const std::vector<std::size_t>& items) {
        return std::accumulate(items.begin(), items.end(), std::size_t(0), [this](const std::size_t& a, std::size_t b){
            return a + recipes.gold_value.find(b)->second;
            });
        };
    };