//
// Created by serg on 10/25/16.
//

#include "FakeStore.h"

FakeStore::Record FakeStore::loadData(std::string table_name, std::string id_field, std::string id_value,
                                      const std::vector<std::string> &fields) const {
    auto table = _tables.at(table_name);

    if (table.empty())
        return Record();

    for (auto &rec:table) {
        if (rec[id_field] == id_value) {
            return rec;
        }
    }
    return std::map<std::string, std::string>();
}

void FakeStore::saveData(std::string table_name, const FakeStore::Record &data) const {
    _tables[table_name].push_back(data);
}
