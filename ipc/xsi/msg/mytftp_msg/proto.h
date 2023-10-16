#ifndef _PROTO_H__
#define _PROTO_H__

#define KEYPATH		"/etc/hosts"
#define KEYPROJ		'a'	

#define PATHSIZE	1024
#define DATASIZE	1024

enum
{
	MSG_PATH=1,
	MSG_DATA,
	MSG_ERR,
	MSG_EOT
};

struct msg_path_st  
{
	long mtype;		/*must be MSG_PATH*/
	char path[PATHSIZE];
};

struct msg_data_st
{
	long mtype;     /*must be MSG_DATA*/
	int datalen;
	char data[DATASIZE];
};

struct msg_err_st
{
	long mtype;			/*must be MSG_ERR*/
	int errno;
}

struct msg_eot_st
{
	long mtype;			/*must be MSG_EOT*/
}

#endif
/*
s:
path_st path
msgrcv(msgid,&path,,MSG_PATH,);

 = fopen();

struct msg_err_st errmsg
errmsg.errno = errno

msgsnd();


c:
msgsnd(  path  );

msgrcv(   ???   );

*/

