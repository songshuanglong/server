#ifndef PLAYERDATA_H
#define PLAYERDATA_H

// 
#define PS_FREE     0
#define PS_STANDON  1
#define PS_SITDOWN  2
#define PS_READY    3
#define PS_WATCH    4
#define PS_GAMING   5
#define PS_OFFLINE  6

struct _tag_player_data
{
    // ID
    unsigned int _id;

    // 
    unsigned char _sex;

    // 
    unsigned char _status;

    // 
    unsigned char _level;

    // 
    unsigned short _tid;

    // 
    unsigned short _cid;

    // 
    long long _experience;

    // 
    long long _score;

    // 
    long long _diamond;

    // 
    long long _gold;
};

typedef struct _tag_player_data SPlayerData;

#endif // PLAYERDATA_H
