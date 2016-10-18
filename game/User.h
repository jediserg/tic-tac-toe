//
// Created by serg on 10/16/16.
//

#ifndef TIC_TAC_TOE_USER_H
#define TIC_TAC_TOE_USER_H

#include <memory>

class User
{
public:
    User() = default;

    User(std::string name, bool is_bot = false);

public:
    const std::string &getName() const;

    bool isBot() const;

private:
    std::string _name;
    bool _is_bot;
};


#endif //TIC_TAC_TOE_USER_H
