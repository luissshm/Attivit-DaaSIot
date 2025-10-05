#ifndef DAAS_CHAT_HPP
#define DAAS_CHAT_HPP

#include "../include/daas.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

extern din_t localDin, localSid;
extern din_t remoteDin;
extern DaasAPI node;
extern std::atomic<bool> running;
extern std::mutex ioMutex; // Protect console I/O

extern std::atomic<bool> running;

bool initCore(int sid, int din);
bool enableDriver(const char *uri);
bool mapNode(din_t din, link_t link, const char *uri);

void receiveMessages();
void sendMessages();

void logEvent(const std::string &context, int code, const std::string &details = "");

#endif // DAAS_CHAT_HPP
