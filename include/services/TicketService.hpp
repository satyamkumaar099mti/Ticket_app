#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "../repositories/TicketRepository.hpp"
#include "../repositories/CommentRepository.hpp"
#include "../repositories/UserRepository.hpp"
#include "../models/Ticket.hpp"
#include "../models/Comment.hpp"
#include "../models/User.hpp"
#include "../utils/Utils.hpp"

namespace Services {

struct TicketStatistics {
    int totalTickets = 0;
    int openTickets = 0;
    int inProgressTickets = 0;
    int resolvedTickets = 0;
    int closedTickets = 0;
    int criticalTickets = 0;
    std::map<std::string, int> categoryCounts;
    std::map<std::string, int> agentWorkload;
};

class TicketService {
private:
    Repositories::TicketRepository& ticketRepo;
    Repositories::CommentRepository& commentRepo;
    Repositories::UserRepository& userRepo;

public:
    TicketService(Repositories::TicketRepository& tRepo,
                 Repositories::CommentRepository& cRepo,
                 Repositories::UserRepository& uRepo)
        : ticketRepo(tRepo), commentRepo(cRepo), userRepo(uRepo) {}

    std::string createTicket(const std::string& title, const std::string& desc,
                             const std::string& category, Models::Priority prio,
                             const std::string& creatorUsername) {
        std::string id = ticketRepo.generateNextId();
        std::string now = Utils::getCurrentTimestamp();
        Models::Ticket ticket(id, title, desc, category, prio, Models::Status::OPEN,
                              creatorUsername, "Unassigned", now, now);
        ticketRepo.addTicket(ticket);
        return id;
    }

    Models::Ticket* getTicket(const std::string& id) {
        return ticketRepo.findById(id);
    }

    std::vector<Models::Ticket> getAllTickets() {
        return ticketRepo.getAll();
    }

    std::vector<Models::Ticket> getTicketsForUser(const Models::User& user) {
        std::vector<Models::Ticket> all = ticketRepo.getAll();
        std::vector<Models::Ticket> filtered;

        if (user.getRole() == Models::Role::ADMIN) {
            return all;
        } else if (user.getRole() == Models::Role::AGENT) {
            for (const auto& t : all) {
                if (t.getAssigneeUsername() == user.getUsername() || t.getAssigneeUsername() == "Unassigned") {
                    filtered.push_back(t);
                }
            }
        } else { // Customer
            for (const auto& t : all) {
                if (t.getCreatorUsername() == user.getUsername()) {
                    filtered.push_back(t);
                }
            }
        }
        return filtered;
    }

    bool assignTicket(const std::string& ticketId, const std::string& agentUsername) {
        Models::Ticket* t = ticketRepo.findById(ticketId);
        if (!t) return false;
        Models::User* agent = userRepo.findByUsername(agentUsername);
        if (!agent || (agent->getRole() != Models::Role::AGENT && agent->getRole() != Models::Role::ADMIN)) {
            return false;
        }

        t->setAssigneeUsername(agentUsername);
        if (t->getStatus() == Models::Status::OPEN) {
            t->setStatus(Models::Status::IN_PROGRESS);
        }
        t->setUpdatedAt(Utils::getCurrentTimestamp());
        return ticketRepo.updateTicket(*t);
    }

    bool updateStatus(const std::string& ticketId, Models::Status newStatus) {
        Models::Ticket* t = ticketRepo.findById(ticketId);
        if (!t) return false;
        t->setStatus(newStatus);
        t->setUpdatedAt(Utils::getCurrentTimestamp());
        return ticketRepo.updateTicket(*t);
    }

    bool updatePriority(const std::string& ticketId, Models::Priority newPriority) {
        Models::Ticket* t = ticketRepo.findById(ticketId);
        if (!t) return false;
        t->setPriority(newPriority);
        t->setUpdatedAt(Utils::getCurrentTimestamp());
        return ticketRepo.updateTicket(*t);
    }

    void addComment(const std::string& ticketId, const std::string& author,
                    const std::string& content, bool isInternal) {
        std::string commentId = commentRepo.generateNextId();
        std::string now = Utils::getCurrentTimestamp();
        Models::Comment comment(commentId, ticketId, author, content, now, isInternal);
        commentRepo.addComment(comment);

        // Also update ticket's updatedAt
        Models::Ticket* t = ticketRepo.findById(ticketId);
        if (t) {
            t->setUpdatedAt(now);
            ticketRepo.updateTicket(*t);
        }
    }

    std::vector<Models::Comment> getTicketComments(const std::string& ticketId, bool includeInternal) {
        return commentRepo.getByTicketId(ticketId, includeInternal);
    }

    std::vector<Models::Ticket> searchAndFilter(const std::string& keyword,
                                                const std::string& categoryFilter,
                                                const std::string& statusFilter,
                                                const std::string& priorityFilter,
                                                const Models::User& user) {
        auto base = getTicketsForUser(user);
        std::vector<Models::Ticket> res;
        std::string kw = Utils::toLower(keyword);

        for (const auto& t : base) {
            if (!keyword.empty()) {
                std::string titleLower = Utils::toLower(t.getTitle());
                std::string descLower = Utils::toLower(t.getDescription());
                std::string idLower = Utils::toLower(t.getId());
                if (titleLower.find(kw) == std::string::npos &&
                    descLower.find(kw) == std::string::npos &&
                    idLower.find(kw) == std::string::npos) {
                    continue;
                }
            }
            if (!categoryFilter.empty() && categoryFilter != "ALL" && t.getCategory() != categoryFilter) {
                continue;
            }
            if (!statusFilter.empty() && statusFilter != "ALL" && Models::statusToString(t.getStatus()) != statusFilter) {
                continue;
            }
            if (!priorityFilter.empty() && priorityFilter != "ALL" && Models::priorityToString(t.getPriority()) != priorityFilter) {
                continue;
            }
            res.push_back(t);
        }
        return res;
    }

    TicketStatistics getStatistics() {
        TicketStatistics stats;
        auto all = ticketRepo.getAll();
        stats.totalTickets = static_cast<int>(all.size());

        for (const auto& t : all) {
            switch (t.getStatus()) {
                case Models::Status::OPEN: stats.openTickets++; break;
                case Models::Status::IN_PROGRESS: stats.inProgressTickets++; break;
                case Models::Status::RESOLVED: stats.resolvedTickets++; break;
                case Models::Status::CLOSED: stats.closedTickets++; break;
            }
            if (t.getPriority() == Models::Priority::CRITICAL) {
                stats.criticalTickets++;
            }
            stats.categoryCounts[t.getCategory()]++;
            if (t.getAssigneeUsername() != "Unassigned") {
                stats.agentWorkload[t.getAssigneeUsername()]++;
            }
        }
        return stats;
    }
};

} // namespace Services

