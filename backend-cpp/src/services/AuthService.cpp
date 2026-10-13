#include "../../include/services/AuthService.hpp"
#include <cstdlib>

namespace alumniverse {

AuthService::AuthService(std::shared_ptr<DataRepository> repo, std::string secretKey)
    : repo_(std::move(repo)), secretKey_(std::move(secretKey)) {
    const char* envSecret = std::getenv("JWT_SECRET");
    if (envSecret && std::string(envSecret).length() > 0) {
        secretKey_ = envSecret;
    }
}

AuthResult AuthService::registerUser(const std::string& name, const std::string& email, const std::string& password, const std::string& roleStr) {
    if (name.empty() || email.empty() || password.empty()) {
        return {false, "", std::nullopt, "Name, email, and password are required."};
    }
    if (password.length() < 6) {
        return {false, "", std::nullopt, "Password must be at least 6 characters long."};
    }

    if (repo_->findUserByEmail(email).has_value()) {
        return {false, "", std::nullopt, "A user with this email address already exists."};
    }

    UserRole role = stringToRole(roleStr);
    std::string passHash = CryptoUtils::hashPassword(password);
    User newUser = repo_->createUser(name, email, passHash, role);

    repo_->logAction(newUser.id, newUser.name, "USER_REGISTER", "users", newUser.id, "User registered with role: " + roleStr);

    std::string token = CryptoUtils::generateJwt(newUser.id, newUser.email, roleToString(newUser.role), secretKey_);
    return {true, token, newUser, ""};
}

AuthResult AuthService::loginUser(const std::string& email, const std::string& password) {
    if (email.empty() || password.empty()) {
        return {false, "", std::nullopt, "Email and password are required."};
    }

    auto userOpt = repo_->findUserByEmail(email);
    if (!userOpt.has_value()) {
        return {false, "", std::nullopt, "Invalid email or password credentials."};
    }

    const auto& user = userOpt.value();
    if (!CryptoUtils::verifyPassword(password, user.passwordHash)) {
        return {false, "", std::nullopt, "Invalid email or password credentials."};
    }

    repo_->logAction(user.id, user.name, "USER_LOGIN", "users", user.id, "User successfully logged in.");

    std::string token = CryptoUtils::generateJwt(user.id, user.email, roleToString(user.role), secretKey_);
    return {true, token, user, ""};
}

std::optional<User> AuthService::validateToken(const std::string& tokenHeader) {
    if (tokenHeader.empty()) return std::nullopt;

    std::string token = tokenHeader;
    if (token.rfind("Bearer ", 0) == 0) {
        token = token.substr(7);
    }

    auto payload = CryptoUtils::verifyJwt(token, secretKey_);
    if (!payload.has_value()) return std::nullopt;

    int userId = payload.value().value("userId", 0);
    return repo_->findUserById(userId);
}

} // namespace alumniverse
