#pragma once

#include <queue>
#include <memory>
#include <definitions/message.h>

using namespace std;

extern queue<shared_ptr<PositionChangedMessage>> serialPositionChangedQueue;
extern queue<shared_ptr<PositionChangedMessage>> displayPositionChangedQueue;
