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

typedef struct msg_path_st  
{
	long mtype;		/*must be MSG_PATH*/
	char path[PATHSIZE];
}msg_path_t;

typedef struct msg_data_st
{
	long mtype;     /*must be MSG_DATA*/
	int datalen;
	char data[DATASIZE];
}msg_data_t;

typedef struct msg_err_st
{
	long mtype;			/*must be MSG_ERR*/
	int errno;
}msg_err_t;

typedef struct msg_eot_st
{
	long mtype;			/*must be MSG_EOT*/
}msg_eot_t;


struct msg_s2c_st
{
	long mtype;
	union msg_s2c_un
	{	
		msg_data_t datamsg;
		msg_err_t errmsg;
		msg_eot_t eotmsg;
	}msg;
};

/*
union msg_s2c_un
{
   	long mtype;
     msg_data_t datamsg;
     msg_err_t errmsg;
	 msg_eot_t eotmsg;
};

s:
msgrcv(path);
union msg_s2c_un sbuf;
sbuf.errmsg.mtype = MSG_ERR;
sbuf.errmsg.errno = errno;
msgsnd(sbuf);

c:
msgsnd(path);

union msg_s2c_un rbuf;
msgrcv(&rbuf);
switch(rbuf.mtype)
{
	case MSG_DATA:

	case MSG_ERR:
		rbuf.errmsg.errno ;
}
*/




#endif



/*
s:
path_st path
msgrcv(msgid,&path,,MSG_PATH,);

 = fopen();

struct msg_s2c_st smsg;
smsg.mtype = MSG_ERR;
smsg.msg.errmsg.mtype = MSG_ERR;
msg.msg.errmsg.errno = errno;

msgsnd( smsg );


c:
msgsnd(  path  );

struct msg_s2c_st rmsg;
msgrcv(   &rmsg   );
swtich(rmsg.mtype)
{
	cast MSG_DATA:

	case MSG_ERR:
		rmsg.errmsg.errno

}


*/







