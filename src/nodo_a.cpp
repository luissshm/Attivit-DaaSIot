#include "daas_chat.hpp"
#include "daas_log.hpp"
#include <cstring>

void runNodoA(const std::string &local_ip, const std::string &remote_ip) {
    DaasLogger::instance().setLogFile("logs/nodo_a.log");
    DaasLogger::instance().log(LogLevel::INFO, "NODE", "=== Starting NodoA ===");

    remoteDin = 200; // NodoB

    // ---- Init Core ----
    int resInit = node.doInit(100, 110);
    DaasLogger::instance().log(LogLevel::INFO, "doInit", "Result: " + std::to_string(resInit));

    // ---- Enable Driver ----
    int resDriver = node.enableDriver(_LINK_INET4, (local_ip + ":2020").c_str());
    DaasLogger::instance().log(LogLevel::INFO, "enableDriver", "Result: " + std::to_string(resDriver));

    // ---- Map Node ----
    int resMap = node.map(remoteDin, _LINK_INET4, (remote_ip + ":2024").c_str());
    DaasLogger::instance().log(LogLevel::INFO, "mapNode", "Result: " + std::to_string(resMap));

    // ---- List Drivers ----
    const char* drivers = node.listAvailableDrivers();
    DaasLogger::instance().log(LogLevel::INFO, "listAvailableDrivers", drivers ? drivers : "NULL");

    // ---- Version / Build ----
    DaasLogger::instance().log(LogLevel::INFO, "getVersion", node.getVersion());
    DaasLogger::instance().log(LogLevel::INFO, "getBuildInfo", node.getBuildInfo());

    // ---- Node Status ----
    nodestate_t status = node.getStatus();
    DaasLogger::instance().log(LogLevel::INFO, "getStatus", 
        "linked=" + std::to_string(status.linked) + " in_sync=" + std::to_string(status.in_sync));

    // ---- Start Loopback / Receiver Threads ----
    node.locate(remoteDin);
    std::thread receiver(receiveMessages);
    std::thread sender(sendMessages);

    // ---- DoPerform example ----
    int resPerform = node.doPerform(PERFORM_CORE_NO_THREAD);
    DaasLogger::instance().log(LogLevel::INFO, "doPerform", "Result: " + std::to_string(resPerform));

    sender.join();
    running = false;
    receiver.join();

    // ---- Frisbee Test ----
    int resFrisbee = node.frisbee(remoteDin);
    DaasLogger::instance().log(LogLevel::INFO, "frisbee", "Result: " + std::to_string(resFrisbee));

    // ---- Push / Pull example ----
    DDO out(12);
    const char* msg = "Hello from NodoA log test";
    out.setPayload(msg, strlen(msg));
    out.setTimestamp(0);
    int resPush = node.push(remoteDin, &out);
    DaasLogger::instance().log(LogLevel::INFO, "push", "Result: " + std::to_string(resPush));

    DDO* inboundData = nullptr;
    int resPull = node.pull(remoteDin, &inboundData);
    DaasLogger::instance().log(LogLevel::INFO, "pull", "Result: " + std::to_string(resPull));
    if (inboundData) {
        unsigned char buffer[1025] = {0};
        inboundData->getPayloadAsBinary(buffer, 0, inboundData->getPayloadSize());
        buffer[inboundData->getPayloadSize()] = '\0';
        DaasLogger::instance().log(LogLevel::INFO, "pull_payload", (char*)buffer);
    }

    DaasLogger::instance().log(LogLevel::INFO, "NODE", "=== NodoA terminated ===");
}
