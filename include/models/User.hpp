#pragma once
#include <string>

namespace Models {

enum class Role {
    CUSTOMER = 1,
    AGENT = 2,
    ADMIN = 3
};

inline std::string roleToString(Role role) {
    switch (role) {
        case Role::CUSTOMER: return "Customer";
        case Role::AGENT:    return "Support Agent";
        case Role::ADMIN:    return "Administrator";
        default:             return "Unknown";
    }
}

inline Role roleFromString(const std::string& str) {
    if (str == "Customer" || str == "CUSTOMER" || str == "1") return Role::CUSTOMER;
    if (str == "Support Agent" || str == "AGENT" || str == "2") return Role::AGENT;
    if (str == "Administrator" || str == "ADMIN" || str == "3") return Role::ADMIN;
    return Role::CUSTOMER;
}

class User {
private:
    std::string id;
    std::string username;
    std::string passwordHash;
    std::string fullName;
    std::string email;
    Role role;
    bool active;

public:
    User() : role(Role::CUSTOMER), active(true) {}

    User(std::string id, std::string username, std::string passwordHash,
         std::string fullName, std::string email, Role role, bool active = true)
        : id(std::move(id)),
          username(std::move(username)),
          passwordHash(std::move(passwordHash)),
          fullName(std::move(fullName)),
          email(std::move(email)),
          role(role),
          active(active) {}

    const std::string& getId() const { return id; }
    const std::string& getUsername() const { return username; }
    const std::string& getPasswordHash() const { return passwordHash; }
    const std::string& getFullName() const { return fullName; }
    const std::string& getEmail() const { return email; }
    Role getRole() const { return role; }
    bool isActive() const { return active; }

    void setPasswordHash(const std::string& newHash) { passwordHash = newHash; }
    void setFullName(const std::string& name) { fullName = name; }
    void setEmail(const std::string& mail) { email = mail; }
    void setRole(Role r) { role = r; }
    void setActive(bool act) { active = act; }
};

} // namespace Models

