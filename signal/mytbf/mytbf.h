#ifndef _MYTBF_H_
#define _MYTBF_H_

#define MYTBF_MAX		1024

typedef void mytbf_t; 

mytbf_t *mytbf_init(int cps,int burst);


/*
mytbf_fetchtoken:xxxx
@p-
@size-
return: success > 0 , failed < 0,-errno
*/
int mytbf_fetchtoken(mytbf_t *p,int size);

int mytbf_returntoken(mytbf_t *,int);

int mytbf_destroy(mytbf_t *);

#endif



