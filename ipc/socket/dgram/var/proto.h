#ifndef __PROTO_H__
#define __PROTO_H__

#define RCVPORT		"1988"

#define NAMEMAX		(512-8-8)	

struct msg_st  
{
	uint32_t math,chinese;
	uint8_t name[1];
}__attribute__((packed));


#endif


