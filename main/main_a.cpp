#include <string>
void runNodoA(const std::string &local_ip, const std::string &remote_ip);

int main() {
    runNodoA("127.0.0.1", "127.0.0.1"); // LOCAL and REMOTE IP in loopback
    return 0;
}
