#include "../../include/graph/AlumniTree.hpp"
#include <algorithm>
#include <queue>
#include <cctype>

namespace alumniverse {

static std::string toLower(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower;
}

void AlumniTree::indexNodeRecursive(const std::shared_ptr<TreeNode>& node) {
    if (!node) return;
    nodeIndex_[node->id] = node;
    for (const auto& child : node->children) {
        indexNodeRecursive(child);
    }
}

void AlumniTree::buildAcademicTree(const Institution& institution,
                                  const std::vector<Department>& departments,
                                  const std::vector<Batch>& batches,
                                  const std::vector<Alumni>& alumniList) {
    nodeIndex_.clear();

    root_ = std::make_shared<TreeNode>();
    root_->id = "inst-" + std::to_string(institution.id);
    root_->label = institution.name;
    root_->type = TreeNodeType::Institution;
    root_->parentId = "";

    // Map department id to department node
    std::unordered_map<int, std::shared_ptr<TreeNode>> deptNodes;
    for (const auto& dept : departments) {
        auto dNode = std::make_shared<TreeNode>();
        dNode->id = "dept-" + std::to_string(dept.id);
        dNode->label = dept.name;
        dNode->type = TreeNodeType::Department;
        dNode->parentId = root_->id;
        root_->children.push_back(dNode);
        deptNodes[dept.id] = dNode;
    }

    // Map batch id to batch node
    std::unordered_map<int, std::shared_ptr<TreeNode>> batchNodes;
    for (const auto& batch : batches) {
        auto dIt = deptNodes.find(batch.departmentId);
        if (dIt == deptNodes.end()) continue;

        auto bNode = std::make_shared<TreeNode>();
        bNode->id = "batch-" + std::to_string(batch.id);
        bNode->label = batch.degree + " (" + std::to_string(batch.startYear) + " - " + std::to_string(batch.graduationYear) + ")";
        bNode->type = TreeNodeType::Batch;
        bNode->parentId = dIt->second->id;

        dIt->second->children.push_back(bNode);
        batchNodes[batch.id] = bNode;
    }

    // Attach alumni to batches
    for (const auto& al : alumniList) {
        auto bIt = batchNodes.find(al.batchId);
        if (bIt == batchNodes.end()) continue;

        auto aNode = std::make_shared<TreeNode>();
        aNode->id = "alumni-" + std::to_string(al.id);
        aNode->label = al.fullName;
        aNode->type = TreeNodeType::Alumni;
        aNode->parentId = bIt->second->id;
        aNode->alumniData = al;
        aNode->alumniCount = 1;

        bIt->second->children.push_back(aNode);
    }

    // Compute alumni counts bottom up
    auto computeCounts = [](auto& self, const std::shared_ptr<TreeNode>& node) -> int {
        if (!node) return 0;
        if (node->type == TreeNodeType::Alumni) return 1;
        int sum = 0;
        for (const auto& child : node->children) {
            sum += self(self, child);
        }
        node->alumniCount = sum;
        return sum;
    };
    computeCounts(computeCounts, root_);

    indexNodeRecursive(root_);
}

void AlumniTree::buildIndustryTree(const std::vector<Company>& companies,
                                  const std::vector<Alumni>& alumniList) {
    nodeIndex_.clear();

    root_ = std::make_shared<TreeNode>();
    root_->id = "industry-root";
    root_->label = "Industry & Professional Network";
    root_->type = TreeNodeType::Root;
    root_->parentId = "";

    std::unordered_map<std::string, std::shared_ptr<TreeNode>> industryNodes;
    std::unordered_map<std::string, std::shared_ptr<TreeNode>> companyNodes;

    for (const auto& comp : companies) {
        std::string indName = comp.industry.empty() ? "Other" : comp.industry;
        if (industryNodes.find(indName) == industryNodes.end()) {
            auto indNode = std::make_shared<TreeNode>();
            indNode->id = "ind-" + indName;
            indNode->label = indName;
            indNode->type = TreeNodeType::Industry;
            indNode->parentId = root_->id;
            root_->children.push_back(indNode);
            industryNodes[indName] = indNode;
        }

        std::string compKey = indName + "::" + comp.name;
        if (companyNodes.find(compKey) == companyNodes.end()) {
            auto cNode = std::make_shared<TreeNode>();
            cNode->id = "comp-" + std::to_string(comp.id);
            cNode->label = comp.name;
            cNode->type = TreeNodeType::Company;
            cNode->parentId = industryNodes[indName]->id;
            industryNodes[indName]->children.push_back(cNode);
            companyNodes[compKey] = cNode;
        }
    }

    for (const auto& al : alumniList) {
        std::string indName = al.currentIndustry.empty() ? "Other" : al.currentIndustry;
        std::string compName = al.currentCompany.empty() ? "Independent" : al.currentCompany;
        std::string compKey = indName + "::" + compName;

        auto cIt = companyNodes.find(compKey);
        if (cIt == companyNodes.end()) {
            // Create fallback company under industry
            if (industryNodes.find(indName) == industryNodes.end()) {
                auto indNode = std::make_shared<TreeNode>();
                indNode->id = "ind-" + indName;
                indNode->label = indName;
                indNode->type = TreeNodeType::Industry;
                indNode->parentId = root_->id;
                root_->children.push_back(indNode);
                industryNodes[indName] = indNode;
            }
            auto cNode = std::make_shared<TreeNode>();
            cNode->id = "comp-fb-" + compName;
            cNode->label = compName;
            cNode->type = TreeNodeType::Company;
            cNode->parentId = industryNodes[indName]->id;
            industryNodes[indName]->children.push_back(cNode);
            companyNodes[compKey] = cNode;
            cIt = companyNodes.find(compKey);
        }

        auto aNode = std::make_shared<TreeNode>();
        aNode->id = "alumni-" + std::to_string(al.id);
        aNode->label = al.fullName + " (" + al.currentJobTitle + ")";
        aNode->type = TreeNodeType::Alumni;
        aNode->parentId = cIt->second->id;
        aNode->alumniData = al;
        aNode->alumniCount = 1;
        cIt->second->children.push_back(aNode);
    }

    auto computeCounts = [](auto& self, const std::shared_ptr<TreeNode>& node) -> int {
        if (!node) return 0;
        if (node->type == TreeNodeType::Alumni) return 1;
        int sum = 0;
        for (const auto& child : node->children) {
            sum += self(self, child);
        }
        node->alumniCount = sum;
        return sum;
    };
    computeCounts(computeCounts, root_);

    indexNodeRecursive(root_);
}

std::shared_ptr<TreeNode> AlumniTree::findNode(const std::string& nodeId) const {
    auto it = nodeIndex_.find(nodeId);
    if (it != nodeIndex_.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<TreeNode> AlumniTree::getSubtree(const std::string& nodeId) const {
    auto target = findNode(nodeId);
    if (!target) return nullptr;
    return target;
}

bool AlumniTree::matchesFilter(const Alumni& al, const FilterCriteria& criteria) {
    if (criteria.searchKeyword.has_value() && !criteria.searchKeyword.value().empty()) {
        std::string kw = toLower(criteria.searchKeyword.value());
        bool match = (toLower(al.fullName).find(kw) != std::string::npos) ||
                     (toLower(al.currentCompany).find(kw) != std::string::npos) ||
                     (toLower(al.currentJobTitle).find(kw) != std::string::npos) ||
                     (toLower(al.departmentName).find(kw) != std::string::npos) ||
                     (toLower(al.bio).find(kw) != std::string::npos);
        if (!match) return false;
    }

    if (criteria.institution.has_value() && !criteria.institution.value().empty()) {
        if (toLower(al.institutionName).find(toLower(criteria.institution.value())) == std::string::npos) {
            return false;
        }
    }

    if (criteria.department.has_value() && !criteria.department.value().empty()) {
        if (toLower(al.departmentName).find(toLower(criteria.department.value())) == std::string::npos) {
            return false;
        }
    }

    if (criteria.degree.has_value() && !criteria.degree.value().empty()) {
        if (toLower(al.degree).find(toLower(criteria.degree.value())) == std::string::npos) {
            return false;
        }
    }

    if (criteria.graduationYear.has_value()) {
        if (al.graduationYear != criteria.graduationYear.value()) {
            return false;
        }
    }

    if (criteria.batchId.has_value()) {
        if (al.batchId != criteria.batchId.value()) {
            return false;
        }
    }

    if (criteria.company.has_value() && !criteria.company.value().empty()) {
        if (toLower(al.currentCompany).find(toLower(criteria.company.value())) == std::string::npos) {
            return false;
        }
    }

    if (criteria.jobTitle.has_value() && !criteria.jobTitle.value().empty()) {
        if (toLower(al.currentJobTitle).find(toLower(criteria.jobTitle.value())) == std::string::npos) {
            return false;
        }
    }

    if (criteria.industry.has_value() && !criteria.industry.value().empty()) {
        if (toLower(al.currentIndustry).find(toLower(criteria.industry.value())) == std::string::npos) {
            return false;
        }
    }

    if (criteria.verificationStatus.has_value() && !criteria.verificationStatus.value().empty()) {
        if (toLower(al.verificationStatus) != toLower(criteria.verificationStatus.value())) {
            return false;
        }
    }

    if (criteria.location.has_value() && !criteria.location.value().empty()) {
        if (toLower(al.currentLocation).find(toLower(criteria.location.value())) == std::string::npos) {
            return false;
        }
    }

    if (!criteria.skills.empty()) {
        if (criteria.skillsAndLogic) {
            // AND logic: alumni must have all selected skills
            for (const auto& sk : criteria.skills) {
                std::string targetSkill = toLower(sk);
                bool hasSk = false;
                for (const auto& s : al.skills) {
                    if (toLower(s).find(targetSkill) != std::string::npos) {
                        hasSk = true;
                        break;
                    }
                }
                if (!hasSk) return false;
            }
        } else {
            // OR logic: alumni must have at least one selected skill
            bool hasAny = false;
            for (const auto& sk : criteria.skills) {
                std::string targetSkill = toLower(sk);
                for (const auto& s : al.skills) {
                    if (toLower(s).find(targetSkill) != std::string::npos) {
                        hasAny = true;
                        break;
                    }
                }
                if (hasAny) break;
            }
            if (!hasAny) return false;
        }
    }

    return true;
}

std::shared_ptr<TreeNode> AlumniTree::cloneNodeShallow(const std::shared_ptr<TreeNode>& orig) {
    if (!orig) return nullptr;
    auto copy = std::make_shared<TreeNode>();
    copy->id = orig->id;
    copy->label = orig->label;
    copy->type = orig->type;
    copy->parentId = orig->parentId;
    copy->alumniData = orig->alumniData;
    copy->alumniCount = orig->alumniCount;
    return copy;
}

std::shared_ptr<TreeNode> AlumniTree::filterSubtreeRecursive(const std::shared_ptr<TreeNode>& current, const FilterCriteria& criteria) {
    if (!current) return nullptr;

    if (current->type == TreeNodeType::Alumni) {
        if (current->alumniData.has_value() && matchesFilter(current->alumniData.value(), criteria)) {
            return cloneNodeShallow(current);
        }
        return nullptr;
    }

    auto copy = cloneNodeShallow(current);
    int totalCount = 0;
    for (const auto& child : current->children) {
        auto filteredChild = filterSubtreeRecursive(child, criteria);
        if (filteredChild) {
            copy->children.push_back(filteredChild);
            totalCount += filteredChild->alumniCount;
        }
    }

    if (copy->children.empty()) {
        return nullptr; // Prune branch with 0 matches
    }

    copy->alumniCount = totalCount;
    return copy;
}

std::shared_ptr<TreeNode> AlumniTree::filterTree(const FilterCriteria& criteria) const {
    if (!root_) return nullptr;
    return filterSubtreeRecursive(root_, criteria);
}

nlohmann::json AlumniTree::exportToReactFlowJson(const std::shared_ptr<TreeNode>& nodeRoot) {
    if (!nodeRoot) {
        return {
            {"nodes", nlohmann::json::array()},
            {"edges", nlohmann::json::array()},
            {"stats", {{"nodeCount", 0}, {"edgeCount", 0}}}
        };
    }

    nlohmann::json rfNodes = nlohmann::json::array();
    nlohmann::json rfEdges = nlohmann::json::array();

    // Perform level-by-level BFS to assign tree coordinates
    std::vector<std::vector<std::shared_ptr<TreeNode>>> levels;
    std::unordered_map<std::string, int> nodeLevel;
    std::queue<std::shared_ptr<TreeNode>> q;

    q.push(nodeRoot);
    nodeLevel[nodeRoot->id] = 0;

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        int lvl = nodeLevel[curr->id];
        if (lvl >= static_cast<int>(levels.size())) {
            levels.resize(lvl + 1);
        }
        levels[lvl].push_back(curr);

        for (const auto& child : curr->children) {
            nodeLevel[child->id] = lvl + 1;
            q.push(child);
        }
    }

    const double LEVEL_HEIGHT = 160.0;
    const double NODE_SPACING_X = 260.0;

    for (size_t lvl = 0; lvl < levels.size(); lvl++) {
        const auto& levelNodes = levels[lvl];
        double totalWidth = levelNodes.size() * NODE_SPACING_X;
        double startX = 600.0 - (totalWidth / 2.0);

        for (size_t i = 0; i < levelNodes.size(); i++) {
            const auto& node = levelNodes[i];
            double posX = startX + (i * NODE_SPACING_X);
            double posY = 80.0 + (lvl * LEVEL_HEIGHT);

            std::string nodeType = (node->type == TreeNodeType::Alumni) ? "alumniNode" : "hierarchyNode";

            nlohmann::json n = {
                {"id", node->id},
                {"type", nodeType},
                {"position", {{"x", std::round(posX)}, {"y", std::round(posY)}}},
                {"data", {
                    {"label", node->label},
                    {"type", treeNodeTypeToString(node->type)},
                    {"alumniCount", node->alumniCount},
                    {"id", node->id}
                }}
            };

            if (node->alumniData.has_value()) {
                n["data"]["alumni"] = node->alumniData->toJson(false);
            }

            rfNodes.push_back(n);

            if (!node->parentId.empty()) {
                std::string edgeId = "edge-" + node->parentId + "-" + node->id;
                nlohmann::json e = {
                    {"id", edgeId},
                    {"source", node->parentId},
                    {"target", node->id},
                    {"type", "smoothstep"},
                    {"animated", (node->type == TreeNodeType::Alumni)},
                    {"style", {{"stroke", (node->type == TreeNodeType::Alumni ? "#00D2FF" : "#9D00FF")}, {"strokeWidth", 2}}}
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
