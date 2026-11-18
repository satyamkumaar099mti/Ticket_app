#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

namespace UI {

namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string WHITE   = "\033[37m";
}

inline void printBanner(const std::string& title) {
    std::cout << "\n" << Color::CYAN << Color::BOLD
              << "================================================================================"
              << Color::RESET << "\n";
    std::cout << Color::BOLD << "   " << title << Color::RESET << "\n";
    std::cout << Color::CYAN << Color::BOLD
              << "================================================================================"
              << Color::RESET << "\n";
}

inline void printSubBanner(const std::string& title) {
    std::cout << "\n" << Color::BLUE << "--- [ " << title << " ] "
              << std::string(std::max(0, 70 - (int)title.length()), '-')
              << Color::RESET << "\n";
}

inline void printSuccess(const std::string& msg) {
    std::cout << Color::GREEN << "[SUCCESS] " << msg << Color::RESET << "\n";
}

inline void printError(const std::string& msg) {
    std::cout << Color::RED << "[ERROR] " << msg << Color::RESET << "\n";
}

inline void printInfo(const std::string& msg) {
    std::cout << Color::YELLOW << "[INFO] " << msg << Color::RESET << "\n";
}

} // namespace UI

