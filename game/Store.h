/*!
 * \brief It's a very simple and easy to use ORM.
 *
 * ConcreteStorage is a template strategy responsable for a data storage in specific place
 * (in a mysql: MysqlStorage or in a memory FakeStorage )
 * To use this class we have to add tabless using addTable. It binds a data model classes to a table. See tests/db_check.cpp
 * All data models must have a constructor with std::map<string, string> (field name, field value) and getters for all fields
 * Than we can use save, load, createTable, dropTable just using data model class as template parameter.
 */

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

#include <mutex>
#include <condition_variable>
#include <iostream>
#include "log.h"

template<class ConcreteStore>
class Store {
public:
    using Getter    = std::function<std::string(const void *)>;
    using Getters   = std::map<std::string, Getter>;
    using Fields    = std::vector<std::string>;

private:
    struct ModelInfo {
        ModelInfo(std::string &&_table, std::string &&_id_field, Fields &&_fields, Getters &&_getters)
                : table(std::move(_table)), id_field(std::move(_id_field)), fields(_fields), getters(_getters) {}

        std::string table;
        std::string id_field;
        std::vector<std::string> fields;
        std::map<std::string, Getter> getters;
    };

    using Models    =  std::map<std::string, ModelInfo>;

    template<class Model>
    const ModelInfo &getModelInfo() const {
        std::string model_name = typeid(Model).name();

        if (_models.find(model_name) == _models.end())
            throw std::invalid_argument(std::string("Model:" + model_name + " was not registered"));

        return _models.at(model_name);
    }

public:

    template<class... Args>
    Store(Args ... args)
            : _concrete_store(args...) {

    }


    template<class Model>
    void createTable() const {
        const auto &model = getModelInfo<Model>();

        _concrete_store.createTable(model.table, model.id_field, model.fields);
    }

    template<class Model>
    void dropTable() const {
        const auto &model = getModelInfo<Model>();

        _concrete_store.dropTable(model.table);
    }

    template<class Model>
    void addTable(std::string table, std::string id_field,
                  std::map<std::string, std::function<std::string(const Model *)>> fields) {

        Getters getters;
        Fields keys;

        for (auto &field: fields) {
            keys.push_back(field.first);
            getters[field.first] = [field](const void *model) {
                return field.second(static_cast<const Model *>(model));
            };
        }

        _models.insert({typeid(Model).name(),
                        ModelInfo(std::move(table), std::move(id_field), std::move(keys), std::move(getters))});
    }

    template<class Model>
    void addSchema(std::string id_field, std::map<std::string, std::function<std::string(const Model *)>> fields) {
        LOG_DEBUG << "Add model:" << typeid(Model).name() << std::endl;

        addTable(id_field, typeid(Model).name(), std::move(fields));
    }


    template<class Model>
    std::shared_ptr<Model> load(std::string id) const {
        const auto &model = getModelInfo<Model>();

        std::map<std::string, std::string> data = _concrete_store.loadData(model.table, model.id_field, id,
                                                                           model.fields);
        if (data.empty())
            return nullptr;

        return std::make_shared<Model>(std::move(data));
    }

    template<class Model>
    void save(const Model &model) const {
        const auto &model_info = getModelInfo<Model>();

        std::map<std::string, std::string> data;

        for (auto &it: model_info.getters)
            data[it.first] = it.second(static_cast<const void *>(&model));

        _concrete_store.saveData(model_info.table, data);
    };
private:
    ConcreteStore _concrete_store;
    Models _models;
};

#endif //TIC_TAC_TOE_DB_H
