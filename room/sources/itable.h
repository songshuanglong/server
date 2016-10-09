#ifndef ITABLE_H
#define ITABLE_H

class ITable
{
public:
    virtual unsigned short id() = 0;
    virtual unsigned char status() = 0;
    virtual void setStatus(unsigned char state) = 0;
};

#endif // ITABLE_H
