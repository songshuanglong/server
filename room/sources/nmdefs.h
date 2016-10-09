#ifndef NMDEFS_H
#define NMDEFS_H
#pragma pack(push, 1)
#ifdef __cplusplus
extern "C" {
#endif

	// user token length
#define USER_TOKEN_LEN              32

// single net message limit length
#define NM_LIMITS_BODY_LEN          1024*2

//
#define TABLE_PASSWORD_LEN          6

//
#define VERSION_MAJOR               0

//
#define VERSION_MINOR               1

//
//
#define NM_SYS        1

//
#define NM_USER       2

//
#define NM_GAME       3

//
#define NM_BEAT       4

//
//
//
//
//
#define NM_SYS_LOGIN_REQUEST      10

//
#define NM_SYS_LOGIN_SUCCESS      1010

//
#define NM_SYS_LOGIN_FAIL         1011

//
#define NM_SYS_SERVER_INFO        1012

//
//
//
//
//#define NM_USER_CREATE_TABLE      41
#define NM_USER_REQUEST_TABLE               10

//
#define NM_USER_TABLE_INFO                  1010

//
#define NM_USER_REQUEST_TABLE_FAIL          1011

//
#define NM_USER_CHAT                        40

//
#define NM_USER_SITDOWN                     42

//
#define NM_USER_READY                       43
#define NM_USER_CANCELREADY                 44

//
#define NM_USER_LEAVE                       45

//
#define NM_USER_STARTGAME                   46

//
#define NM_USER_FORCEQUIT                   47

//
#define NM_USER_WATCH                       48

//
//
//
#define NM_BEAT_REQUEST           15

//
#define NM_BEAT_REPLY             16


//
//
//

struct _tag_nm_common_header
{
	//
	unsigned short  version;

	//
	unsigned short  checksum;

	//
	unsigned int    len;

	//
	//
	unsigned int    cmd;

	//
	unsigned char   data[];
};

struct _tag_nm_login_request
{
	//
	unsigned short uid;

	//
	unsigned char token[USER_TOKEN_LEN];
};

struct _tag_nm_user_request_table
{
	//
	unsigned short tid;

	//
	unsigned short uc;

	//
	unsigned short uid;

	//
	unsigned int score;

	//
	unsigned int time;

	//
	//unsigned char password[TABLE_PASSWORD_LEN];
};

struct _tag_nm_user_data
{

};

typedef struct _tag_nm_common_header NMCommonHeader;
typedef struct _tag_nm_login_request NMSysLoginRequestData;
typedef struct _tag_nm_user_request_table NMUserRequestTableData;
typedef struct _tag_nm_user_data NMUserData;

#ifdef __cplusplus
}
#endif
#pragma pack(pop)
#endif // NMDEFS_H
