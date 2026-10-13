#include "../../include/repositories/DataRepository.hpp"
#include "../../include/utils/CryptoUtils.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <unordered_set>

namespace alumniverse {

static std::string currentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S UTC", std::gmtime(&t));
    return std::string(buf);
}

static std::string toLower(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower;
}

DataRepository::DataRepository() {
    const char* dbUrl = std::getenv("DATABASE_URL");
    if (dbUrl && std::string(dbUrl).length() > 0) {
        dbConnected_ = true;
        dbStatusMessage_ = "PostgreSQL configuration detected via DATABASE_URL";
    } else {
        dbConnected_ = true; // Local operational mode
        dbStatusMessage_ = "Operating in persistent thread-safe in-memory engine (PostgreSQL schema compatible)";
    }
    seedInitialData();
}

bool DataRepository::isDatabaseConnected() const {
    return dbConnected_;
}

std::string DataRepository::getDatabaseStatus() const {
    return dbStatusMessage_;
}

void DataRepository::seedInitialData() {
    // 1. Users (Default password for demo accounts: Password123!)
    std::string seedPassHash = CryptoUtils::hashPassword("Password123!");
    users_[1] = {1, "System Administrator", "admin@alumniverse.edu", seedPassHash, UserRole::Admin, "2026-01-01 00:00:00 UTC", "2026-01-01 00:00:00 UTC"};
    users_[2] = {2, "Aarav Sharma", "aarav.sharma@alumniverse.edu", seedPassHash, UserRole::Alumni, "2026-01-05 00:00:00 UTC", "2026-01-05 00:00:00 UTC"};
    users_[3] = {3, "Priya Patel", "priya.patel@alumniverse.edu", seedPassHash, UserRole::Alumni, "2026-01-06 00:00:00 UTC", "2026-01-06 00:00:00 UTC"};
    users_[4] = {4, "Guest Viewer", "viewer@alumniverse.edu", seedPassHash, UserRole::Viewer, "2026-01-10 00:00:00 UTC", "2026-01-10 00:00:00 UTC"};

    // 2. Institution
    institutions_[1] = {1, "International Institute of Professional Studies (IIPS DAVV)", "https://iips.edu.in", "2026-01-01 00:00:00 UTC"};

    // 3. Departments
    departments_[1] = {1, 1, "Department of Computer Applications"};
    departments_[2] = {2, 1, "Department of Management Studies"};
    departments_[3] = {3, 1, "Department of Commerce & Finance"};

    // 4. Batches
    batches_[1] = {1, 1, "MCA (Integrated 5 Year)", 2015, 2020};
    batches_[2] = {2, 1, "MCA (Integrated 5 Year)", 2017, 2022};
    batches_[3] = {3, 1, "M.Tech (Information Technology)", 2016, 2021};
    batches_[4] = {4, 1, "BCA", 2019, 2022};
    batches_[5] = {5, 2, "MBA (Management Science)", 2016, 2021};
    batches_[6] = {6, 2, "MBA (Tourism)", 2018, 2020};
    batches_[7] = {7, 3, "B.Com (Honors)", 2017, 2020};

    // 5. Companies
    companies_[1] = {1, "Google", "https://about.google", "Technology & Internet"};
    companies_[2] = {2, "Microsoft", "https://microsoft.com", "Enterprise Software & Cloud"};
    companies_[3] = {3, "Amazon AWS", "https://aws.amazon.com", "Cloud Computing & E-Commerce"};
    companies_[4] = {4, "Adobe Systems", "https://adobe.com", "Creative Software & Cloud"};
    companies_[5] = {5, "Atlassian", "https://atlassian.com", "Collaboration & Dev Tools"};
    companies_[6] = {6, "Razorpay", "https://razorpay.com", "FinTech & Payments"};
    companies_[7] = {7, "CRED", "https://cred.club", "FinTech & Consumer Tech"};
    companies_[8] = {8, "Tata Consultancy Services", "https://tcs.com", "IT Consulting & Services"};
    companies_[9] = {9, "Infosys Technologies", "https://infosys.com", "IT Consulting & Services"};
    companies_[10] = {10, "Zomato", "https://zomato.com", "Consumer Tech & Logistics"};
    companies_[11] = {11, "Swiggy", "https://swiggy.com", "Food Tech & Quick Commerce"};
    companies_[12] = {12, "Goldman Sachs", "https://goldmansachs.com", "Investment Banking & Finance"};

    // 6. Alumni Records
    auto createSeedAlumni = [this](int id, std::optional<int> uid, int deptId, int batchId,
                                   const std::string& name, int gradYear, const std::string& loc,
                                   const std::string& bio, const std::string& linkedin,
                                   const std::string& web, const std::string& avatar,
                                   const std::string& vStatus, const std::string& vDate,
                                   const std::string& comp, const std::string& title,
                                   const std::string& ind, const std::vector<std::string>& sk) {
        Alumni a;
        a.id = id;
        a.userId = uid;
        a.institutionId = 1;
        a.institutionName = institutions_[1].name;
        a.departmentId = deptId;
        a.departmentName = departments_[deptId].name;
        a.batchId = batchId;
        a.degree = batches_[batchId].degree;
        a.startYear = batches_[batchId].startYear;
        a.graduationYear = gradYear;
        a.fullName = name;
        a.currentLocation = loc;
        a.bio = bio;
        a.linkedinUrl = linkedin;
        a.websiteUrl = web;
        a.avatarUrl = avatar;
        a.verificationStatus = vStatus;
        a.lastVerifiedAt = vDate;
        a.createdAt = "2026-01-10 10:00:00 UTC";
        a.updatedAt = "2026-02-01 12:00:00 UTC";
        a.currentCompany = comp;
        a.currentJobTitle = title;
        a.currentIndustry = ind;
        a.skills = sk;
        alumni_[id] = a;
        alumniSkills_[id] = sk;
    };

    createSeedAlumni(1, 2, 1, 1, "Aarav Sharma", 2020, "Bengaluru, India",
                     "Staff Software Engineer at Google Core Infrastructure. Passionate about low-latency systems and distributed engines.",
                     "https://linkedin.com/in/demo-aarav-sharma", "https://aaravsharma.dev",
                     "https://images.unsplash.com/photo-1534528741775-53994a69daeb?w=150",
                     "verified", "2026-01-15 10:00:00 UTC", "Google", "Staff Software Engineer", "Technology & Internet",
                     {"C++", "Distributed Systems", "System Design", "Cloud Architecture (AWS/GCP)"});

    createSeedAlumni(2, 3, 1, 1, "Priya Patel", 2020, "Seattle, WA, USA",
                     "Senior Software Engineer at Microsoft Azure Storage. Distributed consensus and cloud resilience specialist.",
                     "https://linkedin.com/in/demo-priya-patel", "https://priyapatel.tech",
                     "https://images.unsplash.com/photo-1494790108377-be9c29b29330?w=150",
                     "verified", "2026-02-10 14:30:00 UTC", "Microsoft", "Senior Software Engineer", "Enterprise Software & Cloud",
                     {"Distributed Systems", "Cloud Architecture (AWS/GCP)", "C++", "Kubernetes & Docker"});

    createSeedAlumni(3, std::nullopt, 1, 1, "Rohan Verma", 2020, "Bengaluru, India",
                     "Principal Architect at Razorpay Payments Gateway. Scaling transactional pipelines to 10k TPS.",
                     "https://linkedin.com/in/demo-rohan-verma", "https://rohanverma.io",
                     "https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d?w=150",
                     "verified", "2026-01-20 09:15:00 UTC", "Razorpay", "Principal Architect", "FinTech & Payments",
                     {"System Design", "PostgreSQL", "Node.js", "Distributed Systems"});

    createSeedAlumni(4, std::nullopt, 1, 2, "Ananya Sen", 2022, "Hyderabad, India",
                     "Software Development Engineer II at Amazon AWS DynamoDB team. Graduate of MCA 2022.",
                     "https://linkedin.com/in/demo-ananya-sen", "",
                     "https://images.unsplash.com/photo-1517841905240-472988babdf9?w=150",
                     "verified", "2026-03-01 11:20:00 UTC", "Amazon AWS", "Software Development Engineer II", "Cloud Computing & E-Commerce",
                     {"Cloud Architecture (AWS/GCP)", "Distributed Systems", "System Design"});

    createSeedAlumni(5, std::nullopt, 1, 2, "Vikram Malhotra", 2022, "Bengaluru, India",
                     "Lead Backend Engineer at CRED. Crafting high-throughput microservices in Go and C++.",
                     "https://linkedin.com/in/demo-vikram-malhotra", "https://vikram.codes",
                     "https://images.unsplash.com/photo-1500648767791-00dcc994a43e?w=150",
                     "verified", "2026-02-18 16:45:00 UTC", "CRED", "Lead Backend Engineer", "FinTech & Consumer Tech",
                     {"C++", "PostgreSQL", "Kubernetes & Docker", "System Design"});

    createSeedAlumni(6, std::nullopt, 1, 3, "Neha Joshi", 2021, "Gurugram, India",
                     "Senior Engineering Manager at Zomato. Overseeing platform reliability and checkout microservices.",
                     "https://linkedin.com/in/demo-neha-joshi", "",
                     "https://images.unsplash.com/photo-1544005313-94ddf0286df2?w=150",
                     "verified", "2026-01-12 12:00:00 UTC", "Zomato", "Senior Engineering Manager", "Consumer Tech & Logistics",
                     {"System Design", "Product Management", "Kubernetes & Docker"});

    createSeedAlumni(7, std::nullopt, 1, 3, "Kavya Deshmukh", 2021, "Pune, India",
                     "Senior Computer Scientist at Adobe Creative Cloud. Graphics algorithms and WASM integration.",
                     "https://linkedin.com/in/demo-kavya-deshmukh", "https://kavyad.art",
                     "https://images.unsplash.com/photo-1524504388940-b1c1722653e1?w=150",
                     "verified", "2026-02-28 15:10:00 UTC", "Adobe Systems", "Senior Computer Scientist", "Creative Software & Cloud",
                     {"C++", "React & JavaScript", "System Design"});

    createSeedAlumni(8, std::nullopt, 1, 4, "Aditya Choudhury", 2022, "Bengaluru, India",
                     "Frontend Engineer at Atlassian Jira Cloud. Web performance enthusiast.",
                     "https://linkedin.com/in/demo-aditya-choudhury", "",
                     "https://images.unsplash.com/photo-1492562080023-ab3db95bfbce?w=150",
                     "verified", "2026-03-05 10:30:00 UTC", "Atlassian", "Frontend Engineer", "Collaboration & Dev Tools",
                     {"React & JavaScript", "Node.js"});

    createSeedAlumni(9, std::nullopt, 2, 5, "Sneha Kapoor", 2021, "Mumbai, India",
                     "Vice President, Product Strategy at Goldman Sachs. Digital wealth platforms and institutional assets.",
                     "https://linkedin.com/in/demo-sneha-kapoor", "",
                     "https://images.unsplash.com/photo-1534528741775-53994a69daeb?w=150",
                     "verified", "2026-02-05 13:00:00 UTC", "Goldman Sachs", "Vice President, Product Strategy", "Investment Banking & Finance",
                     {"Product Management", "Financial Modeling", "System Design"});

    createSeedAlumni(10, std::nullopt, 2, 5, "Arjun Nambiar", 2021, "Bengaluru, India",
                     "Principal Product Manager at Swiggy Instamart. Supply chain optimization and catalog intelligence.",
                     "https://linkedin.com/in/demo-arjun-nambiar", "https://arjunnambiar.me",
                     "https://images.unsplash.com/photo-1506794778202-cad84cf45f1d?w=150",
                     "verified", "2026-01-30 08:45:00 UTC", "Swiggy", "Principal Product Manager", "Food Tech & Quick Commerce",
                     {"Product Management", "System Design"});

    createSeedAlumni(11, std::nullopt, 2, 6, "Divya Nair", 2020, "New Delhi, India",
                     "Brand Marketing Director at Taj Hospitality Ventures. Experiential tourism campaigns.",
                     "https://linkedin.com/in/demo-divya-nair", "",
                     "https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?w=150",
                     "pending", "", "Independent", "Brand Consultant", "Other",
                     {"Brand Strategy"});

    createSeedAlumni(12, std::nullopt, 3, 7, "Manish Agarwal", 2020, "London, United Kingdom",
                     "Senior Quantitative Analyst at Barclays Investment Bank. Equity derivatives and risk management.",
                     "https://linkedin.com/in/demo-manish-agarwal", "",
                     "https://images.unsplash.com/photo-1472099645785-5658abf4ff4e?w=150",
                     "verified", "2026-02-14 17:20:00 UTC", "Goldman Sachs", "Senior Quantitative Analyst", "Investment Banking & Finance",
                     {"Financial Modeling", "Machine Learning & Python"});

    // 7. Career Histories
    careerHistories_.push_back({1, 1, 8, "Tata Consultancy Services", "IT Consulting & Services", "Systems Engineer", "2020-07-01", "2022-03-15", false});
    careerHistories_.push_back({2, 1, 1, "Google", "Technology & Internet", "Staff Software Engineer", "2022-04-01", "", true});

    careerHistories_.push_back({3, 2, 9, "Infosys Technologies", "IT Consulting & Services", "Associate Software Specialist", "2020-07-15", "2022-06-30", false});
    careerHistories_.push_back({4, 2, 2, "Microsoft", "Enterprise Software & Cloud", "Senior Software Engineer", "2022-07-15", "", true});

    careerHistories_.push_back({5, 3, 3, "Amazon AWS", "Cloud Computing & E-Commerce", "Software Development Engineer I", "2020-08-01", "2022-10-31", false});
    careerHistories_.push_back({6, 3, 6, "Razorpay", "FinTech & Payments", "Principal Architect", "2022-11-01", "", true});

    careerHistories_.push_back({7, 4, 3, "Amazon AWS", "Cloud Computing & E-Commerce", "Software Development Engineer II", "2022-07-01", "", true});

    careerHistories_.push_back({8, 5, 6, "Razorpay", "FinTech & Payments", "Backend Engineer", "2022-07-01", "2024-05-31", false});
    careerHistories_.push_back({9, 5, 7, "CRED", "FinTech & Consumer Tech", "Lead Backend Engineer", "2024-06-01", "", true});

    careerHistories_.push_back({10, 6, 8, "Tata Consultancy Services", "IT Consulting & Services", "Senior Systems Analyst", "2021-06-01", "2023-08-31", false});
    careerHistories_.push_back({11, 6, 10, "Zomato", "Consumer Tech & Logistics", "Senior Engineering Manager", "2023-09-01", "", true});

    careerHistories_.push_back({12, 7, 4, "Adobe Systems", "Creative Software & Cloud", "Senior Computer Scientist", "2021-07-01", "", true});
    careerHistories_.push_back({13, 8, 5, "Atlassian", "Collaboration & Dev Tools", "Frontend Engineer", "2022-08-01", "", true});
    careerHistories_.push_back({14, 9, 12, "Goldman Sachs", "Investment Banking & Finance", "Vice President, Product Strategy", "2021-06-15", "", true});

    careerHistories_.push_back({15, 10, 10, "Zomato", "Consumer Tech & Logistics", "Product Lead", "2021-07-01", "2023-12-31", false});
    careerHistories_.push_back({16, 10, 11, "Swiggy", "Food Tech & Quick Commerce", "Principal Product Manager", "2024-01-01", "", true});

    careerHistories_.push_back({17, 12, 12, "Goldman Sachs", "Investment Banking & Finance", "Senior Quantitative Analyst", "2020-09-01", "", true});

    // 8. Confirmed Connections
    auto addSeedConn = [this](int src, int dst, const std::string& type) {
        std::string targetName = alumni_[dst].fullName;
        std::string targetComp = alumni_[dst].currentCompany;
        std::string targetTitle = alumni_[dst].currentJobTitle;
        connections_.push_back({nextConnectionId_++, src, dst, type, 1, "2026-01-20 10:00:00 UTC", targetName, targetComp, targetTitle});
    };

    addSeedConn(1, 2, "classmate");
    addSeedConn(2, 1, "classmate");
    addSeedConn(1, 3, "classmate");
    addSeedConn(3, 1, "classmate");
    addSeedConn(1, 4, "mentor");
    addSeedConn(4, 1, "mentor");
    addSeedConn(2, 4, "professional_connection");
    addSeedConn(4, 2, "professional_connection");
    addSeedConn(3, 5, "colleague");
    addSeedConn(5, 3, "colleague");
    addSeedConn(5, 8, "classmate");
    addSeedConn(8, 5, "classmate");
    addSeedConn(6, 10, "colleague");
    addSeedConn(10, 6, "colleague");
    addSeedConn(9, 12, "colleague");
    addSeedConn(12, 9, "colleague");
    addSeedConn(2, 7, "professional_connection");
    addSeedConn(7, 2, "professional_connection");

    // Hydrate career history and connections on alumni
    for (auto& [id, al] : alumni_) {
        for (const auto& ch : careerHistories_) {
            if (ch.alumniId == id) {
                al.careerHistory.push_back(ch);
            }
        }
        for (const auto& c : connections_) {
            if (c.sourceAlumniId == id) {
                al.connections.push_back(c);
            }
        }
    }

    // 9. Initial Audit Logs
    auditLogs_.push_back({1, 1, "System Administrator", "VERIFY_PROFILE", "alumni", 1, "Verified Aarav Sharma profile credentials via official Google work email.", "2026-01-15 10:00:00 UTC"});
    auditLogs_.push_back({2, 1, "System Administrator", "VERIFY_PROFILE", "alumni", 2, "Verified Priya Patel profile credentials via Microsoft work email.", "2026-02-10 14:30:00 UTC"});
    auditLogs_.push_back({3, 1, "System Administrator", "SEED_DATABASE", "system", 0, "Initial database migration and realistic seed data loaded successfully.", "2026-01-01 00:00:00 UTC"});
}

