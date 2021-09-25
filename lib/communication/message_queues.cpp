#include <message_queues.h>

using namespace std;

queue<shared_ptr<PositionChangedMessage>> serialPositionChangedQueue;
queue<shared_ptr<PositionChangedMessage>> displayPositionChangedQueue;
