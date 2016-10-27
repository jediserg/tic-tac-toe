//
// Created by serg on 10/16/16.
//

#include "User.h"
#include <iostream>

User::User(std::map<std::string, std::string> data) {
    try {
        _name = data["name"];
        if (data.find("win_count") != data.end())
            _win_count = std::stoi(data["win_count"]);

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