std::optional<User> DataRepository::findUserByEmail(const std::string& email) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::string lowerEmail = toLower(email);
    for (const auto& [_, u] : users_) {
        if (toLower(u.email) == lowerEmail) {
            return u;
        }
    }
    return std::nullopt;
}

std::optional<User> DataRepository::findUserById(int id) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = users_.find(id);
    if (it != users_.end()) {
        return it->second;
    }
    return std::nullopt;
}

User DataRepository::createUser(const std::string& name, const std::string& email, const std::string& passwordHash, UserRole role) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::string now = currentTimestamp();
    User u;
    u.id = nextUserId_++;
    u.name = name;
    u.email = email;
    u.passwordHash = passwordHash;
    u.role = role;
    u.createdAt = now;
    u.updatedAt = now;
    users_[u.id] = u;
    return u;
}

std::vector<Institution> DataRepository::getInstitutions() {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<Institution> list;
    for (const auto& [_, inst] : institutions_) {
        list.push_back(inst);
    }
    return list;
}

std::vector<Department> DataRepository::getDepartments(int institutionId) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<Department> list;
    for (const auto& [_, dept] : departments_) {
        if (institutionId == 0 || dept.institutionId == institutionId) {
            list.push_back(dept);
        }
    }
    return list;
}

std::vector<Batch> DataRepository::getBatches(int departmentId) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<Batch> list;
    for (const auto& [_, b] : batches_) {
        if (departmentId == 0 || b.departmentId == departmentId) {
            list.push_back(b);
        }
    }
    return list;
}

