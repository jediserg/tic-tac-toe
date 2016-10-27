//
// Created by serg on 10/24/16.
//

#include <map>
#include <vector>
#include "MysqlStorage.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>

void MysqlStorage::createTable(std::string table, std::string id_field, std::vector<std::string> fields) {
    auto connection = _pool->getConnection();

    std::stringstream query_stream;
    query_stream << "CREATE table  `" << table << "` (";

    bool is_first = true;
    for (auto &field: fields) {
        if (!is_first) {
            query_stream << ',';
        } else {
            is_first = false;
        }

        query_stream << "`" << field << "` VARCHAR(255)";
        if (id_field == field)
            query_stream << " PRIMARY KEY ";
    }
    query_stream << ')';

    std::unique_ptr<sql::Statement> stmt(connection->createStatement());
    stmt->execute(query_stream.str());
}

void MysqlStorage::dropTable(std::string table) {
    auto connection = _pool->getConnection();

    std::stringstream query_stream;
    query_stream << "DROP TABLE  `" << table;

    std::unique_ptr<sql::Statement> stmt(connection->createStatement());
    stmt->execute(query_stream.str());
}

std::map<std::string, std::string>
MysqlStorage::loadData(std::string table, std::string id_field, std::string id_value,
                       std::vector<std::string> &fields) {
    auto connection = _pool->getConnection();

    std::map<std::string, std::string> result;

    std::stringstream query_stream;

    query_stream << "SELECT * FROM `" << table << "` WHERE `" << id_field << "`=?";
    std::string query = query_stream.str();

    std::unique_ptr<sql::PreparedStatement> prepared_stmt(connection->prepareStatement(query));
    prepared_stmt->setString(1, id_value);

    std::unique_ptr<sql::ResultSet> query_result(prepared_stmt->executeQuery());

    if (!query_result->next())
        return result;

    for (auto &field:fields) {
        result[field] = query_result->getString(field);
    }

    return result;
}

void MysqlStorage::saveData(std::string table_name, const std::map<std::string, std::string> &data) {
    auto connection = _pool->getConnection();

    std::stringstream query_stream;
    std::stringstream insert_values_stream;
    std::stringstream update_values_stream;

    query_stream << "INSERT INTO  `" << table_name << "` (";

    bool is_first = true;
    for (auto &field: data) {
        if (!is_first) {
            query_stream << ',';
            insert_values_stream << ',';
            update_values_stream << ',';
        } else
            is_first = false;

        query_stream << '`' << field.first << '`';
        insert_values_stream << '?';
        update_values_stream << '`' << field.first << "`=" << '?';

    }
    query_stream << ") VALUES (";

    query_stream << insert_values_stream.str() << ") ON DUPLICATE KEY UPDATE " << update_values_stream.str();
    std::string query = query_stream.str();
    std::unique_ptr<sql::PreparedStatement> prepared_stmt(connection->prepareStatement(query));

    unsigned int field_counter = 1;

    for (auto &field: data) {
        prepared_stmt->setString(field_counter, field.second);
        prepared_stmt->setString(field_counter++ + data.size(), field.second);
    }

    prepared_stmt->execute();
}

MysqlStorage::MysqlStorage(std::string host, std::string user, std::string password, std::string database,
                           int pool_size)
        : _pool(new ConnectionPool(host, user, password, database, pool_size)), _host(host), _user(user),
          _password(password), _database(database), _pool_size(pool_size) {}
