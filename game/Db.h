//
// Created by serg on 10/18/16.
//

#ifndef TIC_TAC_TOE_DB_H
#define TIC_TAC_TOE_DB_H

#include <string>
#include <memory>
#include <set>
#include <vector>
#include <thread>
#include <sstream>
#include <map>
#include <typeinfo>
#include <cppconn/connection.h>
#include <mutex>
#include <condition_variable>

namespace sql {
    class Connection;
};

struct ConnectionWithInfo {

    sql::Connection *connection;
    bool isInUse;
};

class Db {
public:
    using Getter = std::function<std::string(void *)>;

    Db(std::string host, std::string user, std::string password, size_t capacity = 4);

    template<class Model>
    void addSchema(std::string table, std::string id_field,
                   std::map<std::string, std::function<std::string(Model *)>> fields)
    {
        std::map<std::string, Getter> getters;
        std::vector<std::string> keys(fields.size());
        for(auto &field: fields)
        {
            keys.push_back(field.first);
            getters[field.first] = [field](void *model)
                {
                    return field.second(static_cast<Model *>(model));
                };
        }
        _keys.insert({typeid(Model).name(), std::move(keys)});
        _models.insert({typeid(Model).name(), std::move(getters)});
        _tables.insert({typeid(Model).name(), std::move(table)});
        _id_fields[typeid(Model).name()] = id_field;
    }

    template<class Model>
    void addSchema(std::string id_field, std::map<std::string, std::function<std::string(Model *)>> fields)
    {
        std::cout << "Add model:" << typeid(Model).name() << std::endl;

        addSchema(id_field, typeid(Model).name(), std::move(fields));
    }


    template<class Model>
    std::shared_ptr<Model> load(std::string id)
    {
        std::string model_name = typeid(Model).name();


        auto &keys = _keys[model_name];

        if(keys.empty())
            throw std::invalid_argument(std::string("Model:" + model_name + " was not registered"));

        std::string table_name = _tables[model_name];

        if(table_name.empty())
            throw std::invalid_argument(
                    std::string("Model:" + model_name + " was not registered, table name is empty"));

        std::string id_field = _id_fields[model_name];

        if(id_field.empty())
            throw std::invalid_argument(std::string("Model:" + model_name + " was not registered, id_field is empty"));

        std::map<std::string, std::string> data = _loadData(table_name, id_field, keys);
        return std::make_shared<Model>(std::move(data));
    }

    void _saveData(std::string table_name, std::map<std::string, std::string> data);

    template<class Model>
    void save(Model &model)
    {
        std::string model_name = typeid(Model).name();
        auto fields = _models[model_name];

        if(fields.empty())
            throw std::invalid_argument(std::string("Model:" + model_name + " was not registered"));

        std::string table_name = _tables[model_name];

        if(table_name.empty())
            throw std::invalid_argument(
                    std::string("Model:" + model_name + " was not registered, table name is empty"));

        std::map<std::string, std::string> data;
        for(auto &it: fields)
            data[it.first] = it.second(static_cast<void *>(&model));

        _saveData(table_name, data);
    }
private:
    sql::Connection *getConnection();

    void releaseConnection(sql::Connection *c);

    std::map<std::string, std::string>
    _loadData(std::string table, std::string id_field, std::vector<std::string> &fields);

    std::map<std::string, std::string> _tables;
    std::map<std::string, std::string> _id_fields;
    std::map<std::string, std::vector<std::string>> _keys;
    std::map<std::string, std::map<std::string, Getter>> _models;

    std::vector<ConnectionWithInfo> _connection_pool;

    std::mutex _mutex;
    std::condition_variable _connection_notify;
};


#endif //TIC_TAC_TOE_DB_H