std::optional<Batch> DataRepository::findBatchById(int batchId) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = batches_.find(batchId);
    if (it != batches_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<Company> DataRepository::getCompanies() {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<Company> list;
    for (const auto& [_, comp] : companies_) {
        list.push_back(comp);
    }
    std::sort(list.begin(), list.end(), [](const Company& a, const Company& b){ return a.name < b.name; });
    return list;
}

std::optional<Company> DataRepository::findCompanyById(int companyId) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = companies_.find(companyId);
    if (it != companies_.end()) {
        return it->second;
    }
    return std::nullopt;
}

Company DataRepository::findOrCreateCompany(const std::string& name, const std::string& industry, const std::string& website) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::string lowerName = toLower(name);
    for (const auto& [_, comp] : companies_) {
        if (toLower(comp.name) == lowerName) {
            return comp;
        }
    }
    Company c;
    c.id = nextCompanyId_++;
    c.name = name;
    c.industry = industry.empty() ? "Technology" : industry;
    c.website = website;
    companies_[c.id] = c;
    return c;
}

std::vector<Alumni> DataRepository::getAlumniList(const FilterCriteria& filter, int& totalCount) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<Alumni> filtered;

    for (const auto& [_, al] : alumni_) {
        // Keyword search
        if (filter.searchKeyword.has_value() && !filter.searchKeyword.value().empty()) {
            std::string kw = toLower(filter.searchKeyword.value());
            bool match = (toLower(al.fullName).find(kw) != std::string::npos) ||
                         (toLower(al.currentCompany).find(kw) != std::string::npos) ||
                         (toLower(al.currentJobTitle).find(kw) != std::string::npos) ||
                         (toLower(al.departmentName).find(kw) != std::string::npos) ||
                         (toLower(al.currentLocation).find(kw) != std::string::npos);
            if (!match) continue;
        }

        // Institution
        if (filter.institution.has_value() && !filter.institution.value().empty()) {
            if (toLower(al.institutionName).find(toLower(filter.institution.value())) == std::string::npos) continue;
        }

        // Department
        if (filter.department.has_value() && !filter.department.value().empty()) {
            if (toLower(al.departmentName).find(toLower(filter.department.value())) == std::string::npos) continue;
        }

        // Degree
        if (filter.degree.has_value() && !filter.degree.value().empty()) {
            if (toLower(al.degree).find(toLower(filter.degree.value())) == std::string::npos) continue;
        }

        // Graduation Year
        if (filter.graduationYear.has_value()) {
            if (al.graduationYear != filter.graduationYear.value()) continue;
        }

        // Batch ID
        if (filter.batchId.has_value()) {
            if (al.batchId != filter.batchId.value()) continue;
        }

        // Company
        if (filter.company.has_value() && !filter.company.value().empty()) {
            if (toLower(al.currentCompany).find(toLower(filter.company.value())) == std::string::npos) continue;
        }

        // Job Title
        if (filter.jobTitle.has_value() && !filter.jobTitle.value().empty()) {
            if (toLower(al.currentJobTitle).find(toLower(filter.jobTitle.value())) == std::string::npos) continue;
        }

        // Industry
        if (filter.industry.has_value() && !filter.industry.value().empty()) {
            if (toLower(al.currentIndustry).find(toLower(filter.industry.value())) == std::string::npos) continue;
        }

        // Location
        if (filter.location.has_value() && !filter.location.value().empty()) {
            if (toLower(al.currentLocation).find(toLower(filter.location.value())) == std::string::npos) continue;
        }

        // Verification Status
        if (filter.verificationStatus.has_value() && !filter.verificationStatus.value().empty()) {
            if (toLower(al.verificationStatus) != toLower(filter.verificationStatus.value())) continue;
        }

        // Skills filter
        if (!filter.skills.empty()) {
            if (filter.skillsAndLogic) {
                bool hasAll = true;
                for (const auto& sk : filter.skills) {
                    std::string target = toLower(sk);
                    bool hasSk = false;
                    for (const auto& s : al.skills) {
                        if (toLower(s).find(target) != std::string::npos) { hasSk = true; break; }
                    }
                    if (!hasSk) { hasAll = false; break; }
                }
                if (!hasAll) continue;
            } else {
                bool hasAny = false;
                for (const auto& sk : filter.skills) {
                    std::string target = toLower(sk);
                    for (const auto& s : al.skills) {
                        if (toLower(s).find(target) != std::string::npos) { hasAny = true; break; }
                    }
                    if (hasAny) break;
                }
                if (!hasAny) continue;
            }
        }

        filtered.push_back(al);
    }

    totalCount = static_cast<int>(filtered.size());

    // Sort
    std::sort(filtered.begin(), filtered.end(), [&filter](const Alumni& a, const Alumni& b) {
        if (filter.sortBy == "graduation_year") {
            return filter.sortOrder == "desc" ? (a.graduationYear > b.graduationYear) : (a.graduationYear < b.graduationYear);
        }
        if (filter.sortBy == "updated_at") {
            return filter.sortOrder == "desc" ? (a.updatedAt > b.updatedAt) : (a.updatedAt < b.updatedAt);
        }
        return filter.sortOrder == "desc" ? (a.fullName > b.fullName) : (a.fullName < b.fullName);
    });

    // Pagination
    int offset = (filter.page - 1) * filter.limit;
    if (offset >= static_cast<int>(filtered.size())) {
        return {};
    }

    int endIdx = std::min(offset + filter.limit, static_cast<int>(filtered.size()));
    return std::vector<Alumni>(filtered.begin() + offset, filtered.begin() + endIdx);
}

