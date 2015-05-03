
#pragma once

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
    std::size_t multiplicity;
    RoomOption(pStairs stairs, const std::size_t& multiplicity) : stairs(stairs), multiplicity(multiplicity) {};
    virtual std::size_t exec(Hero& hero, const std::size_t& init_stamina);
    };


struct Hero {
    const std::size_t max_stamina;
    std::size_t stamina;
    const std::size_t modulo;

    Hero(const std::size_t& max_stamina, const std::size_t& stamina, const std::size_t& modulo) : max_stamina(max_stamina), stamina(stamina), modulo(modulo) {};

    static std::size_t visit(const pRoom& room, Hero& hero) {
        DEBUGGING(std::cout << "At room '" << room->id << "' with [" << room->downstairs.size() << "] stairs" << std::endl)
        DEBUGGING(std::cout << "\t stamina: " << hero.stamina << std::endl)
        if (room->is_exit) {
            DEBUGGING(std::cout << "\tEXIT!" << std::endl)
            return 1;
            }
        // Otherwise, compute options
        auto stamina = hero.stamina; // 'hero' has this level of stamina on arrival
        auto options = Hero::generate(room, hero);
        DEBUGGING(std::cout << "\t>> " << options.size() << " options" << std::endl)
        std::size_t successful_options = 0;
        for (auto it = options.begin(); it!=options.end(); ++it) {
            auto new_solutions = it->exec(hero, stamina);
            successful_options = ModularArithmetic<std::uint32_t>::sum(successful_options, new_solutions, hero.modulo);
            }
        return successful_options;
        };

    static std::vector<RoomOption> generate(const pRoom& room, const Hero& hero) {
        std::vector<RoomOption> options;
        for (auto it = room->downstairs.begin(); it!=room->downstairs.end(); ++it) {
            DEBUGGING(std::cout << "\t stair to '" << it->second->down->id << "' (keys='" << it->second->keys << "' stamina='" << it->second->stamina << "')" << std::endl;)
            // Generate options according to each stairs
            //  - Can I get all the keys?
            auto possible = (it->second->keys <= room->downstairs.size());
            DEBUGGING(if (!possible) {std::cout << "\t\t not enough KEYS!" << std::endl;})
            //  - Can I reach all the stamina I need?
            possible &= (it->second->stamina <= hero.max_stamina);
            DEBUGGING(if (!possible) {std::cout << "\t\t not enough MAX_STAMINA!" << std::endl;})
            //  - Can I get all the stamina needed?
            int needed = it->second->stamina - hero.stamina;
            possible &= (needed <= static_cast<int>(room->downstairs.size()));
            DEBUGGING(if (!possible) {std::cout << "\t\t not enough minions to get '" << needed << "' STAMINA!" << std::endl;})
            if (possible) {
                auto multiplicity = ModularArithmetic<std::uint32_t>::combinations(room->downstairs.size()-1, std::max<int>(0, it->second->keys-1), hero.modulo);
                options.insert(options.end(), RoomOption(it->second, multiplicity));
                DEBUGGING(std::cout << "\t\t >> " << multiplicity << " ways (C_{" << room->downstairs.size()-1 << ", " << std::max<int>(0, it->second->keys-1) << "})" << std::endl)
                }
        
            }
        return options;
        }

    };


/* RoomOption implementation */
std::size_t RoomOption::exec(Hero& hero, const std::size_t& init_stamina) {
    // We have to kill a number of minions equal to the number of keys (and then go down through the stairs)
    hero.stamina = std::min(hero.max_stamina, init_stamina + std::max(1, stairs->keys)); // There is always a minion to kill :'(
    hero.stamina = std::min(hero.max_stamina, hero.stamina - stairs->stamina);
    // and we can execute this option in several ways (multiplicity)
    auto solutions = Hero::visit(stairs->down, hero);
    return ModularArithmetic<std::uint32_t>::multiply(multiplicity, solutions, hero.modulo);
    };
