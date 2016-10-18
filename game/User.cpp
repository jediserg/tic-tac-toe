//
// Created by serg on 10/16/16.
//

#include "User.h"

User::User(std::string name, bool is_bot)
        : _name(name), _is_bot(is_bot) {

}

const std::string &User::getName() const {
    return _name;
}

bool User::isBot() const {
    return _is_bot;
}
