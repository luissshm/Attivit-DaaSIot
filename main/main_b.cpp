#include <string>
#include <iostream>
#include "../src/daas_log.hpp"

void runNodoB(const std::string &local_ip, const std::string &remote_ip);

int main() {
    // Initialize logger for Node B
    DaasLogger::instance().setLogFile("logs/nodo_b.log");
    DaasLogger::instance().log(LogLevel::INFO, "MAIN", "=== Starting NodoB ===");
    DaasLogger::instance().log(LogLevel::INFO, "MAIN", "Local IP: 127.0.0.1, Remote IP: 127.0.0.1");

    try {
        runNodoB("127.0.0.1", "127.0.0.1");
    } catch (const std::exception &e) {
        DaasLogger::instance().log(LogLevel::ERROR, "MAIN", std::string("Unhandled exception: ") + e.what());
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    DaasLogger::instance().log(LogLevel::INFO, "MAIN", "=== NodoB terminated ===");
    return 0;
}
