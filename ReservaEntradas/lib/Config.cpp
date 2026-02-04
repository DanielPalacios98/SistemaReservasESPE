#include "include/Config.h"
#include <fstream>
#include <iostream>
using namespace std;

static bool isJsonSpace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static string extractJsonString(const string& content, const string& key) {
    // Parser JSON mínimo: busca "key": "value" y devuelve el string sin comillas.
    const string needle = string("\"") + key + "\"";
    size_t pos = content.find(needle);
    if (pos == string::npos) return "";

    pos = content.find(':', pos + needle.size());
    if (pos == string::npos) return "";
    ++pos;
    while (pos < content.size() && isJsonSpace(content.at(pos))) ++pos;
    if (pos >= content.size() || content.at(pos) != '"') return "";
    ++pos;

    string out;
    while (pos < content.size()) {
        const char c = content.at(pos);
        if (c == '\\') {
            if (pos + 1 < content.size()) {
                out.push_back(content.at(pos + 1));
                pos += 2;
                continue;
            }
            break;
        }
        if (c == '"') break;
        out.push_back(c);
        ++pos;
    }
    return out;
}

Config Config::load(const string& path) {
    Config cfg;
    // Intentar varias ubicaciones relativas (útil al ejecutar desde build/Release)

    class CandidateNode {
    public:
        string value;
        CandidateNode* next;

        CandidateNode(const string& v, CandidateNode* n) : value(v), next(n) {}
    };

    CandidateNode* candidates = new CandidateNode(
        path,
        new CandidateNode(
            string("../") + path,
            new CandidateNode(string("../../") + path, nullptr)
        )
    );

    ifstream in;
    for (CandidateNode* cur = candidates; cur != nullptr; cur = cur->next) {
        in.open(cur->value);
        if (in.is_open()) break;
    }

    while (candidates != nullptr) {
        CandidateNode* next = candidates->next;
        delete candidates;
        candidates = next;
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
