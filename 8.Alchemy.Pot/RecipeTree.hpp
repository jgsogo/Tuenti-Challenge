
#pragma once

#include "RecipeBook.hpp"


struct Node {
    std::size_t compound;
    std::unordered_map<std::size_t, Node*> leaves;
    
    Node(const std::size_t& cmp=0) : compound(cmp) {};
    void build(Recipe::t_ingredients::const_iterator it_begin, Recipe::t_ingredients::const_iterator it_end, std::size_t target) {
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
    void retrieve(Recipe::t_ingredients::const_iterator it_begin, Recipe::t_ingredients::const_iterator it_end, std::vector<std::vector<std::size_t>>& results) {
        if (compound != 0) {
            #ifdef DEBUG
            std::cout << " = " << compound << std::endl;
            #endif
            auto r = results.insert(results.end(), std::vector<std::size_t>(it_begin, it_end));
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

struct RecipeTree {
    const RecipeBook& book;
    Node root;

    RecipeTree(const RecipeBook& recipes) : book(recipes) {};

    void build() {
        for (auto it = book.recipes.begin(); it!=book.recipes.end(); ++it) {
            // Recipe ingredients are ordered
            if (it->second.ingredients.size()) {
                root.build(it->second.ingredients.begin(), it->second.ingredients.end(), it->second.target);
                }
            }
        };

    std::size_t retrieve(const std::vector<std::size_t>& ingredients, std::vector<std::vector<std::size_t>>& results) {
        if (ingredients.size()<2) {
            return 0;
            }
        //std::cout<< std::endl << ">>>>>>> Search for: " << book.translate_ingredients(ingredients) << std::endl;

        // Search from the beginning 
        std::vector<std::vector<std::size_t>> partial1;
        root.retrieve(ingredients.begin(), ingredients.end(), partial1);

        // Search removing one of the ingredients (recursiveness will go for the rest)
        std::vector<std::vector<std::size_t>> partial2;
        for (auto it = ingredients.begin(); it!=ingredients.end()-1; it=std::upper_bound(it, ingredients.end()-1, *it)) {
            std::vector<std::vector<std::size_t>> partial2_inside;
            std::vector<std::size_t> pp(ingredients.begin(), it);
            pp.insert(pp.end(), it+1, ingredients.end());
            this->retrieve(pp, partial2_inside);

            for (auto it_ = partial2_inside.begin(); it_!=partial2_inside.end(); ++it_) {
                it_->insert(std::upper_bound(it_->begin(), it_->end(), *it), *it);
                partial2.insert(partial2.end(), *it_);
                }
            }
        
        // Search using the new discovered formulas
        std::vector<std::vector<std::size_t>> partial3;
        for (auto it = partial1.begin(); it!= partial1.end(); ++it) {
            retrieve(*it, partial3);
            }
        for (auto it = partial2.begin(); it!= partial2.end(); ++it) {
            retrieve(*it, partial3);
            }

        // Append it all
        results.insert(results.end(), partial1.begin(), partial1.end());
        results.insert(results.end(), partial2.begin(), partial2.end());
        results.insert(results.end(), partial3.begin(), partial3.end());

        return results.size();
        }
    };