
#pragma once

#include "Girl.hpp"

// DFS Visitor to compute components in a graph
struct GirlVisitor {
    std::map<std::size_t, std::size_t> components; // stores <component_id, number of girls who likes shopping in that component>
    std::map<std::size_t, std::size_t>::iterator it_cmp;
    std::size_t cmp_id;
    std::size_t total_shoppers;

    GirlVisitor() : cmp_id(0), total_shoppers(0) {};
    void compute(std::map<std::string, pGirl>& girls) {
        for (auto girl = girls.begin(); girl!=girls.end(); ++girl) {
            if (girl->second->component == 0) { // found new component
                bool inserted;
                std::tie(it_cmp, inserted) = components.insert(std::make_pair(++cmp_id, 0));
                this->visit(girl->second);
                total_shoppers += it_cmp->second;
                }
            }
        };
    void visit(pGirl& girl) {
        if (girl->component == 0) {
            girl->component = it_cmp->first;
            it_cmp->second += girl->answers[Girl::SHOP];
            for(auto g: girl->friends) {
                this->visit(g);
                }
            }
        };
    }; 