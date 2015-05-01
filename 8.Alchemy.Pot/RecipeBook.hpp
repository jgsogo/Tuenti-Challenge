
#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>

struct Recipe {
    std::size_t target;
    std::size_t value;
    std::set<std::size_t> compounds;
    Recipe(const std::size_t& target, const std::size_t& value) : target(target), value(value) {};
    };

struct RecipeBook {
    std::string filename;
    std::unordered_map<std::string, std::size_t> mapped_compounds;
    std::map<std::size_t, Recipe> recipes;

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
            auto target = mapped_compounds[element];
            auto it_recipe = recipes.insert(std::make_pair(target, Recipe(target, value)));
            for (auto source = elements.begin(); source != elements.end(); ++source) {
                it_recipe.first->second.compounds.insert(mapped_compounds[(*source)]);
                }
            }
            
        };

    };