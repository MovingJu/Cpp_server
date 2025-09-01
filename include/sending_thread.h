#pragma once

#include <atomic>

void sending_thread();
extern std::atomic<bool> running;