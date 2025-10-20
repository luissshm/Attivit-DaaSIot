#include "daas_chat.hpp"
#include "daas_log.hpp"

din_t localDin, localSid;
din_t remoteDin;
DaasAPI node;
std::atomic<bool> running(true);
std::mutex ioMutex;

bool initCore(int sid, int din) {
    localSid = sid;
    localDin = din;
    int res = node.doInit(sid, din);
    if (res != ERROR_NONE) {
        std::lock_guard<std::mutex> lock(ioMutex);
        DaasLogger::instance().log(LogLevel::ERROR, "INIT", "Failed to initialize node: code " + std::to_string(res));
        return false;
    }
    DaasLogger::instance().log(LogLevel::INFO, "INIT", "Node initialized successfully (SID=" + std::to_string(sid) +
                                 ", DIN=" + std::to_string(din) + ")");
    return true;
}

bool enableDriver(const char *uri) {
    int res = node.enableDriver(_LINK_INET4, uri);
    if (res != ERROR_NONE) {
        std::lock_guard<std::mutex> lock(ioMutex);
        DaasLogger::instance().log(LogLevel::ERROR, "DRIVER", "Failed to enable driver: " + std::string(uri) +
                                      " (code " + std::to_string(res) + ")");
        return false;
    }
    DaasLogger::instance().log(LogLevel::INFO, "DRIVER", "Driver enabled on " + std::string(uri));
    return true;
}

bool mapNode(din_t din, link_t link, const char *uri) {
    int res = node.map(din, link, uri);
    if (res != ERROR_NONE) {
        std::lock_guard<std::mutex> lock(ioMutex);
        DaasLogger::instance().log(LogLevel::ERROR, "MAP", "Failed to map node DIN=" + std::to_string(din) +
                                      " to " + std::string(uri) + " (code " + std::to_string(res) + ")");
        return false;
    }
    DaasLogger::instance().log(LogLevel::INFO, "MAP", "Mapped remote node DIN=" + std::to_string(din) +
                                " via " + std::string(uri));
    return true;
}

void receiveMessages() {
    DaasLogger::instance().log(LogLevel::INFO, "RX", "Receiver thread started");
    while (running) {
        node.doPerform(PERFORM_CORE_NO_THREAD);

        DDO* inboundData = nullptr;
        unsigned char buffer[1025] = {0}; // +1 for null terminator

        if (node.pull(remoteDin, &inboundData) == ERROR_NONE && inboundData != nullptr) {
            uint32_t size = inboundData->getPayloadSize();
            inboundData->getPayloadAsBinary(buffer, 0, size);
            buffer[size] = '\0'; // Ensure null termination
            std::string payload((char*)buffer);

            {
                std::lock_guard<std::mutex> lock(ioMutex);
                DaasLogger::instance().log(LogLevel::INFO, "RX", "Received message from " + std::to_string(remoteDin) + ": " + payload);
                std::cout << "\n[Received] " << payload << std::endl;
                std::cout << "Type a message ('exit' to quit): ";
                std::cout.flush();
            }
            
            inboundData->clearPayload();
            delete inboundData;
            inboundData = nullptr;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    DaasLogger::instance().log(LogLevel::INFO, "RX", "Receiver thread terminated");
}

void sendMessages() {
    DaasLogger::instance().log(LogLevel::INFO, "TX", "Sender thread started");
    while (running) {
        std::string msg;
        {
            std::lock_guard<std::mutex> lock(ioMutex);
            std::cout << "Type a message ('exit' to quit): ";
        }
        std::getline(std::cin, msg);

        if (msg == "exit") {
            running = false;
            DaasLogger::instance().log(LogLevel::INFO, "TX", "Exit command received. Stopping...");
            break;
        }

        DDO out;
        out.allocatePayload(msg.size());
        out.setPayload(msg.c_str(), msg.size());
        out.setTimestamp(0);

        int res = node.push(remoteDin, &out);
        {
            std::lock_guard<std::mutex> lock(ioMutex);
            if (res == ERROR_NONE) {
                DaasLogger::instance().log(LogLevel::INFO, "TX", "Sent message to " + std::to_string(remoteDin) + ": " + msg);
                std::cout << "[Sent] " << msg << std::endl;
            } else {
                DaasLogger::instance().log(LogLevel::ERROR, "TX", "Failed to send message (code " + std::to_string(res) + ")");
                std::cerr << "Error sending message: " << res << std::endl;
            }
        }
        out.clearPayload();
    }

    DaasLogger::instance().log(LogLevel::INFO, "TX", "Sender thread terminated");
    node.doEnd();
}
