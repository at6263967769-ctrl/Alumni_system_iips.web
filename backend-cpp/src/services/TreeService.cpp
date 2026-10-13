#include "../../include/services/TreeService.hpp"

namespace alumniverse {

TreeService::TreeService(std::shared_ptr<DataRepository> repo) : repo_(std::move(repo)) {
    ensureTreesBuilt();
}

void TreeService::ensureTreesBuilt() {
    auto insts = repo_->getInstitutions();
    Institution inst = insts.empty() ? Institution{1, "Academic Institution", "https://iips.edu.in", ""} : insts[0];
    auto depts = repo_->getDepartments();
    auto batches = repo_->getBatches();
    auto comps = repo_->getCompanies();

    FilterCriteria all;
    all.limit = 10000;
    int dummy = 0;
    auto alumniList = repo_->getAlumniList(all, dummy);

    academicTree_.buildAcademicTree(inst, depts, batches, alumniList);
    industryTree_.buildIndustryTree(comps, alumniList);
}

nlohmann::json TreeService::getTree(const std::string& hierarchyType, const FilterCriteria& filter) {
    ensureTreesBuilt();
    const AlumniTree& activeTree = (hierarchyType == "industry") ? industryTree_ : academicTree_;

    std::shared_ptr<TreeNode> rootToExport;

    // Check if any filter is active
    bool hasActiveFilter = filter.searchKeyword.has_value() ||
                           filter.institution.has_value() ||
                           filter.department.has_value() ||
                           filter.degree.has_value() ||
                           filter.graduationYear.has_value() ||
                           filter.batchId.has_value() ||
                           filter.company.has_value() ||
                           filter.jobTitle.has_value() ||
                           filter.industry.has_value() ||
                           filter.verificationStatus.has_value() ||
                           filter.location.has_value() ||
                           !filter.skills.empty();

    if (hasActiveFilter) {
        rootToExport = activeTree.filterTree(filter);
    } else {
        rootToExport = activeTree.getRoot();
    }

    auto rfJson = AlumniTree::exportToReactFlowJson(rootToExport);
    rfJson["hierarchyType"] = hierarchyType;
    rfJson["filterApplied"] = hasActiveFilter;
    return rfJson;
}

nlohmann::json TreeService::getSubtree(const std::string& nodeId, const std::string& hierarchyType, const FilterCriteria& filter) {
    ensureTreesBuilt();
    const AlumniTree& activeTree = (hierarchyType == "industry") ? industryTree_ : academicTree_;

    auto subRoot = activeTree.getSubtree(nodeId);
    if (!subRoot) {
        return {
            {"error", "Node not found in tree"},
            {"nodes", nlohmann::json::array()},
            {"edges", nlohmann::json::array()}
        };
    }

    bool hasActiveFilter = filter.searchKeyword.has_value() ||
                           filter.institution.has_value() ||
                           filter.department.has_value() ||
                           filter.degree.has_value() ||
                           filter.graduationYear.has_value() ||
                           filter.batchId.has_value() ||
                           filter.company.has_value() ||
                           filter.jobTitle.has_value() ||
                           filter.industry.has_value() ||
                           filter.verificationStatus.has_value() ||
                           filter.location.has_value() ||
                           !filter.skills.empty();

    std::shared_ptr<TreeNode> nodeToExport = subRoot;
    if (hasActiveFilter) {
        AlumniTree tempTree;
        // In a filtered subtree, we filter starting from subRoot
        AlumniTree fullTree = activeTree;
        auto filteredFull = fullTree.filterTree(filter);
        if (filteredFull) {
            // Find target nodeId in the filtered tree
            auto findInFiltered = [](auto& self, const std::shared_ptr<TreeNode>& node, const std::string& target) -> std::shared_ptr<TreeNode> {
                if (!node) return nullptr;
                if (node->id == target) return node;
                for (const auto& c : node->children) {
                    auto res = self(self, c, target);
                    if (res) return res;
                }
                return nullptr;
            };
            nodeToExport = findInFiltered(findInFiltered, filteredFull, nodeId);
        } else {
            nodeToExport = nullptr;
        }
    }

    auto rfJson = AlumniTree::exportToReactFlowJson(nodeToExport);
    rfJson["rootNodeId"] = nodeId;
    rfJson["hierarchyType"] = hierarchyType;
    rfJson["filterApplied"] = hasActiveFilter;
    return rfJson;
}

nlohmann::json TreeService::getNodeDetails(const std::string& nodeId, const std::string& hierarchyType) {
    ensureTreesBuilt();
    const AlumniTree& activeTree = (hierarchyType == "industry") ? industryTree_ : academicTree_;
    auto node = activeTree.findNode(nodeId);
    if (!node) {
        return {{"error", "Node not found"}};
    }
    return node->toJson();
}

} // namespace alumniverse
