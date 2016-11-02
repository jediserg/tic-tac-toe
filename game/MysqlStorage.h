/*!
 *\brief It's a strategy class can be sent as a template parameter to the Store class.
 *
 * This class can save/load data(as a field to value map, both can be only string,
 * but values could be converted to other types latter) to/from a table.
 */

#ifndef TIC_TAC_TOE_READER_H
#define TIC_TAC_TOE_READER_H

#include <string>
#include <memory>
#include <set>
#include <vector>
#include <map>

#include "ConnectionPool.h"

class MysqlStorage {
public:
    MysqlStorage(std::string host, std::string user, std::string password, std::string database,
                 int pool_size);

    std::map<std::string, std::string>
    loadData(std::string table, std::string id_field, std::string id_value,
             const std::vector<std::string> &fields) const;

    void saveData(std::string table_name, const std::map<std::string, std::string> &data) const;

    void createTable(std::string table, std::string id_field, std::vector<std::string> fields) const;

    void dropTable(std::string table) const;

private:
    std::unique_ptr<ConnectionPool> _pool;
    std::string _host;
    std::string _user;
    std::string _password;
    std::string _database;
    int _pool_size;


};


#endif //TIC_TAC_TOE_READER_H
