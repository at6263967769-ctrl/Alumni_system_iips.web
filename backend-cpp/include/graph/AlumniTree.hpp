#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <optional>
#include "../models/Models.hpp"
#include "../vendor/nlohmann/json.hpp"

namespace alumniverse {

enum class TreeNodeType {
    Root,
    Institution,
    Department,
    Degree,
    Batch,
    Industry,
    Company,
    Role,
    Alumni
};

inline std::string treeNodeTypeToString(TreeNodeType t) {
    switch (t) {
        case TreeNodeType::Root: return "root";
        case TreeNodeType::Institution: return "institution";
        case TreeNodeType::Department: return "department";
        case TreeNodeType::Degree: return "degree";
        case TreeNodeType::Batch: return "batch";
        case TreeNodeType::Industry: return "industry";
        case TreeNodeType::Company: return "company";
        case TreeNodeType::Role: return "role";
        case TreeNodeType::Alumni: return "alumni";
        default: return "node";
    }
}

struct TreeNode {
    std::string id;
    std::string label;
    TreeNodeType type{TreeNodeType::Root};
    std::string parentId;
    std::vector<std::shared_ptr<TreeNode>> children;
    std::optional<Alumni> alumniData;
    int alumniCount{0}; // Total alumni under this subtree

    nlohmann::json toJson() const {
        nlohmann::json j = {
            {"id", id},
            {"label", label},
            {"type", treeNodeTypeToString(type)},
            {"parentId", parentId},
            {"alumniCount", alumniCount}
        };
        if (alumniData.has_value()) {
            j["alumni"] = alumniData->toJson(true);
        }
        nlohmann::json childArr = nlohmann::json::array();
        for (const auto& c : children) {
            childArr.push_back(c->toJson());
        }
        j["children"] = childArr;
        return j;
    }
};

/**
 * @class AlumniTree
 * @brief Manages hierarchical representation and subtree operations for the alumni network.
 */
class AlumniTree {
public:
    AlumniTree() = default;

    /**
     * @brief Builds an academic hierarchy tree:
     * Institution -> Department -> Degree -> Batch -> Alumni
     */
    void buildAcademicTree(const Institution& institution,
                           const std::vector<Department>& departments,
                           const std::vector<Batch>& batches,
                           const std::vector<Alumni>& alumniList);

    /**
     * @brief Builds an industry hierarchy tree:
     * Industry -> Company -> Job Title -> Alumni
     */
    void buildIndustryTree(const std::vector<Company>& companies,
                           const std::vector<Alumni>& alumniList);

    /**
     * @brief Retrieves a node by ID and all its descendants.
     */
    std::shared_ptr<TreeNode> findNode(const std::string& nodeId) const;

    /**
     * @brief Extracts a subtree rooted at nodeId.
     */
    std::shared_ptr<TreeNode> getSubtree(const std::string& nodeId) const;

    /**
     * @brief Filters the tree using multi-criteria filters, pruning unmatched branches
     * while preserving the parent hierarchy paths leading to matching alumni.
     */
    std::shared_ptr<TreeNode> filterTree(const FilterCriteria& criteria) const;

    /**
     * @brief Returns the root of the full tree.
     */
    std::shared_ptr<TreeNode> getRoot() const { return root_; }

    /**
     * @brief Serializes a tree or subtree into React Flow nodes and edges with computed coordinates.
     */
    static nlohmann::json exportToReactFlowJson(const std::shared_ptr<TreeNode>& nodeRoot);

private:
    std::shared_ptr<TreeNode> root_;
    std::unordered_map<std::string, std::shared_ptr<TreeNode>> nodeIndex_;

    void indexNodeRecursive(const std::shared_ptr<TreeNode>& node);
    static std::shared_ptr<TreeNode> cloneNodeShallow(const std::shared_ptr<TreeNode>& orig);
    static bool matchesFilter(const Alumni& alumni, const FilterCriteria& criteria);
    static std::shared_ptr<TreeNode> filterSubtreeRecursive(const std::shared_ptr<TreeNode>& current, const FilterCriteria& criteria);
};

} // namespace alumniverse
