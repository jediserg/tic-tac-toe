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

    Api() = delete;
    Api(std::string name);

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
