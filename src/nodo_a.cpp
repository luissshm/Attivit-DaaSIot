#include "daas_chat.hpp"

void runNodoA(const std::string &local_ip, const std::string &remote_ip) {
    remoteDin = 200; // NodoB

    if (!initCore(100, 110)) return;
    if (!enableDriver((local_ip + ":2020").c_str())) return;
    if (!mapNode(remoteDin, _LINK_INET4, (remote_ip + ":2024").c_str())) return;

    node.locate(remoteDin);

    std::thread receiver(receiveMessages);
    std::thread sender(sendMessages);

    sender.join();
    running = false;
    receiver.join();
}
