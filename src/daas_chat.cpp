#include "daas_chat.hpp"

din_t localDin, localSid;
din_t remoteDin;
DaasAPI node;
std::atomic<bool> running(true);
std::mutex ioMutex;

// ---------- Utility Logging ----------
void logEvent(const std::string &context, int code, const std::string &details) {
    std::lock_guard<std::mutex> lock(ioMutex);
    std::string msg = "[LOG][" + context + "] code=" + std::to_string(code);
    if (!details.empty()) msg += " | " + details;
    std::cout << msg << std::endl;
}


bool initCore(int sid, int din) {
    localSid = sid;
    localDin = din;
    int res = node.doInit(sid, din);
    if (res != ERROR_NONE) {
        std::lock_guard<std::mutex> lock(ioMutex);
        std::cerr << "Error initCore: " << res << std::endl;
        return false;
    }
    return true;
}

bool enableDriver(const char *uri) {
    int res = node.enableDriver(_LINK_INET4, uri);
    if (res != ERROR_NONE) {
        std::lock_guard<std::mutex> lock(ioMutex);
        std::cerr << "Error enableDriver: " << res << std::endl;
        return false;
    }
    return true;
}

bool mapNode(din_t din, link_t link, const char *uri) {
    int res = node.map(din, link, uri);
    if (res != ERROR_NONE) {
        std::lock_guard<std::mutex> lock(ioMutex);
        std::cerr << "Error mapNode: " << res << std::endl;
        return false;
    }
    return true;
}
void receiveMessages() {
    while (running) {
        node.doPerform(PERFORM_CORE_NO_THREAD);

        DDO* inboundData = nullptr;
        unsigned char buffer[1025] = {0}; // +1 for null terminator

        if (node.pull(remoteDin, &inboundData) == ERROR_NONE && inboundData != nullptr) {
            uint32_t size = inboundData->getPayloadSize();
            inboundData->getPayloadAsBinary(buffer, 0, size);
            buffer[size] = '\0'; // Ensure null termination

            {
                std::lock_guard<std::mutex> lock(ioMutex);
                std::cout << "\n[Received] " << (char*)buffer << std::endl;
                std::cout << "Type a message ('exit' to quit): ";
                std::cout.flush();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void sendMessages() {
    while (running) {
        std::string msg;
        {
            std::lock_guard<std::mutex> lock(ioMutex);
            std::cout << "Type a message ('exit' to quit): ";
        }
        std::getline(std::cin, msg);

        if (msg == "exit") {
            running = false;
            break;
        }

        DDO out(msg.size());
        out.allocatePayload(msg.size());
        out.setPayload(msg.c_str(), msg.size());
        out.setTimestamp(0);

        int res = node.push(remoteDin, &out);
        {
            std::lock_guard<std::mutex> lock(ioMutex);
            if (res == ERROR_NONE) {
                std::cout << "[Sent] " << msg << std::endl;
            } else {
                std::cerr << "Error sending message: " << res << std::endl;
            }
        }
    }
}
