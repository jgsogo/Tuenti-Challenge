
#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>

typedef std::uint32_t t_ingredient;
typedef std::vector<t_ingredient> t_ingredients;

struct Recipe {
    t_ingredient target;
    std::size_t value;
    t_ingredients ingredients; // Can be more than one of the same type
    Recipe(const t_ingredient& target, const std::size_t& value) : target(target), value(value) {};
    };

struct RecipeBook {
    std::string filename;
    // Recipes storage
    std::map<t_ingredient, Recipe> recipes;
    // Quick retrieval containers
    static t_ingredient next_id;
    std::unordered_map<std::string, t_ingredient> mapped_compounds; // <compound_name, compound_id>
    std::unordered_map<t_ingredient, std::size_t> gold_value; // <compound_id, gold_value>
    #ifdef DEBUG
        std::unordered_map<t_ingredient, std::string> translated_keys; // DEBUG ONLY!
    #endif

    RecipeBook(const std::string& filename) : filename(filename) {};
    int read() {
        std::ifstream file(filename.c_str());
        if (!file.is_open()) {
            std::cerr << "Cannot read RecipeBook file: '" << filename << "'" << std::endl;
            return -1;
            }

        std::string line;
        std::string element;
        std::size_t value;
        while(std::getline(file, line)) {
            std::stringstream ss(line);
            ss >> element >> value;

            std::istream_iterator<std::string> it(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> elements(it, end);

            // Build recipe
            auto target = next_id++;
            mapped_compounds[element] = target;
            #ifdef DEBUG
            translated_keys[target] = element;
            #endif
            gold_value[target] = value;
            auto it_recipe = recipes.insert(std::make_pair(target, Recipe(target, value)));
            for (auto source = elements.begin(); source != elements.end(); ++source) {
                it_recipe.first->second.ingredients.push_back(mapped_compounds[(*source)]);
                }
            std::sort(it_recipe.first->second.ingredients.begin(), it_recipe.first->second.ingredients.end());
            }

        };

    t_ingredients parse_ingredients(const std::string& line) const {
        std::stringstream ss(line);
        std::istream_iterator<std::string> it(ss);
        std::istream_iterator<std::string> end;

        t_ingredients ret;
        while (it!=end) {
            ret.push_back(mapped_compounds.find(*it)->second);
            it++;
            }
        std::sort(ret.begin(), ret.end());
        return ret;
        };

    #ifdef DEBUG
    std::string translate_ingredients(const t_ingredients& ingredients) const {
        std::stringstream ss;
        for (auto it = ingredients.begin(); it!=ingredients.end(); ++it) {
            ss << " | " << translated_keys.find(*it)->second;
            }
        return ss.str();
        };
    #endif

    std::size_t value(const t_ingredients& items) const {
        return std::accumulate(items.begin(), items.end(), std::size_t(0), [this](const std::size_t& a, t_ingredient b){
            return a + gold_value.find(b)->second;
            });
        };        
    
    };
t_ingredient RecipeBook::next_id = 1; // Value 0 is reserved for NONE