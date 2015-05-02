
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <bitset>
#include <set>
#include <unordered_map>

// Structs to store data about the world
typedef std::string room_id;

struct Stairs;
typedef std::shared_ptr<Stairs> pStairs;

struct Room {
    bool is_exit;
    room_id id;
    std::unordered_map<room_id, pStairs> downstairs;
    std::unordered_map<room_id, pStairs> upstairs;
    
    Room(const room_id& id) : id(id) {
        is_exit = (id.compare("exit")==0);
        };
    };
typedef std::shared_ptr<Room> pRoom;


struct Stairs {
    int stamina;
    int keys;

    pRoom down;
    pRoom up;

    Stairs() : stamina(0), keys(0) {};
    Stairs(const Stairs& other) : stamina(other.stamina), keys(other.keys) {};

    };
