#pragma once
#include <exception>
#include <string>

class GameException : public std::exception {
private:
    std::string message;
    
public:
    GameException(const std::string& msg) : message(msg) {}
    
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};