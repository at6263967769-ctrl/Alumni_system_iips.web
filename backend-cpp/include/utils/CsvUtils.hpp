#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace alumniverse {

struct CsvAlumniRow {
    std::string fullName;
    int graduationYear{0};
    std::string degree;
    std::string department;
    std::string company;
    std::string jobTitle;
    std::string industry;
    std::string location;
    std::string linkedinUrl;
    std::vector<std::string> skills;
    bool isValid{true};
    std::string errorMessage;
};

class CsvUtils {
public:
    /**
     * @brief Sanitizes a field against formula injection (DDE/CSV injection)
     */
    static std::string sanitizeCell(std::string cell) {
        // Trim leading and trailing spaces
        while (!cell.empty() && std::isspace(static_cast<unsigned char>(cell.front()))) cell.erase(cell.begin());
        while (!cell.empty() && std::isspace(static_cast<unsigned char>(cell.back()))) cell.pop_back();

        // If cell starts with formula trigger characters (=, +, -, @), prefix with a safe single quote
        if (!cell.empty() && (cell[0] == '=' || cell[0] == '+' || cell[0] == '-' || cell[0] == '@')) {
            cell = "'" + cell;
        }
        return cell;
    }

    /**
     * @brief Parses a single CSV line into tokens, respecting quotes
     */
    static std::vector<std::string> parseCsvLine(const std::string& line) {
        std::vector<std::string> tokens;
        std::string current;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '"') {
                if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                    current.push_back('"');
                    i++; // skip escaped quote
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (c == ',' && !inQuotes) {
                tokens.push_back(sanitizeCell(current));
                current.clear();
            } else {
                current.push_back(c);
            }
        }
        tokens.push_back(sanitizeCell(current));
        return tokens;
    }

    /**
     * @brief Parses entire CSV text and extracts alumni rows with validation.
     */
    static std::vector<CsvAlumniRow> parseAlumniCsv(const std::string& csvContent) {
        std::vector<CsvAlumniRow> results;
        std::istringstream stream(csvContent);
        std::string line;

        bool isHeader = true;
        int lineNum = 0;

        while (std::getline(stream, line)) {
            lineNum++;
            if (line.empty()) continue;
            // Trim carriage return
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) continue;

            auto tokens = parseCsvLine(line);
            if (isHeader) {
                isHeader = false;
                continue; // Skip header line
            }

            CsvAlumniRow row;
            // Expected columns: full_name, graduation_year, degree, department, company, job_title, industry, location, linkedin_url, skills
            if (tokens.size() < 6) {
                row.isValid = false;
                row.errorMessage = "Line " + std::to_string(lineNum) + ": Insufficient columns (expected at least 6).";
                results.push_back(row);
                continue;
            }

            row.fullName = tokens[0];
            try {
                row.graduationYear = std::stoi(tokens[1]);
            } catch (...) {
                row.graduationYear = 0;
            }

            row.degree = (tokens.size() > 2) ? tokens[2] : "";
            row.department = (tokens.size() > 3) ? tokens[3] : "";
            row.company = (tokens.size() > 4) ? tokens[4] : "";
            row.jobTitle = (tokens.size() > 5) ? tokens[5] : "";
            row.industry = (tokens.size() > 6) ? tokens[6] : "Technology";
            row.location = (tokens.size() > 7) ? tokens[7] : "";
            row.linkedinUrl = (tokens.size() > 8) ? tokens[8] : "";

            if (tokens.size() > 9 && !tokens[9].empty()) {
                std::istringstream skillStream(tokens[9]);
                std::string sk;
                while (std::getline(skillStream, sk, ',')) {
                    while (!sk.empty() && std::isspace(static_cast<unsigned char>(sk.front()))) sk.erase(sk.begin());
                    while (!sk.empty() && std::isspace(static_cast<unsigned char>(sk.back()))) sk.pop_back();
                    if (!sk.empty()) row.skills.push_back(sk);
                }
            }

            if (row.fullName.empty()) {
                row.isValid = false;
                row.errorMessage = "Line " + std::to_string(lineNum) + ": Name cannot be empty.";
            } else if (row.graduationYear < 1950 || row.graduationYear > 2030) {
                row.isValid = false;
                row.errorMessage = "Line " + std::to_string(lineNum) + ": Invalid graduation year (" + std::to_string(row.graduationYear) + ").";
            }

            results.push_back(row);
        }

        return results;
    }
};

} // namespace alumniverse
