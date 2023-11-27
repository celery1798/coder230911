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

struct msg_s2c_st
{
	long mtype;     /*must be MSG_DATA*/
	int datalen;
	/*
		> 0 datamsg
		== 0 eotmsg
		< 0 -errno errmsg
	*/
	char data[DATASIZE];
};

#endif
/*
s:
path_st path
msgrcv(msgid,&path,,MSG_PATH,);

 = fopen();

struct msg_s2c_st msg;
err.mtype = MSG_ERR;
err.datalen = -errno

msgsnd();


c:
msgsnd(  path  );

struct msg_s2c_st rmsg;
msgrcv(  &rmsg    );

if(rmsg.datalen < 0)

else if( == 0)
....



*/

