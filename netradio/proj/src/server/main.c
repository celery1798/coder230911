


int main()
{
	struct mlib_chnlist_st *listptr;
	int listptrsize;
	//搭建socket


	//守护进程

	//获得频道列表信息,用于组织节目单
	if(mlib_getchnlist(&listptr,&listptrsize) < 0)
	{
		syslog();
		exit(1);
	}	

	//创建thr_list
	thr_list_create(listptr,listptrsize);



	//创建1个或多个thr_channel   1:1
	for(i = 0 ; i < listptrsize; i++)
		thr_channel_create(listptr+i);



	while(1)
		pause();
	exit(0);

}