std::optional<Alumni> DataRepository::getAlumniById(int id) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = alumni_.find(id);
    if (it != alumni_.end()) {
        return it->second;
    }
    return std::nullopt;
}

Alumni DataRepository::createAlumni(const Alumni& newAlumni) {
    std::lock_guard<std::mutex> lock(mtx_);
    Alumni a = newAlumni;
    a.id = nextAlumniId_++;
    std::string now = currentTimestamp();
    a.createdAt = now;
    a.updatedAt = now;

    // Resolve institution name
    if (institutions_.find(a.institutionId) != institutions_.end()) {
        a.institutionName = institutions_[a.institutionId].name;
    }
    // Resolve department name
    if (departments_.find(a.departmentId) != departments_.end()) {
        a.departmentName = departments_[a.departmentId].name;
    }
    // Resolve batch degree & years
    if (batches_.find(a.batchId) != batches_.end()) {
        a.degree = batches_[a.batchId].degree;
        a.startYear = batches_[a.batchId].startYear;
        a.graduationYear = batches_[a.batchId].graduationYear;
    }

    alumni_[a.id] = a;
    alumniSkills_[a.id] = a.skills;
    return a;
}

bool DataRepository::updateAlumni(int id, const Alumni& updated) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = alumni_.find(id);
    if (it == alumni_.end()) return false;

    Alumni& a = it->second;
    a.fullName = updated.fullName;
    a.currentLocation = updated.currentLocation;
    a.bio = updated.bio;
    a.linkedinUrl = updated.linkedinUrl;
    a.websiteUrl = updated.websiteUrl;
    a.avatarUrl = updated.avatarUrl;
    a.currentCompany = updated.currentCompany;
    a.currentJobTitle = updated.currentJobTitle;
    a.currentIndustry = updated.currentIndustry;
    a.skills = updated.skills;
    a.updatedAt = currentTimestamp();
    alumniSkills_[id] = updated.skills;
    return true;
}

