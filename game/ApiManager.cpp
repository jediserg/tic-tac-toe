//
// Created by serg on 10/13/16.
//

#include "ApiManager.h"
#include <json.hpp>

const Api &ApiManager::api(const std::string& name) {
    ApiMap::const_iterator it = _api_map.find(name);

    if(it == _api_map.end())
    {
        auto result = _api_map.emplace(name, name);

        return result.first->second;
    }

    return it->second;
}

bool ApiManager::hasApi(const std::string name) {
    return _api_map.find(name) == _api_map.end();
}

bool ApiManager::callApi(const std::string &name, nlohmann::json &&response, Api::Callback callback) {
    return false;
}
