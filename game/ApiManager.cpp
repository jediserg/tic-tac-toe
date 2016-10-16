//
// Created by serg on 10/13/16.
//

#include "ApiManager.h"
#include <json.hpp>

Api &ApiManager::api(const std::string &name)
{
    ApiMap::iterator it = _api_map.find(name);

    if(it == _api_map.end())
    {
        std::cout << "create new api" << std::endl;
        auto result = _api_map.emplace(name, name);

        return result.first->second;
    }
    std::cout << "existing api" << std::endl;
    return it->second;
}

bool ApiManager::hasApi(const std::string name) {
    return _api_map.find(name) != _api_map.end();
}

bool ApiManager::callApi(nlohmann::json&& request, Api::Callback callback) {
    auto api_name = request.find(API_FIELD);

    if(api_name == request.end())
        return false;

    ApiMap::const_iterator api_it = _api_map.find(*api_name);
    if(api_it == _api_map.end())
        return false;

    return api_it->second.call(std::move(request), callback);
}
