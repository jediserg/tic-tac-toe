//
// Created by serg on 10/12/16.
//
#include <json.hpp>
#include "Api.h"


Api::Api(std::string name)
        : _is_enabled(true),
          _name(std::move(name)) {
}

bool Api::isEnabled() const {
    return _is_enabled;
}

void Api::enable() {
    _is_enabled = true;
}

void Api::disable() {
    _is_enabled = false;
}

bool Api::call(std::shared_ptr<User> user, nlohmann::json &&request) const {
    if (!_is_enabled)
        return false;

    auto command = request.find(COMMAND_FIELD);

    if (command == request.end())
        return false;

    auto handler = _handlers.find(*command);

    if (handler == _handlers.end())
        return false;

    return handler->second(user, std::move(request));
}

void Api::setHandler(const std::string &command, Api::Handler handler) {
    _handlers[command] = handler;
}

const std::string &Api::name() const {
    return _name;
}
