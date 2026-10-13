#pragma once

#include <memory>
#include "../graph/AlumniGraph.hpp"
#include "../repositories/DataRepository.hpp"

namespace alumniverse {

class GraphService {
public:
    explicit GraphService(std::shared_ptr<DataRepository> repo);

    /**
     * @brief Builds or refreshes the AlumniGraph from current repository data.
     */
    void refreshGraph();

    /**
     * @brief Exports full network graph or filtered subgraph for React Flow.
     */
    nlohmann::json getGraph(const FilterCriteria& filter = {});

    /**
     * @brief Retrieves immediate neighbors and relationship details for a specific alumni.
     */
    nlohmann::json getNeighbors(int alumniId);

    /**
     * @brief Finds shortest unweighted path between source and target alumni.
     */
    nlohmann::json findShortestPath(int sourceId, int targetId);

    /**
     * @brief Performs BFS exploration up to maxDepth.
     */
    nlohmann::json bfsTraversal(int startId, int maxDepth = 4);

    /**
     * @brief Performs DFS exploration up to maxDepth.
     */
    nlohmann::json dfsTraversal(int startId, int maxDepth = 4);

private:
    std::shared_ptr<DataRepository> repo_;
    AlumniGraph graph_;
};

} // namespace alumniverse
