#pragma once

#include <string>
#include <vector>
#include <optional>
#include "../vendor/nlohmann/json.hpp"

namespace alumniverse {

enum class UserRole {
    Admin,
    Alumni,
    Viewer
};

inline std::string roleToString(UserRole role) {
    switch (role) {
        case UserRole::Admin: return "admin";
        case UserRole::Alumni: return "alumni";
        case UserRole::Viewer: return "viewer";
        default: return "viewer";
    }
}

inline UserRole stringToRole(const std::string& str) {
    if (str == "admin") return UserRole::Admin;
    if (str == "alumni") return UserRole::Alumni;
    return UserRole::Viewer;
}

struct User {
    int id{0};
    std::string name;
    std::string email;
    std::string passwordHash;
    UserRole role{UserRole::Viewer};
    std::string createdAt;
    std::string updatedAt;

    nlohmann::json toJson(bool includeHash = false) const {
        nlohmann::json j = {
            {"id", id},
            {"name", name},
            {"email", email},
            {"role", roleToString(role)},
            {"created_at", createdAt},
            {"updated_at", updatedAt}
        };
        if (includeHash) {
            j["password_hash"] = passwordHash;
        }
        return j;
    }
};

struct Institution {
    int id{0};
    std::string name;
    std::string website;
    std::string createdAt;

    nlohmann::json toJson() const {
        return {
            {"id", id},
            {"name", name},
            {"website", website},
            {"created_at", createdAt}
        };
    }
};

struct Department {
    int id{0};
    int institutionId{0};
    std::string name;

    nlohmann::json toJson() const {
        return {
            {"id", id},
            {"institution_id", institutionId},
            {"name", name}
        };
    }
};

struct Batch {
    int id{0};
    int departmentId{0};
    std::string degree;
    int startYear{0};
    int graduationYear{0};

    nlohmann::json toJson() const {
        return {
            {"id", id},
            {"department_id", departmentId},
            {"degree", degree},
            {"start_year", startYear},
            {"graduation_year", graduationYear}
        };
    }
};

struct Company {
    int id{0};
    std::string name;
    std::string website;
    std::string industry;

    nlohmann::json toJson() const {
        return {
            {"id", id},
            {"name", name},
            {"website", website},
            {"industry", industry}
        };
    }
};

struct CareerHistory {
    int id{0};
    int alumniId{0};
    int companyId{0};
    std::string companyName;
    std::string industry;
    std::string jobTitle;
    std::string startDate;
    std::string endDate;
    bool isCurrent{false};

    nlohmann::json toJson() const {
        nlohmann::json j = {
            {"id", id},
            {"alumni_id", alumniId},
            {"company_id", companyId},
            {"company_name", companyName},
            {"industry", industry},
            {"job_title", jobTitle},
            {"start_date", startDate},
            {"is_current", isCurrent}
        };
        if (!endDate.empty()) {
            j["end_date"] = endDate;
        } else {
            j["end_date"] = nullptr;
        }
        return j;
    }
};

struct Skill {
    int id{0};
    std::string name;

    nlohmann::json toJson() const {
        return {
            {"id", id},
            {"name", name}
        };
    }
};

struct AlumniConnection {
    int id{0};
    int sourceAlumniId{0};
    int targetAlumniId{0};
    std::string relationshipType;
    int createdBy{0};
    std::string createdAt;
    std::string targetName;
    std::string targetCompany;
    std::string targetJobTitle;

    nlohmann::json toJson() const {
        return {
            {"id", id},
            {"source_alumni_id", sourceAlumniId},
            {"target_alumni_id", targetAlumniId},
            {"relationship_type", relationshipType},
            {"created_by", createdBy},
            {"created_at", createdAt},
            {"target_name", targetName},
            {"target_company", targetCompany},
            {"target_job_title", targetJobTitle}
        };
    }
};

struct Alumni {
    int id{0};
    std::optional<int> userId;
    int institutionId{0};
    std::string institutionName;
    int departmentId{0};
    std::string departmentName;
    int batchId{0};
    std::string degree;
    int startYear{0};
    int graduationYear{0};
    std::string fullName;
    std::string currentLocation;
    std::string bio;
    std::string linkedinUrl;
    std::string websiteUrl;
    std::string avatarUrl;
    std::string verificationStatus{"pending"};
    std::string lastVerifiedAt;
    std::string createdAt;
    std::string updatedAt;

    // Computed / hydrated fields
    std::string currentCompany;
    std::string currentJobTitle;
    std::string currentIndustry;
    std::vector<CareerHistory> careerHistory;
    std::vector<std::string> skills;
    std::vector<AlumniConnection> connections;

    nlohmann::json toJson(bool summaryOnly = false) const {
        nlohmann::json j = {
            {"id", id},
            {"full_name", fullName},
            {"institution_id", institutionId},
            {"institution_name", institutionName},
            {"department_id", departmentId},
            {"department_name", departmentName},
            {"batch_id", batchId},
            {"degree", degree},
            {"graduation_year", graduationYear},
            {"current_company", currentCompany},
            {"current_job_title", currentJobTitle},
            {"current_industry", currentIndustry},
            {"current_location", currentLocation},
            {"avatar_url", avatarUrl},
            {"verification_status", verificationStatus},
            {"skills", skills}
        };

        if (userId.has_value()) {
            j["user_id"] = userId.value();
        } else {
            j["user_id"] = nullptr;
        }

        if (!summaryOnly) {
            j["bio"] = bio;
            j["linkedin_url"] = linkedinUrl;
            j["website_url"] = websiteUrl;
            j["last_verified_at"] = lastVerifiedAt.empty() ? nullptr : nlohmann::json(lastVerifiedAt);
            j["created_at"] = createdAt;
            j["updated_at"] = updatedAt;

            nlohmann::json chList = nlohmann::json::array();
            for (const auto& ch : careerHistory) {
                chList.push_back(ch.toJson());
            }
            j["career_history"] = chList;

            nlohmann::json connList = nlohmann::json::array();
            for (const auto& conn : connections) {
                connList.push_back(conn.toJson());
            }
            j["connections"] = connList;
        }

        return j;
    }
};

struct AuditLog {
    int id{0};
    std::optional<int> actorUserId;
    std::string actorName;
    std::string action;
    std::string entityType;
    int entityId{0};
    std::string details;
    std::string createdAt;

    nlohmann::json toJson() const {
        nlohmann::json j = {
            {"id", id},
            {"action", action},
            {"entity_type", entityType},
            {"entity_id", entityId},
            {"details", details},
            {"created_at", createdAt},
            {"actor_name", actorName.empty() ? "System" : actorName}
        };
        if (actorUserId.has_value()) {
            j["actor_user_id"] = actorUserId.value();
        } else {
            j["actor_user_id"] = nullptr;
        }
        return j;
    }
};

struct FilterCriteria {
    std::optional<std::string> searchKeyword;
    std::optional<std::string> institution;
    std::optional<std::string> department;
    std::optional<std::string> degree;
    std::optional<int> graduationYear;
    std::optional<int> batchId;
    std::optional<std::string> company;
    std::optional<std::string> jobTitle;
    std::optional<std::string> industry;
    std::vector<std::string> skills;
    bool skillsAndLogic{false}; // false = OR logic, true = AND logic
    std::optional<std::string> verificationStatus;
    std::optional<std::string> location;

    int page{1};
    int limit{20};
    std::string sortBy{"full_name"}; // "full_name", "graduation_year", "updated_at"
    std::string sortOrder{"asc"}; // "asc", "desc"
};

} // namespace alumniverse
