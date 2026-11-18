#pragma once
#include <string>
#include <memory>
#include "../repositories/UserRepository.hpp"
#include "../models/User.hpp"
#include "../utils/Utils.hpp"

namespace Services {

class AuthService {
private:
    Repositories::UserRepository& userRepo;
    Models::User* currentUser;

public:
    explicit AuthService(Repositories::UserRepository& repo)
        : userRepo(repo), currentUser(nullptr) {}

    bool login(const std::string& username, const std::string& password) {
        Models::User* user = userRepo.findByUsername(username);
        if (!user) return false;
        if (!user->isActive()) return false;

        std::string hashed = Utils::hashPassword(password);
        if (user->getPasswordHash() == hashed) {
            currentUser = user;
            return true;
        }
        return false;
    }

    void logout() {
        currentUser = nullptr;
    }

    bool registerCustomer(const std::string& username, const std::string& password,
                          const std::string& fullName, const std::string& email) {
        if (userRepo.findByUsername(username) != nullptr) {
            return false;
        }
        std::string newId = userRepo.generateNextId();
        std::string hash = Utils::hashPassword(password);
        Models::User newUser(newId, username, hash, fullName, email, Models::Role::CUSTOMER, true);
        return userRepo.addUser(newUser);
    }

    bool isAuthenticated() const {
        return currentUser != nullptr;
    }

    Models::User* getCurrentUser() const {
        return currentUser;
    }
};

} // namespace Services

