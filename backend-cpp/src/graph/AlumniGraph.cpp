#include "../../include/graph/AlumniGraph.hpp"
#include <cmath>

namespace alumniverse {

void AlumniGraph::addVertex(const GraphNode& node) {
    nodes_[node.id] = node;
    if (adjList_.find(node.id) == adjList_.end()) {
        adjList_[node.id] = {};
    }
}

bool AlumniGraph::removeVertex(int id) {
    if (nodes_.find(id) == nodes_.end()) {
        return false;
    }

    // Remove all edges referencing this vertex from neighbors
    for (const auto& edge : adjList_[id]) {
        auto& neighborEdges = adjList_[edge.targetId];
        neighborEdges.erase(
            std::remove_if(neighborEdges.begin(), neighborEdges.end(),
                           [id](const GraphEdge& e) { return e.targetId == id; }),
            neighborEdges.end()
        );
        if (edgeCount_ > 0) edgeCount_--;
    }

    adjList_.erase(id);
    nodes_.erase(id);
    return true;
}

bool AlumniGraph::hasVertex(int id) const {
    return nodes_.find(id) != nodes_.end();
}

std::optional<GraphNode> AlumniGraph::getVertex(int id) const {
    auto it = nodes_.find(id);
    if (it != nodes_.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool AlumniGraph::addEdge(int sourceId, int targetId, const std::string& relationshipType, const std::string& createdAt) {
    if (sourceId == targetId) return false; // No self loops
    if (!hasVertex(sourceId) || !hasVertex(targetId)) return false; // Detect invalid references

    // Check duplicate edge
    const auto& sEdges = adjList_[sourceId];
    for (const auto& e : sEdges) {
        if (e.targetId == targetId && e.relationshipType == relationshipType) {
            return false; // Duplicate edge exists
        }
    }

    // Undirected connection: add both forward and reverse
    adjList_[sourceId].push_back({targetId, relationshipType, createdAt});
    adjList_[targetId].push_back({sourceId, relationshipType, createdAt});
    edgeCount_++;
    return true;
}

bool AlumniGraph::removeEdge(int sourceId, int targetId) {
    if (!hasVertex(sourceId) || !hasVertex(targetId)) return false;

    auto& sEdges = adjList_[sourceId];
    auto sIt = std::remove_if(sEdges.begin(), sEdges.end(),
                              [targetId](const GraphEdge& e) { return e.targetId == targetId; });
    bool removed = (sIt != sEdges.end());
    sEdges.erase(sIt, sEdges.end());

    auto& tEdges = adjList_[targetId];
    tEdges.erase(
        std::remove_if(tEdges.begin(), tEdges.end(),
                       [sourceId](const GraphEdge& e) { return e.targetId == sourceId; }),
        tEdges.end()
    );

    if (removed && edgeCount_ > 0) {
        edgeCount_--;
    }
    return removed;
}

std::vector<GraphEdge> AlumniGraph::getNeighbors(int id) const {
    auto it = adjList_.find(id);
    if (it != adjList_.end()) {
        return it->second;
    }
    return {};
}

std::vector<int> AlumniGraph::bfs(int startId, int maxDepth, size_t maxNodes) const {
    if (!hasVertex(startId)) return {};

    std::vector<int> result;
    std::unordered_set<int> visited;
    // queue stores pairs of {nodeId, currentDepth}
    std::queue<std::pair<int, int>> q;

    visited.insert(startId);
    q.push({startId, 0});

    while (!q.empty() && result.size() < maxNodes) {
        auto [currentId, depth] = q.front();
        q.pop();

        result.push_back(currentId);

        if (depth >= maxDepth) continue;

        auto it = adjList_.find(currentId);
        if (it != adjList_.end()) {
            for (const auto& edge : it->second) {
                if (visited.find(edge.targetId) == visited.end()) {
                    visited.insert(edge.targetId);
                    q.push({edge.targetId, depth + 1});
                }
            }
        }
    }

    return result;
}

std::vector<int> AlumniGraph::dfs(int startId, int maxDepth, size_t maxNodes) const {
    if (!hasVertex(startId)) return {};

    std::vector<int> result;
    std::unordered_set<int> visited;
    // stack stores pairs of {nodeId, currentDepth}
    std::stack<std::pair<int, int>> st;

    st.push({startId, 0});

    while (!st.empty() && result.size() < maxNodes) {
        auto [currentId, depth] = st.top();
        st.pop();

        if (visited.find(currentId) != visited.end()) continue;

        visited.insert(currentId);
        result.push_back(currentId);

        if (depth >= maxDepth) continue;

        auto it = adjList_.find(currentId);
        if (it != adjList_.end()) {
            for (const auto& edge : it->second) {
                if (visited.find(edge.targetId) == visited.end()) {
                    st.push({edge.targetId, depth + 1});
                }
            }
        }
    }

    return result;
}

std::vector<int> AlumniGraph::findShortestPath(int sourceId, int targetId) const {
    if (!hasVertex(sourceId) || !hasVertex(targetId)) return {};
    if (sourceId == targetId) return {sourceId};

    std::unordered_map<int, int> parent;
    std::unordered_set<int> visited;
    std::queue<int> q;

    visited.insert(sourceId);
    q.push(sourceId);
    bool found = false;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        if (curr == targetId) {
            found = true;
            break;
        }

        auto it = adjList_.find(curr);
        if (it != adjList_.end()) {
            for (const auto& edge : it->second) {
                if (visited.find(edge.targetId) == visited.end()) {
                    visited.insert(edge.targetId);
                    parent[edge.targetId] = curr;
                    q.push(edge.targetId);
                }
            }
        }
    }

    if (!found) return {};

    // Reconstruct path backwards from targetId to sourceId
    std::vector<int> path;
    int curr = targetId;
    while (curr != sourceId) {
        path.push_back(curr);
        curr = parent[curr];
    }
    path.push_back(sourceId);
    std::reverse(path.begin(), path.end());
    return path;
}

void AlumniGraph::clear() {
    nodes_.clear();
    adjList_.clear();
    edgeCount_ = 0;
}

nlohmann::json AlumniGraph::exportToReactFlowJson(const std::vector<int>& highlightedPath) const {
    std::vector<int> allNodeIds;
    allNodeIds.reserve(nodes_.size());
    for (const auto& [id, _] : nodes_) {
        allNodeIds.push_back(id);
    }
    std::sort(allNodeIds.begin(), allNodeIds.end());
    return getInducedSubgraphJson(allNodeIds);
}

nlohmann::json AlumniGraph::getInducedSubgraphJson(const std::vector<int>& subsetIds) const {
    nlohmann::json rfNodes = nlohmann::json::array();
    nlohmann::json rfEdges = nlohmann::json::array();

    std::unordered_set<int> validSubset(subsetIds.begin(), subsetIds.end());

    // Arrange nodes in a circular / grouped layout
    const double radius = 320.0 + (subsetIds.size() * 15.0);
    const double centerX = 500.0;
    const double centerY = 400.0;
    const double angleStep = subsetIds.empty() ? 0.0 : (2.0 * 3.14159265358979323846 / subsetIds.size());

    size_t index = 0;
    for (int id : subsetIds) {
        auto it = nodes_.find(id);
        if (it == nodes_.end()) continue;
        const auto& node = it->second;

        double angle = index * angleStep;
        double posX = centerX + radius * std::cos(angle);
        double posY = centerY + radius * std::sin(angle);

        nlohmann::json n = {
            {"id", std::to_string(id)},
            {"type", "alumniNode"},
            {"position", {{"x", std::round(posX)}, {"y", std::round(posY)}}},
            {"data", node.toJson()}
        };
        rfNodes.push_back(n);
        index++;
    }

    // Add edges between nodes in the subset (undirected: add only once where source < target)
    std::unordered_set<std::string> seenEdges;
    for (int id : subsetIds) {
        auto it = adjList_.find(id);
        if (it == adjList_.end()) continue;

        for (const auto& edge : it->second) {
            if (validSubset.count(edge.targetId) && id < edge.targetId) {
                std::string edgeId = "e-" + std::to_string(id) + "-" + std::to_string(edge.targetId);
                nlohmann::json e = {
                    {"id", edgeId},
                    {"source", std::to_string(id)},
                    {"target", std::to_string(edge.targetId)},
                    {"label", edge.relationshipType},
                    {"animated", false},
                    {"style", {{"stroke", "#00D2FF"}, {"strokeWidth", 2}}}
                };
                rfEdges.push_back(e);
            }
        }
    }

    return {
        {"nodes", rfNodes},
        {"edges", rfEdges},
        {"stats", {
            {"nodeCount", rfNodes.size()},
            {"edgeCount", rfEdges.size()}
        }}
    };
}

} // namespace alumniverse
