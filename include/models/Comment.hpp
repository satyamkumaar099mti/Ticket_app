#pragma once
#include <string>

namespace Models {

class Comment {
private:
    std::string id;
    std::string ticketId;
    std::string authorUsername;
    std::string content;
    std::string timestamp;
    bool isInternal; // Visible only to Agent/Admin

public:
    Comment() : isInternal(false) {}

    Comment(std::string id, std::string ticketId, std::string authorUsername,
            std::string content, std::string timestamp, bool isInternal = false)
        : id(std::move(id)),
          ticketId(std::move(ticketId)),
          authorUsername(std::move(authorUsername)),
          content(std::move(content)),
          timestamp(std::move(timestamp)),
          isInternal(isInternal) {}

    const std::string& getId() const { return id; }
    const std::string& getTicketId() const { return ticketId; }
    const std::string& getAuthorUsername() const { return authorUsername; }
    const std::string& getContent() const { return content; }
    const std::string& getTimestamp() const { return timestamp; }
    bool getIsInternal() const { return isInternal; }
};

} // namespace Models