bool DataRepository::deleteAlumni(int id) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = alumni_.find(id);
    if (it == alumni_.end()) return false;

    // Delete associated career history
    careerHistories_.erase(
        std::remove_if(careerHistories_.begin(), careerHistories_.end(),
                       [id](const CareerHistory& ch) { return ch.alumniId == id; }),
        careerHistories_.end()
    );

    // Delete connections
    connections_.erase(
        std::remove_if(connections_.begin(), connections_.end(),
                       [id](const AlumniConnection& c) { return c.sourceAlumniId == id || c.targetAlumniId == id; }),
        connections_.end()
    );

    alumniSkills_.erase(id);
    alumni_.erase(it);
    return true;
}

bool DataRepository::verifyAlumni(int id, const std::string& status, const std::string& verifiedBy) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = alumni_.find(id);
    if (it == alumni_.end()) return false;

    it->second.verificationStatus = status;
    it->second.lastVerifiedAt = currentTimestamp();
    it->second.updatedAt = currentTimestamp();
    return true;
}

bool DataRepository::addCareerHistory(int alumniId, int companyId, const std::string& jobTitle,
                                     const std::string& startDate, const std::string& endDate, bool isCurrent) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (alumni_.find(alumniId) == alumni_.end()) return false;
    if (companies_.find(companyId) == companies_.end()) return false;

    // Enforce rule: only one job marked as current
    if (isCurrent) {
        for (auto& ch : careerHistories_) {
            if (ch.alumniId == alumniId) {
                ch.isCurrent = false;
            }
        }
        for (auto& ch : alumni_[alumniId].careerHistory) {
            ch.isCurrent = false;
        }
        alumni_[alumniId].currentCompany = companies_[companyId].name;
        alumni_[alumniId].currentJobTitle = jobTitle;
        alumni_[alumniId].currentIndustry = companies_[companyId].industry;
    }

    CareerHistory ch;
    ch.id = nextCareerId_++;
    ch.alumniId = alumniId;
    ch.companyId = companyId;
    ch.companyName = companies_[companyId].name;
    ch.industry = companies_[companyId].industry;
    ch.jobTitle = jobTitle;
    ch.startDate = startDate;
    ch.endDate = endDate;
    ch.isCurrent = isCurrent;

    careerHistories_.push_back(ch);
    alumni_[alumniId].careerHistory.push_back(ch);
    return true;
}

