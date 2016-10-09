#ifndef CPOOL_H
#define CPOOL_H

#include "common.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <set>

template< typename T >
class CPool
{
public:
    typedef boost::shared_ptr< CPool< T > > PoolPtr;
    typedef boost::shared_ptr< T > Element;
    typedef std::set< Element > ElementQueue;
    typedef boost::mutex Mutex;

public:
    static PoolPtr get()
    {
        PoolPtr _new(new CPool< T >());

        return _new;
    }

    Element alloc()
    {
        Element e;
        Mutex::scoped_lock lock(_lock);

        if(_elementQueue.empty()){
            e = T::get();
            ++_elementCount;
        }else{
            for(typename ElementQueue::iterator it = _elementQueue.begin(); it != _elementQueue.end(); ++it){
                if((*it).use_count() == 1){
                    e = *it;
                    break;
                }
            }
            if(!e.get()){
                e = T::get();
                ++_elementCount;
            }
        }

        return e;
    }

    void free(const Element &e)
    {
        Mutex::scoped_lock lock(_lock);

        typename ElementQueue::iterator it = _elementQueue.find(e);
        if(it == _elementQueue.end()){
            _elementQueue.insert(e);
        }
    }

protected:
    CPool() :   _elementCount(0)
    {

    }

private:
    Mutex _lock;

    ElementQueue _elementQueue;

    unsigned int _elementCount;
};

#endif // CPOOL_H
