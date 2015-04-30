
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>

// Structs to store data about the world
struct Connection;
typedef std::shared_ptr<Connection> pConnection;

struct Island {
    std::string id;
    long cost;
    std::vector<pConnection> outgoing;
    std::vector<pConnection> incoming;
    std::map<std::size_t, long> step_cost;
    std::size_t distance_to_end;
    std::size_t distance_to_start;
    bool visited;

    Island() {
        distance_to_end = std::numeric_limits<std::size_t>::max();
        distance_to_start = std::numeric_limits<std::size_t>::max();
        visited = false;
        };
    long get_cost(const std::size_t& step) {
        // At each step the cost of visiting an island is Island::cost, except if there are pirates arriving.
        auto it = step_cost.find(step);
        return (it != step_cost.end()) ? cost + it->second : cost;
        };
    void order_connections();
    void disconnect();
    };
typedef std::shared_ptr<Island> pIsland;

struct Connection {
    pIsland start;
    pIsland end;
    long cost;
    bool operator==(const Connection& other) const {
        return ((start==other.start) && (end==other.end) && (cost==other.cost));
        };
    };


/**** Implementation ******/
void Island::order_connections() {
    // Order connections from lower cost to higher cost
    auto order_connections = [](const pConnection& lhs, const pConnection& rhs) {
        return lhs->cost < rhs->cost;
        };
    std::stable_sort(outgoing.begin(), outgoing.end(), order_connections);
    }

void Island::disconnect() {
    // Delete all connections and references to adjacent islands.
    for (auto it = outgoing.begin(); it!=outgoing.end(); ++it) {
        std::vector<pConnection>& inc = (*it)->end->incoming;
        std::remove(inc.begin(), inc.end(), *it);
        }
    for (auto it = incoming.begin(); it!=incoming.end(); ++it) {
        std::vector<pConnection>& out = (*it)->start->outgoing;
        std::remove(out.begin(), out.end(), *it);
        }
    outgoing.clear();
    incoming.clear();
    }