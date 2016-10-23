//
// Created by serg on 10/16/16.
//

#ifndef TIC_TAC_TOE_USER_H
#define TIC_TAC_TOE_USER_H

#include <memory>
#include <map>
#include "Validatable.h"

class User : public Validatable
{
public:
    User(std::map<std::string, std::string> data);
public:
    const std::string &getName() const;
private:
    std::string _name;
    std::string _password;
    int _win_count;
public:
    int getWinCount() const;

public:
    const std::string &getPassword() const;
};


#endif //TIC_TAC_TOE_USER_H
