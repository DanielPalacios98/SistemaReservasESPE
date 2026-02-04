#include "Config.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>
using namespace std;

static string extractJsonString(const string& content, const string& key) {
    // Busca "key" : "value" ignorando espacios
    regex re("\\\"" + key + "\\\"\\s*:\\s*\\\"([^\\\"]*)\\\"");
    smatch m;
    if (regex_search(content, m, re)) return m[1].str();
    return "";
}

Config Config::load(const string& path) {
    Config cfg;
    // Intentar varias ubicaciones relativas (útil al ejecutar desde build/Release)
    vector<string> candidates = { path, string("../") + path, string("../../") + path };
    ifstream in;
    for (const auto& p : candidates) {
        in.open(p);
        if (in.is_open()) break;
    }
    if (!in.is_open()) return cfg; // defaults
    string content;
    in.seekg(0, ios::end);
    content.reserve(static_cast<size_t>(in.tellg()));
    in.seekg(0, ios::beg);
    content.assign(istreambuf_iterator<char>(in), istreambuf_iterator<char>());
    in.close();

    string backend = extractJsonString(content, "backend");
    if (!backend.empty()) cfg.backend = backend;
    cfg.mongoUri = extractJsonString(content, "mongoUri");
    cfg.mongoDb = extractJsonString(content, "mongoDb");
    cfg.mongoCollection = extractJsonString(content, "mongoCollection");
    return cfg;
}
