/*
 * AbstractSubject.h
 *
 *  Created on: 02/04/2012
 *      Author: arliones
 */

#ifndef ABSTRACTSUBJECT_H_
#define ABSTRACTSUBJECT_H_

#include <set>

using namespace std;

template<typename T>
class AbstractSubject
{
public:
    class Observer {
    public:
        virtual ~Observer() {}
        virtual void notify(const T & __t) = 0;
    };

    AbstractSubject() {}
    virtual ~AbstractSubject() {}

    void attach_observer(Observer * __obs) {
        observers.insert(__obs);
    }
    void detach_observer(Observer * __obs) {
        observers.erase(__obs);
    }
    void notify_all(const T & __t)
    {
        typename set<Observer*>::iterator it = observers.begin();
        for(; it != observers.end(); ++it)
            (*it)->notify(__t);
    }

private:
    set<Observer*> observers;
};

#endif /* ABSTRACTSUBJECT_H_ */
