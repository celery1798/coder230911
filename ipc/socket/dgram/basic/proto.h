#ifndef __PROTO_H__
#define __PROTO_H__

#define NAMESIZE	13

struct msg_st  
{
	uint8_t name[NAMESIZE];
	uint32_t math,chinese;
}__attribute__((packed));


#endif


