#define CROW_MAIN
#define CROW_FILESYSTEM_IS_EXPERIMENTAL

#include "../include/vendor/crow_all.h"
#include "../include/vendor/nlohmann/json.hpp"
#include "../include/models/Models.hpp"
#include "../include/repositories/DataRepository.hpp"
#include "../include/services/AuthService.hpp"
#include "../include/services/GraphService.hpp"
#include "../include/services/TreeService.hpp"
#include "../include/utils/CsvUtils.hpp"

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

using namespace alumniverse;

// Helper function to set CORS headers on any crow::response
void setCorsHeaders(crow::response& res) {
    res.add_header("Access-Control-Allow-Origin", "*");
    res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With");
    res.add_header("Content-Type", "application/json");
}

crow::response jsonResponse(int code, const nlohmann::json& body) {
    crow::response res(code, body.dump());
    setCorsHeaders(res);
    return res;
}

crow::response errorResponse(int code, const std::string& message) {
    nlohmann::json err = {
        {"status", "error"},
        {"code", code},
        {"message", message}
    };
    return jsonResponse(code, err);
}

crow::response successResponse(const nlohmann::json& data, int code = 200) {
    nlohmann::json res = {
        {"status", "success"},
        {"data", data}
    };
    return jsonResponse(code, res);
}

FilterCriteria parseFilterFromQuery(const crow::query_string& qs) {
    FilterCriteria f;
    if (qs.get("search")) f.searchKeyword = qs.get("search");
    if (qs.get("institution")) f.institution = qs.get("institution");
    if (qs.get("department")) f.department = qs.get("department");
    if (qs.get("degree")) f.degree = qs.get("degree");
    if (qs.get("company")) f.company = qs.get("company");
    if (qs.get("job_title")) f.jobTitle = qs.get("job_title");
    if (qs.get("industry")) f.industry = qs.get("industry");
    if (qs.get("location")) f.location = qs.get("location");
    if (qs.get("verification_status")) f.verificationStatus = qs.get("verification_status");

    if (qs.get("grad_year")) {
        try { f.graduationYear = std::stoi(qs.get("grad_year")); } catch (...) {}
    }
    if (qs.get("batch_id")) {
        try { f.batchId = std::stoi(qs.get("batch_id")); } catch (...) {}
    }
    if (qs.get("page")) {
        try { f.page = std::max(1, std::stoi(qs.get("page"))); } catch (...) {}
    }
    if (qs.get("limit")) {
        try { f.limit = std::max(1, std::min(100, std::stoi(qs.get("limit")))); } catch (...) {}
    }
    if (qs.get("sort_by")) f.sortBy = qs.get("sort_by");
    if (qs.get("sort_order")) f.sortOrder = qs.get("sort_order");

    if (qs.get("skills_logic") && std::string(qs.get("skills_logic")) == "and") {
        f.skillsAndLogic = true;
    }

    if (qs.get("skills")) {
        std::string skStr = qs.get("skills");
        std::istringstream ss(skStr);
        std::string token;
        while (std::getline(ss, token, ',')) {
            while (!token.empty() && std::isspace(static_cast<unsigned char>(token.front()))) token.erase(token.begin());
            while (!token.empty() && std::isspace(static_cast<unsigned char>(token.back()))) token.pop_back();
            if (!token.empty()) f.skills.push_back(token);
        }
    }

    return f;
}

