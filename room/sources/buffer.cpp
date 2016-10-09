#include "buffer.h"
#include <string.h>

CBuffer::CBuffer(size_t size) :
    _memory(new char[size]),
    _total(size),
    _current(0)
{

}

CBuffer::BufferPtr CBuffer::get(size_t size)
{
    BufferPtr _new(new CBuffer(size));

    return _new;
}

void CBuffer::reset()
{
    memset(_memory.get(), 0, _total);
}

char *CBuffer::memory()
{
    return _memory.get();
}

size_t CBuffer::total()
{
    return _total;
}

size_t CBuffer::current()
{
    return _current;
}

void CBuffer::setCurrrent(size_t size)
{
    _current = size;
}
