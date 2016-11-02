//
// Created by serg on 10/13/16.
//

#include "ApiManager.h"

Api &ApiManager::api(const std::string &name) {
    return _api_map.at(name);
}

bool ApiManager::isApiSupported(std::string name) noexcept {
    return _api_map.find(std::move(name)) != _api_map.end();
}

bool ApiManager::callApi(std::shared_ptr<User> user, nlohmann::json &&request, Api::Callback callback) const noexcept {
    auto api_name = request.find(API_FIELD);

    if (api_name == request.end())
        return false;

    ApiMap::const_iterator api_it = _api_map.find(*api_name);
    if (api_it == _api_map.end())
        return false;

    return api_it->second.call(user, std::move(request), callback);
}

ApiManager &ApiManager::get() {
    static ApiManager api_manager;
    return api_manager;
}

const ApiManager &ApiManager::get_const() {
    const ApiManager &const_api_manager = get();
    return const_api_manager;
}

void ApiManager::setSupportedApi(std::string api_name) {
    _api_map.clear();

    _api_map.emplace(api_name, api_name);
}

void ApiManager::setSupportedApi(std::initializer_list<std::string> api_names) {
    _api_map.clear();

    for (auto api: api_names)
        _api_map.emplace(api, api);
}

void ApiManager::setHandler(std::string api_name, std::string command, Api::Handler handler) {
    Api &api = _api_map.at(api_name);
    api.setHandler(command, handler);
}

void ApiManager::setHandlerForAllApi(std::string command, Api::Handler handler) {
    for (auto it: _api_map) {
        it.second.setHandler(command, handler);
    }
}
