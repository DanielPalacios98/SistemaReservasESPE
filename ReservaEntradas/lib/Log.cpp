#include "include/Log.h"
#include <fstream>
#include <filesystem>

static const char* LOG_DIR = "logs";
static const char* LOG_FILE = "logs/app.log";

static void ensureLogDir() {
    std::error_code ec;
    std::filesystem::create_directories(LOG_DIR, ec);
}

static void write(const char* level, const std::string& msg) {
    ensureLogDir();
    std::ofstream out(LOG_FILE, std::ios::app);
    if (!out.is_open()) return;
    out << level << " " << msg << "\n";
    out.close();
}

void Log::info(const std::string& msg) { write("[INFO]", msg); }
void Log::error(const std::string& msg) { write("[ERROR]", msg); }
