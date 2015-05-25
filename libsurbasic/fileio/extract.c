#include "extract.h"
//初始化链表
int InitList(LinkList *L)
{ 
	*L=(LinkList)malloc(sizeof(LNode)); 
	if(!*L) 
		return 0;
	(*L)->next=NULL; 
	return 1;
}
//销毁
void DestroyList(LinkList *L)
{
	LinkList q;
	while(*L)
	{
		q=(*L)->next;
		free(*L);
		*L=q;
	}
}
//清空
void ClearList(LinkList L) 
{ 
	LinkList p,q;
	p=L->next; 
	while(p) 
	{
		q=p->next;
		if(p->data)
			free(p->data);
		free(p);
		p=q;
	}
	L->next=NULL; 
}
//判断表是否为空，即是否只有一个表头
int ListEmpty(LinkList L) 
{ 
	return L->next == NULL;
}
/* 初始条件：线性表L已存在。操作结果：返回L中数据元素个数 */
int ListLength(LinkList L)
{ 
	int i=0;
	LinkList p=L->next; 
	while(p) 
	{
		i++;
		p=p->next;
	}
	return i;
}
//查找
int FindElem(LinkList L,const char *str,LinkList *e) 
{
	int j=1;
	LinkList p=L->next; 
	while(p) 
	{
		if(strcmp(str,p->str) == 0)
		{
			*e = p;
			return j;
		}
		p=p->next;
		j++;
	}
	*e = NULL;
	return 0;
}
/* 在带头结点的单链线性表L中第i个位置之前插入元素e */
int ListUpdatedFilsInsert(LinkList L,const char* str,const void* data,int len) 
{ 
	LinkList p=L,s;
	LinkList pos;
		pos->len = len;
		memcpy(pos->data,data,len);
	}
		if(!p || str == NULL || strlen(str) <= 0) 
			return 0;

		while(p->next) //插到最后
			p=p->next;

		s = (LinkList)malloc(sizeof(LNode)); /* 生成新结点 */
		strcpy(s->str,str);
		s->data = (void*) malloc(len);
		s->len = len;
		memcpy(s->data,data,len);
		s->next=p->next;
		p->next=s;
	}
	return 1;
}
/* 在带头结点的单链线性表L中第i个位置之前插入元素e */
int ListInsert(LinkList L,const char* str, unz64_file_pos *pos) 
{ 
	LinkList p=L,s;
	if(!p || str == NULL || strlen(str) <= 0) 
		return 0;

	while(p->next) 
		p=p->next;

	s = (LinkList)malloc(sizeof(LNode)); 
	strcpy(s->str,str);
	if(pos){
		s->pos.pos_in_zip_directory = pos->pos_in_zip_directory;
		s->pos.num_of_file = pos->num_of_file;
	}
	s->next=p->next;
	p->next=s;
	return 1;
}
//添加
int ListAdd(LinkList L,const void* data,int len){
	LinkList p,s;
	p = L;
	if(!p) 
		return 0;
	while(p->next) 
		p=p->next;

	s = (LinkList)malloc(sizeof(struct LNode)); 
	s->data = (void*) malloc(len);
	s->len = len;
	memcpy(s->data,data,len);
	s->next=p->next;
	p->next=s;
	return 1;
}
//合并数据
int ListGenBuf(LinkList L,void** pStream){
	int len;
	unsigned int pos;
	void* buf;
	LinkList p=L->next;
	len = 0;
	while(p)
	{
		len += p->len;
		p=p->next;
	}
	buf = malloc(len);
	if(buf == NULL)
		return 0;
	p=L->next;
	pos = 0;
	while(p)
	{
		memcpy((void*)((char*)buf + pos),p->data,p->len);
		pos += p->len;
		free(p->data);
		p=p->next;
	}
	*pStream = buf;
	return len;
}

/* 初始条件：线性表L已存在。操作结果：依次对L的每个数据元素调用函数vi() */
void ListTraverse(LinkList L,void* pEx,void(*vi)(void*,LinkList))
{ 
	LinkList p=L->next;
	while(p)
	{
		vi(pEx,p);
		p=p->next;
	}
}
//初始化压缩包
int InitExtract(PExtract *pEx)
{
	*pEx=(PExtract)malloc(sizeof(Extract)); 
	if(!*pEx) 
		return 0;
	(*pEx)->nPageCount = 0;
	(*pEx)->hZip = NULL;
	(*pEx)->hUnZip = NULL;
	InitList(&(*pEx)->listPages);
	InitList(&(*pEx)->listUpdatedFils);
	InitList(&(*pEx)->listPos);
	memset((*pEx)->szZipName,0,FILENAME_LENGTH);
	memset((*pEx)->szUnZipName,0,FILENAME_LENGTH);
	memset((*pEx)->szImagePath,0,FILENAME_LENGTH);
	return 1;

}
//销毁
int DestroyExtract(PExtract *pEx)
{
	if(!*pEx) 
		return 0;
	ClearList((*pEx)->listPages); //清除page数据流
	DestroyList(&(*pEx)->listPages);
	ClearList((*pEx)->listUpdatedFils); //清除修改后的数据流
	DestroyList(&(*pEx)->listUpdatedFils);
	DestroyList(&(*pEx)->listPos);
	free(*pEx);
	*pEx = NULL;
	return 1;
}
//文件是否存在
int IsFileExistent(const char *pszFileName){
{
	return pEx->listUpdatedFils->next != NULL ? 1 : 0;
		zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
	zi.dosDate = 0;
	zi.internal_fa = 0;
	zi.external_fa = 0;

{ 
	// L为带头结点的单链表的头指针。
	char filename[32];
	memset(filename,0,32);
	sprintf(filename,"page_%d",index-1); //传过来页索引index是以1开始的，page文件夹以0开始，所以减一
	while(p ) 
	{
		if (strstr(p->str,filename) != NULL)
	}
	if(!p){ /* 元素不存在 */
		*e = NULL;
		return 0;
	}
	*e=p; /* 取第i个元素 */
	return 1;
}
			if(*pStream == NULL)
				return 0;
			memcpy(*pStream,pos->data,pos->len);
			return pos->len;
		}
			if(*pStream == NULL)
				return 0;
			memcpy(*pStream,e->data,e->len);
			return e->len;
//			return RetrieveStream(pEx,pStream,e->str);
{ 
	LinkList p=L->next; /* p指向第一个结点,有效数据从第一个next开始 */
	while(p) 
	{
		p->len = RetrieveStream(pEx,&p->data,p->str);
		p=p->next;
	}
	return 1;
}