int main() {
    std::cout << "====================================================\n";
    std::cout << "   AlumniVerse C++ Backend Server (Crow Engine)    \n";
    std::cout << "====================================================\n";

    // 1. Initialize Dependency Injection Layers
    auto repo = std::make_shared<DataRepository>();
    auto authService = std::make_shared<AuthService>(repo);
    auto graphService = std::make_shared<GraphService>(repo);
    auto treeService = std::make_shared<TreeService>(repo);

    crow::App<crow::CORSHandler> app;
    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("*")
        .methods("POST"_method, "GET"_method, "PUT"_method, "DELETE"_method, "OPTIONS"_method)
        .headers("Content-Type", "Authorization", "X-Requested-With", "Accept", "Origin");

    auto makeOptionsResponse = []() {
        crow::response res(200, "{\"status\":\"ok\"}");
        setCorsHeaders(res);
        return res;
    };

    // Explicit and catch-all OPTIONS preflight handlers for CORS
    CROW_ROUTE(app, "/<path>").methods("OPTIONS"_method)
    ([&makeOptionsResponse](const crow::request&, const std::string&) {
        return makeOptionsResponse();
    });
    CROW_ROUTE(app, "/api/v1/<path>").methods("OPTIONS"_method)
    ([&makeOptionsResponse](const crow::request&, const std::string&) {
        return makeOptionsResponse();
    });
    CROW_ROUTE(app, "/api/v1/<path>/<path>").methods("OPTIONS"_method)
    ([&makeOptionsResponse](const crow::request&, const std::string&, const std::string&) {
        return makeOptionsResponse();
    });
    CROW_ROUTE(app, "/api/v1/<path>/<path>/<path>").methods("OPTIONS"_method)
    ([&makeOptionsResponse](const crow::request&, const std::string&, const std::string&, const std::string&) {
        return makeOptionsResponse();
    });
    CROW_ROUTE(app, "/api/v1/<path>/<path>/<path>/<path>").methods("OPTIONS"_method)
    ([&makeOptionsResponse](const crow::request&, const std::string&, const std::string&, const std::string&, const std::string&) {
        return makeOptionsResponse();
    });

    // -------------------------------------------------------------
    // Health Check Endpoints
    // -------------------------------------------------------------
    auto handleHealth = [&repo](const crow::request& req) {
        nlohmann::json healthData = {
            {"status", "healthy"},
            {"service", "AlumniVerse C++ Backend"},
            {"version", "1.0.0"},
            {"database", {
                {"connected", repo->isDatabaseConnected()},
                {"status", repo->getDatabaseStatus()}
            }}
        };
        return successResponse(healthData);
    };

    CROW_ROUTE(app, "/").methods("GET"_method)([](const crow::request& req) {
        nlohmann::json info = {
            {"status", "online"},
            {"service", "AlumniVerse C++ Crow Backend Engine"},
            {"version", "1.0.0"},
            {"health_endpoint", "/health"},
            {"api_base", "/api/v1"}
        };
        return successResponse(info);
    });

    CROW_ROUTE(app, "/health").methods("GET"_method)(handleHealth);
    CROW_ROUTE(app, "/api/v1/health").methods("GET"_method)(handleHealth);

    // -------------------------------------------------------------
    // Authentication Endpoints
    // -------------------------------------------------------------
    CROW_ROUTE(app, "/api/v1/auth/register").methods("POST"_method)
    ([authService](const crow::request& req) {
        try {
            auto body = nlohmann::json::parse(req.body);
            std::string name = body.value("name", "");
            std::string email = body.value("email", "");
            std::string password = body.value("password", "");
            std::string role = body.value("role", "alumni");

            auto res = authService->registerUser(name, email, password, role);
            if (!res.success) {
                return errorResponse(400, res.errorMessage);
            }

            nlohmann::json ret = {
                {"token", res.token},
                {"user", res.user->toJson()}
            };
            return successResponse(ret, 201);
        } catch (...) {
            return errorResponse(400, "Invalid JSON payload for user registration.");
        }
    });

    CROW_ROUTE(app, "/api/v1/auth/login").methods("POST"_method)
    ([authService](const crow::request& req) {
        try {
            auto body = nlohmann::json::parse(req.body);
            std::string email = body.value("email", "");
            std::string password = body.value("password", "");

            auto res = authService->loginUser(email, password);
            if (!res.success) {
                return errorResponse(401, res.errorMessage);
            }

            nlohmann::json ret = {
                {"token", res.token},
                {"user", res.user->toJson()}
            };
            return successResponse(ret);
        } catch (...) {
            return errorResponse(400, "Invalid JSON payload for user login.");
        }
    });

    CROW_ROUTE(app, "/api/v1/auth/logout").methods("POST"_method)
    ([](const crow::request& req) {
        return successResponse({{"message", "Successfully logged out."}});
    });

    CROW_ROUTE(app, "/api/v1/auth/me").methods("GET"_method)
    ([authService](const crow::request& req) {
        std::string authHeader = req.get_header_value("Authorization");
        auto userOpt = authService->validateToken(authHeader);
        if (!userOpt.has_value()) {
            return errorResponse(401, "Unauthorized: invalid or missing authentication token.");
        }
        return successResponse(userOpt->toJson());
    });

    // -------------------------------------------------------------
    // Alumni Endpoints
    // -------------------------------------------------------------
    CROW_ROUTE(app, "/api/v1/alumni").methods("GET"_method)
    ([repo](const crow::request& req) {
        auto filter = parseFilterFromQuery(req.url_params);
        int totalCount = 0;
        auto list = repo->getAlumniList(filter, totalCount);

        nlohmann::json items = nlohmann::json::array();
        for (const auto& a : list) {
            items.push_back(a.toJson(false));
        }

        nlohmann::json res = {
            {"items", items},
            {"pagination", {
                {"page", filter.page},
                {"limit", filter.limit},
                {"total", totalCount},
                {"pages", (totalCount + filter.limit - 1) / filter.limit}
            }}
        };
        return successResponse(res);
    });

    CROW_ROUTE(app, "/api/v1/alumni/<int>").methods("GET"_method)
    ([repo](const crow::request& req, int id) {
        auto alOpt = repo->getAlumniById(id);
        if (!alOpt.has_value()) {
            return errorResponse(404, "Alumni record not found with ID: " + std::to_string(id));
        }
        return successResponse(alOpt->toJson(false));
    });

    CROW_ROUTE(app, "/api/v1/alumni").methods("POST"_method)
    ([repo, authService, graphService, treeService](const crow::request& req) {
        std::string authHeader = req.get_header_value("Authorization");
        auto user = authService->validateToken(authHeader);
        if (!user.has_value() || user->role == UserRole::Viewer) {
            return errorResponse(403, "Forbidden: Only authorized alumni or admins can create profiles.");
        }

        try {
            auto body = nlohmann::json::parse(req.body);
            Alumni a;
            a.fullName = body.value("full_name", "");
            a.institutionId = body.value("institution_id", 1);
            a.departmentId = body.value("department_id", 1);
            a.batchId = body.value("batch_id", 1);
            a.currentLocation = body.value("current_location", "");
            a.bio = body.value("bio", "");
            a.linkedinUrl = body.value("linkedin_url", "");
            a.websiteUrl = body.value("website_url", "");
            a.avatarUrl = body.value("avatar_url", "");
            a.currentCompany = body.value("current_company", "");
            a.currentJobTitle = body.value("current_job_title", "");
            a.currentIndustry = body.value("current_industry", "Technology");
            if (body.contains("skills") && body["skills"].is_array()) {
                a.skills = body["skills"].get<std::vector<std::string>>();
            }
            a.verificationStatus = (user->role == UserRole::Admin) ? "verified" : "pending";

            Alumni created = repo->createAlumni(a);
            repo->logAction(user->id, user->name, "CREATE_ALUMNI", "alumni", created.id, "Created alumni profile: " + created.fullName);

            graphService->refreshGraph();
            return successResponse(created.toJson(false), 201);
        } catch (...) {
            return errorResponse(400, "Invalid JSON payload for alumni creation.");
        }
    });

    CROW_ROUTE(app, "/api/v1/alumni/<int>").methods("PUT"_method)
    ([repo, authService, graphService](const crow::request& req, int id) {
        std::string authHeader = req.get_header_value("Authorization");
        auto user = authService->validateToken(authHeader);
        if (!user.has_value() || user->role == UserRole::Viewer) {
            return errorResponse(403, "Forbidden: unauthorized profile edit.");
        }

        auto existing = repo->getAlumniById(id);
        if (!existing.has_value()) {
            return errorResponse(404, "Alumni record not found.");
        }

        try {
            auto body = nlohmann::json::parse(req.body);
            Alumni updated = existing.value();
            if (body.contains("full_name")) updated.fullName = body["full_name"];
            if (body.contains("current_location")) updated.currentLocation = body["current_location"];
            if (body.contains("bio")) updated.bio = body["bio"];
            if (body.contains("linkedin_url")) updated.linkedinUrl = body["linkedin_url"];
            if (body.contains("website_url")) updated.websiteUrl = body["website_url"];
            if (body.contains("avatar_url")) updated.avatarUrl = body["avatar_url"];
            if (body.contains("current_company")) updated.currentCompany = body["current_company"];
            if (body.contains("current_job_title")) updated.currentJobTitle = body["current_job_title"];
            if (body.contains("current_industry")) updated.currentIndustry = body["current_industry"];
            if (body.contains("skills") && body["skills"].is_array()) {
                updated.skills = body["skills"].get<std::vector<std::string>>();
            }

            repo->updateAlumni(id, updated);
            repo->logAction(user->id, user->name, "UPDATE_ALUMNI", "alumni", id, "Updated profile fields for: " + updated.fullName);
            graphService->refreshGraph();

            return successResponse(updated.toJson(false));
        } catch (...) {
            return errorResponse(400, "Invalid payload for updating alumni profile.");
        }
    });

    CROW_ROUTE(app, "/api/v1/alumni/<int>").methods("DELETE"_method)
    ([repo, authService, graphService](const crow::request& req, int id) {
        std::string authHeader = req.get_header_value("Authorization");
        auto user = authService->validateToken(authHeader);
        if (!user.has_value() || user->role != UserRole::Admin) {
            return errorResponse(403, "Forbidden: Only administrators can delete alumni records.");
        }

        bool deleted = repo->deleteAlumni(id);
        if (!deleted) {
            return errorResponse(404, "Alumni not found.");
        }

        repo->logAction(user->id, user->name, "DELETE_ALUMNI", "alumni", id, "Deleted alumni record ID: " + std::to_string(id));
        graphService->refreshGraph();
        return successResponse({{"message", "Alumni record deleted successfully."}});
    });

    // -------------------------------------------------------------
    // Tree Endpoints
    // -------------------------------------------------------------
    CROW_ROUTE(app, "/api/v1/tree").methods("GET"_method)
    ([treeService](const crow::request& req) {
        std::string hType = req.url_params.get("hierarchy") ? req.url_params.get("hierarchy") : "academic";
        auto filter = parseFilterFromQuery(req.url_params);
        auto treeJson = treeService->getTree(hType, filter);
        return successResponse(treeJson);
    });

    CROW_ROUTE(app, "/api/v1/tree/<string>/subtree").methods("GET"_method)
    ([treeService](const crow::request& req, const std::string& nodeId) {
        std::string hType = req.url_params.get("hierarchy") ? req.url_params.get("hierarchy") : "academic";
        auto filter = parseFilterFromQuery(req.url_params);
        auto subtreeJson = treeService->getSubtree(nodeId, hType, filter);
        return successResponse(subtreeJson);
    });

    CROW_ROUTE(app, "/api/v1/tree/<string>").methods("GET"_method)
    ([treeService](const crow::request& req, const std::string& nodeId) {
        std::string hType = req.url_params.get("hierarchy") ? req.url_params.get("hierarchy") : "academic";
        auto details = treeService->getNodeDetails(nodeId, hType);
        return successResponse(details);
    });

    // -------------------------------------------------------------
    // Graph Endpoints
    // -------------------------------------------------------------
    CROW_ROUTE(app, "/api/v1/graph").methods("GET"_method)
    ([graphService](const crow::request& req) {
        auto filter = parseFilterFromQuery(req.url_params);
        auto graphJson = graphService->getGraph(filter);
        return successResponse(graphJson);
    });

    CROW_ROUTE(app, "/api/v1/graph/<int>/neighbors").methods("GET"_method)
    ([graphService](const crow::request& req, int alumniId) {
        auto neighborsJson = graphService->getNeighbors(alumniId);
        return successResponse(neighborsJson);
    });

    CROW_ROUTE(app, "/api/v1/graph/<int>/connections").methods("GET"_method)
    ([repo](const crow::request& req, int alumniId) {
        auto conns = repo->getAlumniConnections(alumniId);
        nlohmann::json list = nlohmann::json::array();
        for (const auto& c : conns) list.push_back(c.toJson());
        return successResponse({{"connections", list}, {"count", list.size()}});
    });

    CROW_ROUTE(app, "/api/v1/graph/path").methods("GET"_method)
    ([graphService](const crow::request& req) {
        if (!req.url_params.get("source") || !req.url_params.get("target")) {
            return errorResponse(400, "Missing required query parameters: source and target.");
        }
        try {
            int sourceId = std::stoi(req.url_params.get("source"));
            int targetId = std::stoi(req.url_params.get("target"));
            auto pathJson = graphService->findShortestPath(sourceId, targetId);
            return successResponse(pathJson);
        } catch (...) {
            return errorResponse(400, "Source and target parameters must be integer IDs.");
        }
    });

    // -------------------------------------------------------------
    // Companies & Metadata Endpoints
    // -------------------------------------------------------------
    CROW_ROUTE(app, "/api/v1/companies").methods("GET"_method)
    ([repo](const crow::request& req) {
        auto comps = repo->getCompanies();
        nlohmann::json list = nlohmann::json::array();
        for (const auto& c : comps) list.push_back(c.toJson());
        return successResponse(list);
    });

    CROW_ROUTE(app, "/api/v1/companies/<int>/alumni").methods("GET"_method)
    ([repo](const crow::request& req, int compId) {
        auto comp = repo->findCompanyById(compId);
        if (!comp.has_value()) {
            return errorResponse(404, "Company not found.");
        }
        FilterCriteria f;
        f.company = comp->name;
        f.limit = 1000;
        int count = 0;
        auto alList = repo->getAlumniList(f, count);
        nlohmann::json items = nlohmann::json::array();
        for (const auto& a : alList) items.push_back(a.toJson(true));
        return successResponse({{"company", comp->toJson()}, {"alumni", items}, {"count", count}});
    });

    CROW_ROUTE(app, "/api/v1/metadata").methods("GET"_method)
    ([repo](const crow::request& req) {
        auto insts = repo->getInstitutions();
        auto depts = repo->getDepartments();
        auto batches = repo->getBatches();
        auto comps = repo->getCompanies();

        nlohmann::json jInst = nlohmann::json::array();
        for (const auto& i : insts) jInst.push_back(i.toJson());
        nlohmann::json jDepts = nlohmann::json::array();
        for (const auto& d : depts) jDepts.push_back(d.toJson());
        nlohmann::json jBatches = nlohmann::json::array();
        for (const auto& b : batches) jBatches.push_back(b.toJson());
        nlohmann::json jComps = nlohmann::json::array();
        for (const auto& c : comps) jComps.push_back(c.toJson());

        return successResponse({
            {"institutions", jInst},
            {"departments", jDepts},
            {"batches", jBatches},
            {"companies", jComps}
        });
    });

    // -------------------------------------------------------------
    // Administration Endpoints
    // -------------------------------------------------------------
    CROW_ROUTE(app, "/api/v1/admin/analytics").methods("GET"_method)
    ([repo](const crow::request& req) {
        auto analytics = repo->getAnalytics();
        return successResponse(analytics);
    });

    CROW_ROUTE(app, "/api/v1/admin/alumni/<int>/verify").methods("POST"_method)
    ([repo, authService](const crow::request& req, int id) {
        std::string authHeader = req.get_header_value("Authorization");
        auto user = authService->validateToken(authHeader);
        if (!user.has_value() || user->role != UserRole::Admin) {
            return errorResponse(403, "Forbidden: Only admins can verify alumni credentials.");
        }

        std::string status = "verified";
        try {
            if (!req.body.empty()) {
                auto body = nlohmann::json::parse(req.body);
                if (body.contains("status")) status = body["status"];
            }
        } catch (...) {}

        bool ok = repo->verifyAlumni(id, status, user->name);
        if (!ok) {
            return errorResponse(404, "Alumni record not found.");
        }

        repo->logAction(user->id, user->name, "VERIFY_ALUMNI", "alumni", id, "Profile verification status set to: " + status);
        return successResponse({{"message", "Profile verification status successfully updated.", {"status", status}}});
    });

    CROW_ROUTE(app, "/api/v1/admin/import/csv").methods("POST"_method)
    ([repo, authService, graphService, treeService](const crow::request& req) {
        std::string authHeader = req.get_header_value("Authorization");
        auto user = authService->validateToken(authHeader);
        if (!user.has_value() || user->role != UserRole::Admin) {
            return errorResponse(403, "Forbidden: Only administrators can import alumni via CSV.");
        }

        if (req.body.empty()) {
            return errorResponse(400, "Empty CSV payload.");
        }

        auto rows = CsvUtils::parseAlumniCsv(req.body);
        int importedCount = 0;
        int errorCount = 0;
        nlohmann::json details = nlohmann::json::array();

        for (const auto& r : rows) {
            if (!r.isValid) {
                errorCount++;
                details.push_back({{"name", r.fullName}, {"status", "rejected"}, {"reason", r.errorMessage}});
                continue;
            }

            // Create or resolve company
            repo->findOrCreateCompany(r.company, r.industry);

            Alumni a;
            a.fullName = r.fullName;
            a.graduationYear = r.graduationYear;
            a.currentCompany = r.company;
            a.currentJobTitle = r.jobTitle;
            a.currentIndustry = r.industry;
            a.currentLocation = r.location;
            a.linkedinUrl = r.linkedinUrl;
            a.institutionId = 1;
            a.departmentId = 1; // Default mapped department
            a.batchId = 1;      // Default mapped batch
            a.skills = r.skills;
            a.verificationStatus = "verified"; // Admin imported

            Alumni created = repo->createAlumni(a);
            importedCount++;
            details.push_back({{"id", created.id}, {"name", created.fullName}, {"status", "imported"}});
        }

        repo->logAction(user->id, user->name, "IMPORT_CSV", "batch", importedCount,
                        "Imported " + std::to_string(importedCount) + " alumni records via CSV.");

        graphService->refreshGraph();

        return successResponse({
            {"imported_count", importedCount},
            {"error_count", errorCount},
            {"details", details}
        });
    });

    CROW_ROUTE(app, "/api/v1/admin/connections").methods("POST"_method)
    ([repo, authService, graphService](const crow::request& req) {
        std::string authHeader = req.get_header_value("Authorization");
        auto user = authService->validateToken(authHeader);
        if (!user.has_value() || user->role != UserRole::Admin) {
            return errorResponse(403, "Forbidden: Only administrators can confirm new connections.");
        }

        try {
            auto body = nlohmann::json::parse(req.body);
            int src = body.value("source_alumni_id", 0);
            int dst = body.value("target_alumni_id", 0);
            std::string type = body.value("relationship_type", "professional_connection");

            bool ok = repo->addConnection(src, dst, type, user->id);
            if (!ok) {
                return errorResponse(400, "Could not add connection. Ensure valid non-duplicate nodes.");
            }

            repo->logAction(user->id, user->name, "ADD_CONNECTION", "alumni_connections", src, "Connected alumni " + std::to_string(src) + " with " + std::to_string(dst));
            graphService->refreshGraph();

            return successResponse({{"message", "Connection confirmed successfully."}}, 201);
        } catch (...) {
            return errorResponse(400, "Invalid connection payload.");
        }
    });

    CROW_ROUTE(app, "/api/v1/admin/audit-logs").methods("GET"_method)
    ([repo, authService](const crow::request& req) {
        std::string authHeader = req.get_header_value("Authorization");
        auto user = authService->validateToken(authHeader);
        if (!user.has_value() || user->role != UserRole::Admin) {
            return errorResponse(403, "Forbidden: Admin access required.");
        }

        auto logs = repo->getAuditLogs(100);
        nlohmann::json list = nlohmann::json::array();
        for (const auto& l : logs) list.push_back(l.toJson());
        return successResponse(list);
    });

    // -------------------------------------------------------------
    // Server Configuration & Startup
    // -------------------------------------------------------------
    int port = 8080;
    const char* envPort = std::getenv("PORT");
    if (envPort && std::string(envPort).length() > 0) {
        try { port = std::stoi(envPort); } catch (...) {}
    }

    std::cout << "[INFO] Binding AlumniVerse HTTP Server to 0.0.0.0:" << port << "\n";
    std::cout << "[INFO] REST API base: http://0.0.0.0:" << port << "/api/v1\n";
    std::cout << "[INFO] Database status: " << repo->getDatabaseStatus() << "\n";

    app.bindaddr("0.0.0.0").port(port).multithreaded().run();
    return 0;
}
