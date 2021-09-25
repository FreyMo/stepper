#include <message_queues.h>

using namespace std;

shared_ptr<queue<shared_ptr<PositionChangedMessage>>> serialPositionChangedQueue = shared_ptr<queue<shared_ptr<PositionChangedMessage>>>(new queue<shared_ptr<PositionChangedMessage>>());
shared_ptr<queue<shared_ptr<PositionChangedMessage>>> displayPositionChangedQueue = shared_ptr<queue<shared_ptr<PositionChangedMessage>>>(new queue<shared_ptr<PositionChangedMessage>>());
