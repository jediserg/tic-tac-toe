/*!
 * \brief This Class represents an API, commands and handlers for them.
 *
 * Api has it's name (version) and a collection(map) of Handlers.
 * \code
 * using Handler = std::function<bool(std::shared_ptr<User>, nlohmann::json &&, Callback)>;
 * It is supossed that handler is no blocking, so all long tasks should be launched in separated thread and call
 * the Callback to send a response to the client.
 */

#ifndef TIC_TAC_TOE_API_H
#define TIC_TAC_TOE_API_H

#include <functional>
#include <json.hpp>
#include <map>
#include <memory>
#include <string>

class User;


class Api {
public:
    using Callback = std::function<void(nlohmann::json &&)>;
    using Handler = std::function<bool(std::shared_ptr<User>, nlohmann::json &&, Callback)>;
    using Handlers = std::map<std::string, Handler>;

    Api(std::string name);

    Api() = delete;

    Api &operator=(const Api &) = delete;

    bool isEnabled() const;

    void enable();

    void disable();

    bool call(std::shared_ptr<User> user, nlohmann::json &&request, Callback callback) const;

    const std::string &name() const;

    void setHandler(const std::string &command, Handler handler);

    static constexpr const char *COMMAND_FIELD = "command";
private:
    bool _is_enabled;
    //All handlers must be set up before server start, so after we have readonly access to the map, so it shoud be thread-safe
    Handlers _handlers;
    std::string _name;
};


#endif //TIC_TAC_TOE_API_H
