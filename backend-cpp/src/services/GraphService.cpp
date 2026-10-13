#include "../../include/services/GraphService.hpp"

namespace alumniverse {

GraphService::GraphService(std::shared_ptr<DataRepository> repo) : repo_(std::move(repo)) {
    refreshGraph();
}

void GraphService::refreshGraph() {
    graph_.clear();

    // 1. Load all alumni as graph vertices
    FilterCriteria all;
    all.limit = 10000;
    int dummy = 0;
    auto alumniList = repo_->getAlumniList(all, dummy);

    for (const auto& al : alumniList) {
        GraphNode node;
        node.id = al.id;
        node.name = al.fullName;
        node.currentCompany = al.currentCompany;
        node.currentJobTitle = al.currentJobTitle;
        node.currentIndustry = al.currentIndustry;
        node.department = al.departmentName;
        node.degree = al.degree;
        node.graduationYear = al.graduationYear;
        node.avatarUrl = al.avatarUrl;
        node.verificationStatus = al.verificationStatus;
        node.location = al.currentLocation;
        node.skills = al.skills;
        graph_.addVertex(node);
    }

    // 2. Load all confirmed connections as graph edges
    auto connections = repo_->getAllConnections();
    for (const auto& c : connections) {
        graph_.addEdge(c.sourceAlumniId, c.targetAlumniId, c.relationshipType, c.createdAt);
    }
}

nlohmann::json GraphService::getGraph(const FilterCriteria& filter) {
    refreshGraph();

    int total = 0;
    FilterCriteria f = filter;
    f.limit = 10000;
    auto filteredAlumni = repo_->getAlumniList(f, total);

    std::vector<int> filteredIds;
    for (const auto& al : filteredAlumni) {
        filteredIds.push_back(al.id);
    }

    return graph_.getInducedSubgraphJson(filteredIds);
}

nlohmann::json GraphService::getNeighbors(int alumniId) {
    refreshGraph();
    auto vertex = graph_.getVertex(alumniId);
    if (!vertex.has_value()) {
        return {{"error", "Alumni not found in graph"}, {"neighbors", nlohmann::json::array()}};
    }

    auto edges = graph_.getNeighbors(alumniId);
    nlohmann::json nList = nlohmann::json::array();
    for (const auto& e : edges) {
        auto neighborVertex = graph_.getVertex(e.targetId);
        if (neighborVertex.has_value()) {
            nList.push_back({
                {"alumni_id", e.targetId},
                {"name", neighborVertex->name},
                {"company", neighborVertex->currentCompany},
                {"job_title", neighborVertex->currentJobTitle},
                {"relationship_type", e.relationshipType},
                {"avatar_url", neighborVertex->avatarUrl}
            });
        }
    }

    return {
        {"alumni", vertex->toJson()},
        {"neighbors", nList},
        {"count", nList.size()}
    };
}

nlohmann::json GraphService::findShortestPath(int sourceId, int targetId) {
    refreshGraph();
    if (!graph_.hasVertex(sourceId) || !graph_.hasVertex(targetId)) {
        return {
            {"found", false},
            {"error", "One or both alumni nodes do not exist in graph."},
            {"path", nlohmann::json::array()}
        };
    }

    auto path = graph_.findShortestPath(sourceId, targetId);
    if (path.empty()) {
        return {
            {"found", false},
            {"message", "No connected path exists between the specified alumni."},
            {"path", nlohmann::json::array()}
        };
    }

    nlohmann::json pathNodes = nlohmann::json::array();
    for (int id : path) {
        auto v = graph_.getVertex(id);
        if (v.has_value()) {
            pathNodes.push_back(v->toJson());
        }
    }

    // Induced subgraph containing the path and connecting edges
    auto subgraphJson = graph_.getInducedSubgraphJson(path);

    return {
        {"found", true},
        {"pathLength", path.size() - 1},
        {"pathNodeIds", path},
        {"pathDetails", pathNodes},
        {"visualization", subgraphJson}
    };
}

nlohmann::json GraphService::bfsTraversal(int startId, int maxDepth) {
    refreshGraph();
    auto order = graph_.bfs(startId, maxDepth);
    return {
        {"startId", startId},
        {"maxDepth", maxDepth},
        {"traversalOrder", order},
        {"visitedCount", order.size()}
    };
}

nlohmann::json GraphService::dfsTraversal(int startId, int maxDepth) {
    refreshGraph();
    auto order = graph_.dfs(startId, maxDepth);
    return {
        {"startId", startId},
        {"maxDepth", maxDepth},
        {"traversalOrder", order},
        {"visitedCount", order.size()}
    };
}

} // namespace alumniverse
