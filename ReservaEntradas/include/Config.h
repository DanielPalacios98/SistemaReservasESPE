#pragma once
#include <string>
using namespace std;

class Config {
public:
    string backend;         // "json" | "mongo"
    string mongoUri;        // mongodb connection string
    string mongoDb;
    string mongoCollection;

    Config() : backend("json"), mongoUri(""), mongoDb(""), mongoCollection("") {}

    static Config load(const string& path);
};
