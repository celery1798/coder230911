#ifndef   
#define xxx

struct mlib_chnlist_st
{
	chnid_t chnid;
    char *desc;
};



int mlib_getchnlist( struct mlib_chnlist_st ** , int * );
int mlib_freechnlist(struct mlib_chnlist_st *);

ssize_t mlib_chnread(chnid, void *buf, int size);

...


#endif