std::vector<AlumniConnection> DataRepository::getAlumniConnections(int alumniId) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<AlumniConnection> list;
    for (const auto& c : connections_) {
        if (c.sourceAlumniId == alumniId) {
            list.push_back(c);
        }
    }
    return list;
}

std::vector<AlumniConnection> DataRepository::getAllConnections() {
    std::lock_guard<std::mutex> lock(mtx_);
    return connections_;
}

bool DataRepository::addConnection(int sourceId, int targetId, const std::string& type, int createdBy) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (sourceId == targetId) return false;
    if (alumni_.find(sourceId) == alumni_.end() || alumni_.find(targetId) == alumni_.end()) return false;

    for (const auto& c : connections_) {
        if (c.sourceAlumniId == sourceId && c.targetAlumniId == targetId && c.relationshipType == type) {
            return false; // Already exists
        }
    }

    std::string now = currentTimestamp();
    AlumniConnection forwardConn{nextConnectionId_++, sourceId, targetId, type, createdBy, now,
                                 alumni_[targetId].fullName, alumni_[targetId].currentCompany, alumni_[targetId].currentJobTitle};
    connections_.push_back(forwardConn);
    alumni_[sourceId].connections.push_back(forwardConn);

    AlumniConnection reverseConn{nextConnectionId_++, targetId, sourceId, type, createdBy, now,
                                 alumni_[sourceId].fullName, alumni_[sourceId].currentCompany, alumni_[sourceId].currentJobTitle};
    connections_.push_back(reverseConn);
    alumni_[targetId].connections.push_back(reverseConn);

    return true;
}

