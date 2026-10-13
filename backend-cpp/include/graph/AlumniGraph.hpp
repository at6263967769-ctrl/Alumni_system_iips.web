#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <optional>
#include <algorithm>
#include "../vendor/nlohmann/json.hpp"

namespace alumniverse {

struct GraphEdge {
    int targetId;
    std::string relationshipType;
    std::string createdAt;

    bool operator==(const GraphEdge& other) const {
        return targetId == other.targetId && relationshipType == other.relationshipType;
    }
};

struct GraphNode {
    int id{0};
    std::string name;
    std::string currentCompany;
    std::string currentJobTitle;
    std::string currentIndustry;
    std::string department;
    std::string degree;
    int graduationYear{0};
    std::string avatarUrl;
    std::string verificationStatus;
    std::string location;
    std::vector<std::string> skills;

    nlohmann::json toJson() const {
        return {
            {"id", std::to_string(id)},
            {"numeric_id", id},
            {"name", name},
            {"company", currentCompany},
            {"job_title", currentJobTitle},
            {"industry", currentIndustry},
            {"department", department},
            {"degree", degree},
            {"graduation_year", graduationYear},
            {"avatar_url", avatarUrl},
            {"verification_status", verificationStatus},
            {"location", location},
            {"skills", skills}
        };
    }
};

/**
 * @class AlumniGraph
 * @brief High-performance adjacency list representation of the alumni relationship network.
 * 
 * Supports authoritative graph operations:
 * - Vertex insertion & removal
 * - Edge insertion (with duplicate suppression and endpoint validation)
 * - BFS and DFS traversals with cycle protection
 * - Shortest path discovery via unweighted BFS
 * - Subgraph extraction and neighbor querying
 */
class AlumniGraph {
public:
    AlumniGraph() = default;

    /**
     * @brief Adds or updates a vertex in the graph.
     * Time Complexity: O(1) average.
     */
    void addVertex(const GraphNode& node);

    /**
     * @brief Removes a vertex and all its incident edges.
     * Time Complexity: O(V + E) where V is vertices, E is edges.
     */
    bool removeVertex(int id);

    /**
     * @brief Checks if a vertex exists.
     * Time Complexity: O(1) average.
     */
    bool hasVertex(int id) const;

    /**
     * @brief Retrieves a vertex by ID.
     * Time Complexity: O(1) average.
     */
    std::optional<GraphNode> getVertex(int id) const;

    /**
     * @brief Adds an undirected relationship between two existing vertices.
     * Time Complexity: O(deg(source) + deg(target)).
     * @return true if edge was added; false if vertices are invalid or duplicate edge exists.
     */
    bool addEdge(int sourceId, int targetId, const std::string& relationshipType, const std::string& createdAt = "");

    /**
     * @brief Removes an edge between source and target.
     * Time Complexity: O(deg(source) + deg(target)).
     */
    bool removeEdge(int sourceId, int targetId);

    /**
     * @brief Gets all neighbors of a node.
     * Time Complexity: O(deg(id)).
     */
    std::vector<GraphEdge> getNeighbors(int id) const;

    /**
     * @brief Breadth-First Search traversal starting from startId.
     * Time Complexity: O(V + E).
     * Handles cyclic graphs safely via a visited set.
     */
    std::vector<int> bfs(int startId, int maxDepth = 10, size_t maxNodes = 500) const;

    /**
     * @brief Depth-First Search traversal starting from startId.
     * Time Complexity: O(V + E).
     * Handles cyclic graphs safely via a visited set.
     */
    std::vector<int> dfs(int startId, int maxDepth = 10, size_t maxNodes = 500) const;

    /**
     * @brief Finds the shortest unweighted path between sourceId and targetId.
     * Time Complexity: O(V + E) via BFS parent tracking.
     * @return List of alumni IDs in order from source to target, or empty if unreachable.
     */
    std::vector<int> findShortestPath(int sourceId, int targetId) const;

    /**
     * @brief Clears all vertices and edges.
     */
    void clear();

    /**
     * @brief Returns total vertex count.
     */
    size_t getVertexCount() const { return nodes_.size(); }

    /**
     * @brief Returns total undirected edge count.
     */
    size_t getEdgeCount() const { return edgeCount_; }

    /**
     * @brief Exports graph to React Flow format (nodes array, edges array).
     */
    nlohmann::json exportToReactFlowJson(const std::vector<int>& highlightedPath = {}) const;

    /**
     * @brief Returns an induced subgraph for the given set of node IDs.
     */
    nlohmann::json getInducedSubgraphJson(const std::vector<int>& subsetIds) const;

private:
    std::unordered_map<int, GraphNode> nodes_;
    std::unordered_map<int, std::vector<GraphEdge>> adjList_;
    size_t edgeCount_{0};
};

} // namespace alumniverse
