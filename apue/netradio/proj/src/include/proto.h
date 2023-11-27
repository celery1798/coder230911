



#define MGROUP		"224.2.2.2"
#define SERVERPORT	"2089"

#define CHNMAX		100
#define LISTID		0

typedef uint8_t chnid_t;

#define MAX_MSG		(65536-20-8)
#define MAX_DATA	(MAX_MSG-sizeof(chnid_t))

struct msg_channel_st
{
	chnid_t chnid;		// [1,CHNMAX]
	uint8_t data[1];
}__attribute__((packed));

struct list_entry_st
{
	chnid_t chnid;
	uint8_t desc[1];
}__attribute__((packed));

struct msg_list_st
{
	chnid_t chnid; 		// LISTID
	struct list_entry_st  entry[1];
}__attribute__((packed));









