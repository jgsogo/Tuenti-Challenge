
#pragma once

#include "RecipeBook.hpp"
#include <cassert>


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
    void retrieve(Recipe::t_ingredients::const_iterator it_begin, Recipe::t_ingredients::const_iterator it_end, std::unordered_map<std::size_t, std::pair<Recipe::t_ingredients::const_iterator, Recipe::t_ingredients::const_iterator>>& targets) {
        if (compound != 0) {
            #ifdef DEBUG
            std::cout << " = " << compound << std::endl;
            #endif
            targets.insert(std::make_pair(compound, std::make_pair(it_begin, it_end)));
            }
        if (it_begin != it_end) {
            auto it = leaves.find(*it_begin);
            if (it != leaves.end()) {
                #ifdef DEBUG
                std::cout << " " << it->first;
                #endif
                it->second->retrieve(it_begin+1, it_end, targets);
                }
            }
        };
    };

struct RecipeTree {
    const RecipeBook& book;

    // Tree structure    
    Node root;

    RecipeTree(const RecipeBook& recipes) : book(recipes) {};
    void build() {
        for (auto it = book.recipes.begin(); it!=book.recipes.end(); ++it) {
            // Recipe ingredients are ordered
            if (it->second.ingredients.size()) {
                root.build(it->second.ingredients.begin(), it->second.ingredients.end(), it->second.target);
                }
            else {
                //auto leaf = root.leaves.insert(std::make_pair(it->second.target, new Node(it->second.target)));
                //assert(leaf.second);
                }
            }
        };

    int retrieve(const std::vector<std::size_t>& ingredients, std::vector<std::vector<std::size_t>>& results) {
        std::unordered_map<std::size_t, std::pair<Recipe::t_ingredients::const_iterator, Recipe::t_ingredients::const_iterator>> targets;
        root.retrieve(ingredients.begin(), ingredients.end(), targets);

        for (auto it = targets.begin(); it!=targets.end(); ++it) {
            // Insert discovered
            auto r = results.insert(results.end(), std::vector<std::size_t>(it->second.first, it->second.second));
            (*r).push_back(it->first);
            std::sort((*r).begin(), (*r).end());

            // Search for more
            retrieve(*r, results);
            }
        return targets.size();
        }
    };