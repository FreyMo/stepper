#pragma once

#include <queue>
#include <memory>
#include <definitions/message.h>

extern std::queue<std::shared_ptr<PositionChangedMessage>> serialPositionChangedQueue;
extern std::queue<std::shared_ptr<PositionChangedMessage>> displayPositionChangedQueue;
