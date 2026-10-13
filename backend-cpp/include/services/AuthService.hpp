#pragma once

#include <memory>
#include <string>
#include "../repositories/DataRepository.hpp"
#include "../utils/CryptoUtils.hpp"

namespace alumniverse {

struct AuthResult {
    bool success{false};
    std::string token;
    std::optional<User> user;
    std::string errorMessage;
};

class AuthService {
public:
    AuthService(std::shared_ptr<DataRepository> repo, std::string secretKey = "alumniverse_jwt_secret_dev_key_2026");

    AuthResult registerUser(const std::string& name, const std::string& email, const std::string& password, const std::string& roleStr = "alumni");
    AuthResult loginUser(const std::string& email, const std::string& password);
    std::optional<User> validateToken(const std::string& tokenHeader);

private:
    std::shared_ptr<DataRepository> repo_;
    std::string secretKey_;
};

} // namespace alumniverse
