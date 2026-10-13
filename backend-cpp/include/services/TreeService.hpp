#pragma once

#include <memory>
#include "../graph/AlumniTree.hpp"
#include "../repositories/DataRepository.hpp"

namespace alumniverse {

class TreeService {
public:
    explicit TreeService(std::shared_ptr<DataRepository> repo);

    /**
     * @brief Generates hierarchical tree in React Flow format.
     * @param hierarchyType "academic" or "industry"
     * @param filter Criteria to filter leaves/branches
     */
    nlohmann::json getTree(const std::string& hierarchyType = "academic",
                           const FilterCriteria& filter = {});

    /**
     * @brief Extracts a specific subtree rooted at nodeId and applies any active filters.
     */
    nlohmann::json getSubtree(const std::string& nodeId,
                              const std::string& hierarchyType = "academic",
                              const FilterCriteria& filter = {});

    /**
     * @brief Retrieves raw node details by nodeId.
     */
    nlohmann::json getNodeDetails(const std::string& nodeId,
                                  const std::string& hierarchyType = "academic");

private:
    std::shared_ptr<DataRepository> repo_;
    AlumniTree academicTree_;
    AlumniTree industryTree_;

    void ensureTreesBuilt();
};

} // namespace alumniverse
