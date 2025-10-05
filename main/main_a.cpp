#include <string>
#include <iostream>
#include "../src/daas_log.hpp"

void runNodoA(const std::string &local_ip, const std::string &remote_ip);

int main() {
    // Initialize logger for Node A
    DaasLogger::instance().setLogFile("logs/nodo_a.log");
    DaasLogger::instance().log(LogLevel::INFO, "MAIN", "=== Starting NodoA ===");
    DaasLogger::instance().log(LogLevel::INFO, "MAIN", "Local IP: 127.0.0.1, Remote IP: 127.0.0.1");

    try {
        runNodoA("127.0.0.1", "127.0.0.1"); // LOCAL and REMOTE IP in loopback
    } catch (const std::exception &e) {
        DaasLogger::instance().log(LogLevel::ERROR, "MAIN", std::string("Unhandled exception: ") + e.what());
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    DaasLogger::instance().log(LogLevel::INFO, "MAIN", "=== NodoA terminated ===");
    return 0;
}
