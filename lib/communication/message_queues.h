#pragma once

#include <queue>
#include <memory>
#include <definitions/message.h>

using namespace std;

extern shared_ptr<queue<shared_ptr<PositionChangedMessage>>> serialPositionChangedQueue;
extern shared_ptr<queue<shared_ptr<PositionChangedMessage>>> displayPositionChangedQueue;
