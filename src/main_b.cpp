#include <string>
void runNodoB(const std::string &local_ip, const std::string &remote_ip);

int main() {
    runNodoB("127.0.0.1", "127.0.0.1");
    return 0;
}
