#ifndef CBUFFER_H
#define CBUFFER_H

#include "common.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_array.hpp>

class CBuffer :
        public boost::enable_shared_from_this< CBuffer >,
        protected boost::noncopyable
{
public:
    typedef boost::shared_ptr< CBuffer > BufferPtr;
    typedef boost::shared_array< char > Array;

public:
    static BufferPtr get(size_t size = DEFAULT_SHAREDBUFFER_SIZE);

    void reset();

    char *memory();
    size_t total();
    size_t current();

    void setCurrrent(size_t size);

private:
    Array _memory;
    size_t _total;
    size_t _current;

protected:
    CBuffer(size_t size);
};

#endif // CBUFFER_H
