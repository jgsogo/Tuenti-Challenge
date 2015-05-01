
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <bitset>
#include <set>


// Structs to store data about the world
struct Girl {
    enum Q {GAME, FIGURE, SUITS, CATS, SHOP};
    std::size_t id;
    std::string label;
    std::bitset<5> answers;
    std::size_t points;
    std::size_t component; // for graph connected_components computation.

    std::set<std::shared_ptr<Girl>> friends;
    //std::vector<std::shared_ptr<Girl>> friends_of_friends;

    Girl(std::size_t id) : id(id) {
        points = 0;
        component = 0;
        };

    void parse_line(std::ifstream& file) {
        char q[5];
        file >> label >> q[0] >> q[1] >> q[2] >> q[3] >> q[4];
        for (auto i=0; i<5; ++i) {
            if (q[i]=='Y') {answers.set(i);}
            }
        };

    void print(std::ostream& os) {
        os << "Girl [" << id << "]: " << label << std::endl;
        os << "\t GAME:" << answers[0];
        os << "\t FIGURE:" << answers[1];
        os << "\t SUITS:" << answers[2];
        os << "\t CATS:" << answers[3];
        os << "\t SHOP:" << answers[4] << std::endl;
        os << "\t friends:";
        for (auto it = friends.begin(); it!=friends.end(); ++it) {
            std::cout << " " << (*it)->label;
            }            
        std::cout << std::endl;
        std::cout << "\t >>> component: " << component << std::endl;
        std::cout << "\t >>> points: " << points << std::endl;
        };    
        
    std::size_t compute_points(const std::map<std::size_t, std::size_t>& components, const std::size_t& total_shoppers);
    };
typedef std::shared_ptr<Girl> pGirl;


std::size_t Girl::compute_points(const std::map<std::size_t, std::size_t>& components, const std::size_t& total_shoppers) {
    // RULE#1: 7 points if G likes naughty, dirty games
    points += answers[GAME] ? 7 : 0;

    // RULE#2: 3 points for every friend of G who likes super hero action figures.
    for (auto it = friends.begin(); it!= friends.end(); ++it) {
        points += (*it)->answers[FIGURE] ? 3 : 0;
        }

    // RULE#3: 6 points for every friend of a friend of G, not including the friends of G and G herself, who likes men in leisure suits
    // RULE#4: 4 points if G has any friend H who likes cats, and no friend of H (except perhaps G) likes cats (4 points at most, not 4 for every friend).
    //  - Build friends of friends vector (and cat candidates)
    std::vector<pGirl> friends_cat;
    std::set<pGirl> friends_of_friends;
    for (auto it = friends.begin(); it!=friends.end(); ++it) {
        if ( (*it)->answers[CATS] ) {
            friends_cat.push_back(*it);
            }
        friends_of_friends.insert((*it)->friends.begin(), (*it)->friends.end());
        }
    //  - Remove friends from friends_of_friends
    std::vector<pGirl> v(friends_of_friends.size());
    std::vector<pGirl>::iterator it;
    it = std::set_difference(friends_of_friends.begin(), friends_of_friends.end(), friends.begin(), friends.end(), v.begin());
    v.resize(it-v.begin());
    //  - Remove herself from friend_of_friends
    v.erase(std::remove_if(v.begin(), v.end(), [this](const pGirl& girl){ return girl->id == id;}), v.end());

    // RULE#3: 6 points for every friend of a friend of G, not including the friends of G and G herself, who likes men in leisure suits
    for (auto it = v.begin(); it!= v.end(); ++it) {
        points += (*it)->answers[SUITS] ? 6 : 0;
        }

    // RULE#4: 4 points if G has any friend H who likes cats, and no friend of H (except perhaps G) likes cats (4 points at most, not 4 for every friend).
    for (auto it = friends_cat.begin(); it!=friends_cat.end(); ++it) {
        auto none = std::none_of((*it)->friends.begin(), (*it)->friends.end(), [this](const pGirl& girl){
            return ((girl->id != id) && (girl->answers[CATS]));
            });
        points += none ? 4 : 0;
        }


    // RULE#5: 5 points for every girl H who likes to go shopping and has no possible connection with G through a chain of friends (friends, friends of friends, friends of friends of friends, etc.)
    auto non_connected_shoppers = total_shoppers - components.find(component)->second;
    points += 5*non_connected_shoppers;
    
    return points;
    };
