//
// Created by serg on 10/25/16.
//

#ifndef TIC_TAC_TOE_FAKESTORE_H
#define TIC_TAC_TOE_FAKESTORE_H


#include <string>
#include <map>
#include <vector>

class FakeStore {
public:
    using Record = std::map<std::string, std::string>;
    using Table = std::vector<Record>;
    using Tables = std::map<std::string, Table>;

    Record
    loadData(std::string table_name, std::string id_field, std::string id_value, std::vector<std::string> &fields);

    void saveData(std::string table_name, const Record &data);

private:
    Tables _tables;
};


#endif //TIC_TAC_TOE_FAKESTORE_H
