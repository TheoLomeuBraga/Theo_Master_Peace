#pragma once
#include <string>
#include <unordered_map>

class Table {
public:
    std::unordered_map<std::string, float> m_floatMap;
    std::unordered_map<std::string, std::string> m_stringMap;
    std::unordered_map<std::string, Table> m_tableMap;

    void setFloat(const std::string& key, float value) {
        m_floatMap[key] = value;
    }

    float getFloat(const std::string& key) {
        return m_floatMap[key];
    }

    void setString(const std::string& key, const std::string& value) {
        m_stringMap[key] = value;
    }

    std::string getString(const std::string& key) {
        return m_stringMap[key];
    }

    void setTable(const std::string& key, const Table& table) {
        m_tableMap[key] = table;
    }

    Table getTable(const std::string& key) {
        return m_tableMap[key];
    }


    
};