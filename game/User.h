/*!
 * Simple model class to work with Store
 */

#ifndef TIC_TAC_TOE_USER_H
#define TIC_TAC_TOE_USER_H

#include <memory>
#include <map>
#include "Validatable.h"

class User : public Validatable {
public:
    User(std::map<std::string, std::string> data);

    const std::string &getName() const;

    const std::string &getPassword() const;

    int getWinCount() const;

private:
    std::string _name;
    std::string _password;
    int _win_count;
};


#endif //TIC_TAC_TOE_USER_H
