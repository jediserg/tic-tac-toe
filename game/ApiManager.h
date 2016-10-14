//
// Created by serg on 10/13/16.
//

#ifndef TIC_TAC_TOE_APIMANAGER_H
#define TIC_TAC_TOE_APIMANAGER_H

#include <json.hpp>

#include "Api.h"

class ApiManager {
public:
    using ApiMap = std::map<std::string, Api>;

    const Api& api(const std::string& name);
    bool hasApi(const std::string name);

    bool callApi(nlohmann::json&& request, Api::Callback callback);

    static constexpr const char* API_FIELD = "api";
private:
    ApiMap _api_map;
};

#endif //TIC_TAC_TOE_APIMANAGER_H
