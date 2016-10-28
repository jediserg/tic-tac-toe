//
// Created by serg on 10/16/16.
//

#include "User.h"
#include <iostream>

User::User(std::map<std::string, std::string> data) : _win_count(0) {
    try {
        _name = data["name"];
        _password = data["password"];
        if (data.find("win_count") != data.end()) {
            auto temp = data["win_count"];
            _win_count = std::stoi(temp);
        }

        setValid(true);
    } catch (std::exception &e) {
        std::cout << "Bad arguments for User:" << e.what() << std::endl;
        setValid(false);
    }
}

const std::string &User::getName() const {
    return _name;
}

const std::string &User::getPassword() const {
    return _password;
}

int User::getWinCount() const {
    return _win_count;
}
