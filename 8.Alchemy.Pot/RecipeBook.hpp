
#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>

struct Recipe {
    typedef std::vector<std::size_t> t_ingredients;
    std::size_t target;
    std::size_t value;
    t_ingredients ingredients; // Can be more than one of the same type
    Recipe(const std::size_t& target, const std::size_t& value) : target(target), value(value) {};
    };

struct RecipeBook {
    std::string filename;
    // Recipes storage
    std::map<std::size_t, Recipe> recipes;
    // Quick retrieval containers
    static std::size_t next_id;
    std::unordered_map<std::string, std::size_t> mapped_compounds; // <compound_name, compound_id>
    std::unordered_map<std::size_t, std::size_t> gold_value; // <compound_id, gold_value>

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
            gold_value[target] = value;
            auto it_recipe = recipes.insert(std::make_pair(target, Recipe(target, value)));
            for (auto source = elements.begin(); source != elements.end(); ++source) {
                it_recipe.first->second.ingredients.push_back(mapped_compounds[(*source)]);
                }
            std::sort(it_recipe.first->second.ingredients.begin(), it_recipe.first->second.ingredients.end());
            }

        };

    std::vector<std::size_t> parse_ingredients(const std::string& line) {
        std::stringstream ss(line);
        std::istream_iterator<std::string> it(ss);
        std::istream_iterator<std::string> end;

        std::vector<std::size_t> ret;
        while (it!=end) {
            ret.push_back(mapped_compounds[(*it)]);
            it++;
            }
        std::sort(ret.begin(), ret.end());
        return ret;
        };
    
    std::size_t value(const std::vector<std::size_t>& items) {
        return std::accumulate(items.begin(), items.end(), std::size_t(0), [this](const std::size_t& a, std::size_t b){
            return a + gold_value.find(b)->second;
            });
        };        
    
    };
std::size_t RecipeBook::next_id = 1; // Value 0 is reserved for NONE