#pragma once

#include <list>
#include <memory>
#include <algorithm>
#include <iostream>

class Subscriber {
public:
    virtual void notify() = 0;
};

class Publisher {
public:
    /**
    *  @brief  Add @a sub to subscribers list.
    *  @param[in]  sub is shared_ptr to Subscriber.
    * 
    *  Subscribe @a sub to notification
    */
    void addSubscriber(std::shared_ptr<Subscriber> sub) {
        subscribers.emplace_back(sub);
    }
    /**
    *  @brief  Delete @a sub from subscribers list.
    *  @param[in]  sub  is shared_ptr to Subscriber.
    * 
    *  Unsubscribe @a sub from notification
    */
    void deleteSubscriber(std::shared_ptr<Subscriber> sub) {
        auto it = std::remove_if(subscribers.begin(), subscribers.end(),
                            [sub](auto s) {
                                auto ptr = s.lock();
                                return (!ptr || ptr == sub);
                            });
        subscribers.erase(it, subscribers.end());     
    }
    /**
    *  @brief  Send notification to subscribers.
    */
    void notifyUpdate() {
        for (auto & sub: subscribers) {
            if (auto s = sub.lock(); s) {
                s->notify();
            }
        }
    }
private:
    std::list<std::weak_ptr<Subscriber>> subscribers;
};