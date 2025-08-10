#include "daas_chat.hpp"

void runNodoB(const std::string &local_ip, const std::string &remote_ip) {
    remoteDin = 110; // NodoA

    if (!initCore(100, 200)) return;
    if (!enableDriver((local_ip + ":2024").c_str())) return;
    if (!mapNode(remoteDin, _LINK_INET4, (remote_ip + ":2020").c_str())) return;

    node.locate(remoteDin); // Locate only once

    std::thread receiver(receiveMessages);
    std::thread sender(sendMessages);

    sender.join();
    running = false;
    receiver.join();
}

/* int main() { */
/*     std::cout << "Ejecutando Nodo B (IP local: 127.0.0.1, IP remota: 127.0.0.1)\n"; */
/*     runNodoB("127.0.0.1", "127.0.0.1"); */
/*     std::cout << "Nodo B terminado.\n"; */
/*     return 0; */
/* } */