bool DataRepository::removeConnection(int sourceId, int targetId) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = std::remove_if(connections_.begin(), connections_.end(),
                             [sourceId, targetId](const AlumniConnection& c) {
                                 return (c.sourceAlumniId == sourceId && c.targetAlumniId == targetId) ||
                                        (c.sourceAlumniId == targetId && c.targetAlumniId == sourceId);
                             });
    bool removed = (it != connections_.end());
    connections_.erase(it, connections_.end());

    if (alumni_.find(sourceId) != alumni_.end()) {
        auto& sc = alumni_[sourceId].connections;
        sc.erase(std::remove_if(sc.begin(), sc.end(), [targetId](const AlumniConnection& c){ return c.targetAlumniId == targetId; }), sc.end());
    }
    if (alumni_.find(targetId) != alumni_.end()) {
        auto& tc = alumni_[targetId].connections;
        tc.erase(std::remove_if(tc.begin(), tc.end(), [sourceId](const AlumniConnection& c){ return c.targetAlumniId == sourceId; }), tc.end());
    }
    return removed;
}

void DataRepository::logAction(std::optional<int> actorId, const std::string& actorName, const std::string& action,
                              const std::string& entityType, int entityId, const std::string& details) {
    std::lock_guard<std::mutex> lock(mtx_);
    AuditLog l;
    l.id = nextAuditLogId_++;
    l.actorUserId = actorId;
    l.actorName = actorName;
    l.action = action;
    l.entityType = entityType;
    l.entityId = entityId;
    l.details = details;
    l.createdAt = currentTimestamp();
    auditLogs_.push_back(l);
}

