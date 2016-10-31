/*!
 * \brief A set of different version of API we should support
 *
 * Some times we have to support different version of APIs (old versions, different clients, etc) so this class stores
 * different versions of APIs. ApiManager has a method to add a command to all supported APIs.
 * \todo add support to link on old APIs method from the new one and export a subset of an API to another API.
 */

#ifndef TIC_TAC_TOE_APIMANAGER_H
#define TIC_TAC_TOE_APIMANAGER_H

#include <initializer_list>
#include <json.hpp>

#include "Api.h"

class User;

class ApiManager {
public:
    using ApiMap = std::map<std::string, Api>;
    static constexpr const char *API_FIELD = "api";

    void setSupportedApi(std::string api_name);

    void setSupportedApi(std::initializer_list<std::string> api_names);

    Api &api(const std::string &name);

    bool supportApi(std::string name) noexcept;

    bool callApi(std::shared_ptr<User> user, nlohmann::json &&request, Api::Callback callback) const noexcept;

    void setHandler(std::string api_name, std::string command, Api::Handler handler);

    void setHandlerForAllApi(std::string command, Api::Handler handler);

    static ApiManager &get();

    static const ApiManager &get_const();

private:
    ApiMap _api_map;
};

#endif //TIC_TAC_TOE_APIMANAGER_H
