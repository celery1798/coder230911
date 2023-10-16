#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH		"/etc/hosts"
#define KEYPROJ		'a'	
#define NAMESIZE	32

#define MYMSGTYPE	11

struct msg_st  
{
	long mtype;
	char name[NAMESIZE];
	int math,chinese;
};


#endif


