//
// Created by serg on 10/12/16.
//

#ifndef TIC_TAC_TOE_API_H
#define TIC_TAC_TOE_API_H

#include <string>
#include <map>
#include <functional>
#include <json.hpp>

class Api
{
public:
    using Callback = std::function<void (nlohmann::json&&)>;
    using Handler = std::function<bool (nlohmann::json&&, Callback)>;
    using Handlers = std::map<std::string, Handler>;

    Api(std::string name);

    //Api(const Api&) = delete;
    //Api(Api&&) = delete;
    Api() = delete;

    Api &operator=(const Api &) = delete;

    bool isEnabled() const;
    void enable();
    void disable();

    bool call(nlohmann::json&& request, Callback callback) const;

    const std::string& name() const;

    void setHandler(const std::string& command, Handler handler);

    static constexpr const char* COMMAND_FIELD = "command";


private:
    bool _is_enabled;
    Handlers _handlers;
    std::string _name;
};


#endif //TIC_TAC_TOE_API_H