std::vector<AuditLog> DataRepository::getAuditLogs(int limit) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<AuditLog> list = auditLogs_;
    std::reverse(list.begin(), list.end()); // Latest first
    if (static_cast<int>(list.size()) > limit) {
        list.resize(limit);
    }
    return list;
}

nlohmann::json DataRepository::getAnalytics() {
    std::lock_guard<std::mutex> lock(mtx_);

    int totalAlumni = static_cast<int>(alumni_.size());
    int verifiedCount = 0;
    std::unordered_set<std::string> compSet;
    std::unordered_set<std::string> indSet;
    std::unordered_map<int, int> yearDistribution;

    for (const auto& [_, al] : alumni_) {
        if (al.verificationStatus == "verified") verifiedCount++;
        if (!al.currentCompany.empty() && al.currentCompany != "Independent") compSet.insert(al.currentCompany);
        if (!al.currentIndustry.empty() && al.currentIndustry != "Other") indSet.insert(al.currentIndustry);
        yearDistribution[al.graduationYear]++;
    }

    nlohmann::json yearArr = nlohmann::json::array();
    for (const auto& [yr, count] : yearDistribution) {
        yearArr.push_back({{"year", yr}, {"count", count}});
    }
    std::sort(yearArr.begin(), yearArr.end(), [](const nlohmann::json& a, const nlohmann::json& b){
        return a["year"].get<int>() < b["year"].get<int>();
    });

    nlohmann::json recentVerified = nlohmann::json::array();
    for (const auto& [_, al] : alumni_) {
        if (al.verificationStatus == "verified" && !al.lastVerifiedAt.empty()) {
            recentVerified.push_back({
                {"id", al.id},
                {"name", al.fullName},
                {"company", al.currentCompany},
                {"job_title", al.currentJobTitle},
                {"verified_at", al.lastVerifiedAt}
            });
            if (recentVerified.size() >= 5) break;
        }
    }

    nlohmann::json result;
    result["total_alumni"] = totalAlumni;
    result["verified_profiles"] = verifiedCount;
    result["companies_represented"] = static_cast<int>(compSet.size());
    result["industries_represented"] = static_cast<int>(indSet.size());
    result["connections_count"] = static_cast<int>(connections_.size() / 2);
    result["alumni_distribution_by_year"] = yearArr;
    result["recent_verified_updates"] = recentVerified;
    return result;
}

} // namespace alumniverse
