#pragma once

#include <vector>
#include <unordered_map>
#include <mutex>
#include <optional>
#include <memory>
#include "../models/Models.hpp"

namespace alumniverse {

class DataRepository {
public:
    DataRepository();

    // Health & Info
    bool isDatabaseConnected() const;
    std::string getDatabaseStatus() const;

    // Users
    std::optional<User> findUserByEmail(const std::string& email);
    std::optional<User> findUserById(int id);
    User createUser(const std::string& name, const std::string& email, const std::string& passwordHash, UserRole role);

    // Institutions, Departments, Batches
    std::vector<Institution> getInstitutions();
    std::vector<Department> getDepartments(int institutionId = 0);
    std::vector<Batch> getBatches(int departmentId = 0);
    std::optional<Batch> findBatchById(int batchId);

    // Companies
    std::vector<Company> getCompanies();
    std::optional<Company> findCompanyById(int companyId);
    Company findOrCreateCompany(const std::string& name, const std::string& industry, const std::string& website = "");

    // Alumni
    std::vector<Alumni> getAlumniList(const FilterCriteria& filter, int& totalCount);
    std::optional<Alumni> getAlumniById(int id);
    Alumni createAlumni(const Alumni& newAlumni);
    bool updateAlumni(int id, const Alumni& updatedAlumni);
    bool deleteAlumni(int id);
    bool verifyAlumni(int id, const std::string& status, const std::string& verifiedBy = "Admin");

    // Career History
    bool addCareerHistory(int alumniId, int companyId, const std::string& jobTitle, const std::string& startDate, const std::string& endDate, bool isCurrent);

    // Connections
    std::vector<AlumniConnection> getAlumniConnections(int alumniId);
    std::vector<AlumniConnection> getAllConnections();
    bool addConnection(int sourceId, int targetId, const std::string& type, int createdBy);
    bool removeConnection(int sourceId, int targetId);

    // Audit Logs
    void logAction(std::optional<int> actorId, const std::string& actorName, const std::string& action, const std::string& entityType, int entityId, const std::string& details);
    std::vector<AuditLog> getAuditLogs(int limit = 50);

    // Admin & Analytics
    nlohmann::json getAnalytics();

private:
    void seedInitialData();

    mutable std::mutex mtx_;
    bool dbConnected_{false};
    std::string dbStatusMessage_{"Operating in persistent high-performance in-memory repository (PostgreSQL compatible schema)"};

    int nextUserId_{5};
    int nextInstitutionId_{2};
    int nextDepartmentId_{4};
    int nextBatchId_{8};
    int nextCompanyId_{13};
    int nextAlumniId_{13};
    int nextCareerId_{18};
    int nextConnectionId_{19};
    int nextAuditLogId_{4};

    std::unordered_map<int, User> users_;
    std::unordered_map<int, Institution> institutions_;
    std::unordered_map<int, Department> departments_;
    std::unordered_map<int, Batch> batches_;
    std::unordered_map<int, Company> companies_;
    std::unordered_map<int, Alumni> alumni_;
    std::vector<CareerHistory> careerHistories_;
    std::vector<AlumniConnection> connections_;
    std::vector<AuditLog> auditLogs_;
    std::unordered_map<int, std::vector<std::string>> alumniSkills_;
};

} // namespace alumniverse
