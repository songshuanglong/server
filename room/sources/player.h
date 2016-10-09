#ifndef CPLAYER_H
#define CPLAYER_H

#include "common.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class CNetConnection;

class CPlayer :
        public boost::enable_shared_from_this< CPlayer >,
        protected boost::noncopyable
{
public:
    typedef boost::shared_ptr< CPlayer > PlayerPtr;

public:
    static PlayerPtr get();

    bool onUserMessage(unsigned short type, void *data, int len);
    bool onGameMessage(unsigned short type, void *data, int len);

protected:
    CPlayer();

private:
};

#endif // CPLAYER_H
