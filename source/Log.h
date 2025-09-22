#pragma once

#include <plugin.h>
#include <CMessages.h>
#include <fstream>
#include <string>

using namespace plugin;

struct Log {
    static std::ofstream file;

    static void Init(const std::string& filename = "MotorEngine.log") {
        file.open(filename, std::ios::out | std::ios::app);
        if (file.is_open()) {
            file << "=== Logger iniciado ===" << std::endl;
        }
    }

    static void Close() {
        if (file.is_open()) {
            file << "=== Logger finalizado ===" << std::endl;
            file.close();
        }
    }
};