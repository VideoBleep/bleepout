//
//  MessageQueue.h
//  bleepout
//
//  Created by tekt on 12/26/14.
//
//

#ifndef __bleepout__MessageQueue__
#define __bleepout__MessageQueue__

#include <ofTypes.h>
#include <ofEventUtils.h>
#include <deque>
#include <list>

template<typename T>
class MessageQueue {
public:
  void pushMessage(const T& message) {
    ofScopedLock lock(_mutex);
    _messages.push_back(message);
  }
  std::vector<T> popMessages() {
    ofScopedLock lock(_mutex);
    std::vector<T> popped;
    while (!_messages.empty()) {
      popped.push_back(_messages.front());
      _messages.pop_front();
    }
    return popped;
  }
private:
  std::list<T> _messages;
  ofMutex _mutex;
};

template<typename T>
class EventMessageQueue {
public:
  EventMessageQueue(ofEvent<T>& event)
  : _event(event), _queue() { }
  
  void pushMessage(const T& message) {
    _queue.pushMessage(message);
  }
  void sendEvents() {
    std::vector<T> messages = _queue.popMessages();
    for (T& message : messages) {
      ofNotifyEvent(_event, message);
    }
  }
private:
  MessageQueue<T> _queue;
  ofEvent<T>& _event;
};

#endif /* defined(__bleepout__MessageQueue__) */
