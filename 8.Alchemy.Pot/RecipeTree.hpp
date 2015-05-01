
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
std::size_t hash_it(t_ingredients const& vec) {
    std::size_t seed = 0;
    for(auto& i : vec) {
        seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    return seed;
    }


struct RecipeTree {
    const RecipeBook& book;
    Node root;
    std::unordered_map<std::size_t, std::unordered_map<std::size_t, t_ingredients>> cache;

    RecipeTree(const RecipeBook& recipes) : book(recipes) {};

    void build() {
        for (auto it = book.recipes.begin(); it!=book.recipes.end(); ++it) {
            // Recipe ingredients are ordered
            if (it->second.ingredients.size()) {
                root.build(it->second.ingredients.begin(), it->second.ingredients.end(), it->second.target);
                }
            }
        };

    bool is_cached(const std::size_t& hash) const {
        return (cache.find(hash) != cache.end());
        };
    const std::unordered_map<std::size_t, t_ingredients>& get_from_cache(const std::size_t& hash) const {
        //std::cout << ".";
        return cache.find(hash)->second;
        };
    void set_cache(const std::size_t& hash, const std::unordered_map<std::size_t, t_ingredients>& data) {
        cache[hash] = data;
        //std::cout << "+";
        };
    /*
    void do_cache(const t_ingredients& ingredients, const std::unordered_map<std::size_t, t_ingredients>& data) {
        auto hash = hash_it(ingredients);
        set_cache(hash, data);
        };
    */

    std::size_t retrieve(const t_ingredients& ingredients, std::vector<t_ingredients>& results) {
        if (ingredients.size()<2) {
            return 0;
            }
        #ifdef DEBUG
        std::cout<< std::endl << ">> Search for (" << results.size() << "): " << book.translate_ingredients(ingredients) << std::endl;
        #endif
        auto hash = hash_it(ingredients);
        if (is_cached(hash)) {
            auto data = get_from_cache(hash);
            for (auto it = data.begin(); it!=data.end(); ++it) {
                results.insert(results.end(), it->second);
                }
            return results.size();
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
            retrieve(pp, partial2_inside);

            for (auto it_ = partial2_inside.begin(); it_!=partial2_inside.end(); ++it_) {
                it_->insert(std::upper_bound(it_->begin(), it_->end(), *it), *it);
                partial2.insert(partial2.end(), *it_);
                }
            }
        
        // Search using the new discovered formulas ()
        std::vector<t_ingredients> partial3;
        for (auto it = partial1.begin(); it!= partial1.end(); ++it) {
            retrieve(*it, partial3);
            }
        for (auto it = partial2.begin(); it!= partial2.end(); ++it) {
            retrieve(*it, partial3);
            }

        // Append all results and cache.
        std::unordered_map<std::size_t, t_ingredients> total;
        for (auto it = partial1.begin(); it!=partial1.end(); ++it) {
            total.insert(std::make_pair(hash_it(*it), *it));
            }
        for (auto it = partial2.begin(); it!=partial2.end(); ++it) {
            total.insert(std::make_pair(hash_it(*it), *it));
            }
        for (auto it = partial3.begin(); it!=partial3.end(); ++it) {
            total.insert(std::make_pair(hash_it(*it), *it));
            }
        set_cache(hash, total);

        for (auto it = total.begin(); it!=total.end(); ++it) {
            results.insert(results.end(), it->second);
            }
        return results.size();
        }
    };