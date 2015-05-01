
#pragma once

#include "RecipeBook.hpp"


struct Node {
    t_ingredient compound;
    std::unordered_map<t_ingredient, Node*> leaves;
    
    Node(const t_ingredient& cmp=0) : compound(cmp) {};
    void build(t_ingredients::const_iterator it_begin, t_ingredients::const_iterator it_end, t_ingredient target) {
        if (it_begin != it_end) {
            auto leaf = leaves.insert(std::make_pair((*it_begin), (Node*)0));
            if (leaf.second) {
                leaf.first->second = new Node();
                }
            leaf.first->second->build(++it_begin, it_end, target);
            }
        else {
            compound = target;
            }
        };
    void retrieve(t_ingredients::const_iterator it_begin, t_ingredients::const_iterator it_end, std::vector<t_ingredients>& results) {
        if (compound != 0) {
            #ifdef DEBUG
            std::cout << " = " << compound << std::endl;
            #endif
            auto r = results.insert(results.end(), t_ingredients(it_begin, it_end));
            r->insert(r->end(), compound);
            }
        else {
            if (it_begin != it_end) {
                auto it = leaves.find(*it_begin);
                if (it != leaves.end()) {
                    #ifdef DEBUG
                    std::cout << " " << it->first;
                    #endif
                    it->second->retrieve(it_begin+1, it_end, results);
                    }
                }
            }
        };
    };


// Hash function borrowed from here (http://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector)
std::size_t hash(t_ingredients const& vec) {
    std::size_t seed = 0;
    for(auto& i : vec) {
        seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    return seed;
    }


struct RecipeTree {
    const RecipeBook& book;
    Node root;
    std::unordered_map<std::size_t, std::vector<t_ingredients>> cache;

    RecipeTree(const RecipeBook& recipes) : book(recipes) {};

    void build() {
        for (auto it = book.recipes.begin(); it!=book.recipes.end(); ++it) {
            // Recipe ingredients are ordered
            if (it->second.ingredients.size()) {
                root.build(it->second.ingredients.begin(), it->second.ingredients.end(), it->second.target);
                }
            }
        };

    std::size_t retrieve(const t_ingredients& ingredients, std::vector<t_ingredients>& results) {
        if (ingredients.size()<2) {
            return 0;
            }            
        //std::cout<< std::endl << ">>>>>>> Search for: " << book.translate_ingredients(ingredients) << std::endl;
        auto hashed = hash(ingredients);
        auto cached = cache.find(hashed);
        if (cached != cache.end()) {
            results.insert(results.end(), cached->second.begin(), cached->second.end());
            return cached->second.size();
            }

        // Search from the beginning 
        std::vector<t_ingredients> partial1;
        root.retrieve(ingredients.begin(), ingredients.end(), partial1);

        // Search removing one of the ingredients (recursiveness will go for the rest)
        std::vector<t_ingredients> partial2;
        for (auto it = ingredients.begin(); it!=ingredients.end()-1; it=std::upper_bound(it, ingredients.end()-1, *it)) {
            std::vector<t_ingredients> partial2_inside;
            t_ingredients pp(ingredients.begin(), it);
            pp.insert(pp.end(), it+1, ingredients.end());
            this->retrieve(pp, partial2_inside);

            for (auto it_ = partial2_inside.begin(); it_!=partial2_inside.end(); ++it_) {
                it_->insert(std::upper_bound(it_->begin(), it_->end(), *it), *it);
                partial2.insert(partial2.end(), *it_);
                }
            }
        
        // Search using the new discovered formulas
        std::vector<t_ingredients> partial3;
        for (auto it = partial1.begin(); it!= partial1.end(); ++it) {
            retrieve(*it, partial3);
            }
        for (auto it = partial2.begin(); it!= partial2.end(); ++it) {
            retrieve(*it, partial3);
            }

        // Depending on the number of ingredients, do cache or not (mem problem)
        if (ingredients.size() > 5) {
            auto data = cache.insert(std::make_pair(hashed, std::vector<t_ingredients>()));

            // Append it all
            data.first->second.insert(data.first->second.end(), partial1.begin(), partial1.end());
            data.first->second.insert(data.first->second.end(), partial2.begin(), partial2.end());
            data.first->second.insert(data.first->second.end(), partial3.begin(), partial3.end());

            results.insert(results.end(), data.first->second.begin(), data.first->second.end());
            }
        else {
            results.insert(results.end(), partial1.begin(), partial1.end());
            results.insert(results.end(), partial2.begin(), partial2.end());
            results.insert(results.end(), partial3.begin(), partial3.end());
            }

        return results.size();
        }
    };