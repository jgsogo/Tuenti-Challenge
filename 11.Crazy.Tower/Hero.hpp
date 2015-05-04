
#pragma once

#include <cstdint>
#include "Room.hpp"
#include "ModularArithmetic.hpp"

#if(VERBOSE)
    #define DEBUGGING(msg) msg;
#else
    #define DEBUGGING(msg) void(0);
#endif

struct Hero;

struct RoomOption {
    pStairs stairs;
    std::uint32_t multiplicity;
    RoomOption(pStairs stairs, const std::uint32_t& multiplicity) : stairs(stairs), multiplicity(multiplicity) {};
    virtual std::uint32_t exec(Hero& hero, const std::size_t& init_stamina);
    };


struct Hero {
    const std::size_t max_stamina;
    std::size_t stamina;
    const std::uint32_t modulo;

    Hero(const std::size_t& max_stamina, const std::size_t& stamina, const std::uint32_t& modulo) : max_stamina(max_stamina), stamina(stamina), modulo(modulo) {};

    static std::uint32_t visit(const pRoom& room, Hero& hero) {
        DEBUGGING(std::cout << "At room '" << room->id << "' with [" << room->downstairs.size() << "] stairs" << std::endl)
        DEBUGGING(std::cout << "\t stamina: " << hero.stamina << std::endl)
        if (room->is_exit) {
            DEBUGGING(std::cout << "\tEXIT!" << std::endl)
            return 1;
            }

        // Check if I have already been here
        if (room->min_stamina_for_solutions > hero.stamina) {
            return 0;
            }
        auto sol_cache = room->solutions.insert(std::make_pair(hero.stamina, 0));
        if (!sol_cache.second) { // It is already cached!
            return sol_cache.first->second;
            }

        // Otherwise, compute options
        auto stamina = hero.stamina; // 'hero' has this level of stamina on arrival
        auto options = Hero::generate(room, hero);
        DEBUGGING(std::cout << "\t>> " << options.size() << " options" << std::endl)
        std::uint32_t successful_options = 0;
        for (auto it = options.begin(); it!=options.end(); ++it) {
            auto new_solutions = it->exec(hero, stamina);
            successful_options = ModularArithmetic<std::uint32_t>::sum(successful_options, new_solutions, hero.modulo);
            }

        // Update cache
        if (!successful_options) {
            room->min_stamina_for_solutions = std::max(room->min_stamina_for_solutions, stamina);
            }
        sol_cache.first->second = successful_options;
        
        return successful_options;
        };

    static std::vector<RoomOption> generate(const pRoom& room, const Hero& hero) {
        std::vector<RoomOption> options;
        for (auto it = room->downstairs.begin(); it!=room->downstairs.end(); ++it) {
            DEBUGGING(std::cout << "\t stair to '" << (*it)->down->id << "' (keys='" << (*it)->keys << "' stamina='" << (*it)->stamina << "')" << std::endl;)
            // Generate options according to each stairs
            //  - Can I get all the keys?
            auto possible = ((*it)->keys <= room->downstairs.size());
            DEBUGGING(if (!possible) {std::cout << "\t\t not enough KEYS!" << std::endl;})
            //  - Can I reach all the stamina I need?
            possible &= ((*it)->stamina <= static_cast<int>(hero.max_stamina));
            DEBUGGING(if (!possible) {std::cout << "\t\t not enough MAX_STAMINA!" << std::endl;})
            //  - Can I get all the stamina needed?
            int needed = (*it)->stamina - hero.stamina;
            possible &= (needed <= static_cast<int>(room->downstairs.size()));
            DEBUGGING(if (!possible) {std::cout << "\t\t not enough minions to get '" << needed << "' STAMINA!" << std::endl;})
            if (possible) {
                auto multiplicity = ModularArithmetic<std::uint32_t>::combinations(room->downstairs.size()-1, std::max<int>(0, static_cast<int>((*it)->keys)-1), hero.modulo);
                options.insert(options.end(), RoomOption(*it, multiplicity));
                DEBUGGING(std::cout << "\t\t >> " << multiplicity << " ways (C_{" << room->downstairs.size()-1 << ", " << std::max<int>(0, static_cast<int>((*it)->keys)-1) << "})" << std::endl)
                }
        
            }
        return options;
        }

    };


/* RoomOption implementation */
std::uint32_t RoomOption::exec(Hero& hero, const std::size_t& init_stamina) {
    // We have to kill a number of minions equal to the number of keys (and then go down through the stairs)
    hero.stamina = std::min(hero.max_stamina, init_stamina + std::max(1, stairs->keys)); // There is always a minion to kill :'(
    hero.stamina = std::min(hero.max_stamina, hero.stamina - stairs->stamina);
    // and we can execute this option in several ways (multiplicity)
    auto solutions = Hero::visit(stairs->down, hero);
    return ModularArithmetic<std::uint32_t>::multiply(multiplicity, solutions, hero.modulo);
    };
