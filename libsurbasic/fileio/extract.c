#include "extract.h"#include <stdio.h>#include "zip.h"#include "unzip.h"#ifdef CURRENTOS_IS_IOS#import "stdlib.h"#else#include "malloc.h"#endif#include <string.h>#include <memory.h>
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
	LinkList pos;	if(FindElem(L,str,&pos)) //如果已经存在，就替换	{		if(pos->data)			free(pos->data);		pos->data = (void*) malloc(len);
		pos->len = len;
		memcpy(pos->data,data,len);
	}	else{
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
int IsFileExistent(const char *pszFileName){	FILE *pFile;	pFile = fopen(pszFileName,"rb");	if(pFile)	{		fclose(pFile);		return 1;	}	return 0;}//文件是否更新int InUpdatedFiles(PExtract pEx,const char * pszFileName){	LinkList pos;	return FindElem(pEx->listUpdatedFils,pszFileName,&pos);}//文件是否修改int ZipFileIsChanged(PExtract pEx)
{
	return pEx->listUpdatedFils->next != NULL ? 1 : 0;}//提取流int RetrieveStream(PExtract pEx,void **pStream, const char *szFileName){	LinkList bufList;	LinkList pos;	int nReadSize = 0;	void *pBuffer = NULL;	if(strlen(pEx->szUnZipName) == 0)		return 0;	if(pEx->hUnZip == NULL)		return 0;	if(!FindElem(pEx->listPos,szFileName,&pos))		return 0;	if(unzGoToFilePos64(pEx->hUnZip, &(pos->pos)) != UNZ_OK)		return 0;	unzOpenCurrentFile(pEx->hUnZip);	pBuffer = (void*)malloc(BUFFER_LENGTH);	if (pBuffer==NULL)		return 0;	InitList(&bufList);	do	{		nReadSize = unzReadCurrentFile(pEx->hUnZip,pBuffer,BUFFER_LENGTH);		if(nReadSize < 0)			return 0;		if(nReadSize > 0)			ListAdd(bufList,pBuffer,nReadSize);	}	while (nReadSize> 0 );	nReadSize = ListGenBuf(bufList,pStream);	unzCloseCurrentFile(pEx->hUnZip);	free(pBuffer);	DestroyList(&bufList);	return nReadSize;}//插入流int InsertStream(PExtract pEx,const void *pStream,int len, const char* szFileName){	zip_fileinfo zi;	int nDeflateLevel = Z_DEFLATED;	int nReadSize = 0;	int nCount = 0;		if(!pStream || strlen(pEx->szZipName) ==0)		return 0;	if(pEx->hZip == NULL)	{ 		pEx->hZip = zipOpen64(pEx->szZipName, APPEND_STATUS_CREATE);		if(pEx->hZip == NULL)			return 0;	}	zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
		zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
	zi.dosDate = 0;
	zi.internal_fa = 0;
	zi.external_fa = 0;
	if(zipOpenNewFileInZip3_64(pEx->hZip, szFileName, &zi, NULL, 0, NULL, 0, NULL,Z_DEFLATED, nDeflateLevel,0,  -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,NULL, 0, 0) != ZIP_OK)		return 0;	do	{ 		nReadSize = (len - nCount < BUFFER_LENGTH) ? len - nCount : BUFFER_LENGTH;		if (nReadSize > 0)		{			if(zipWriteInFileInZip (pEx->hZip,(void*)((char*)pStream + nCount),nReadSize) < 0)				return 0;			nCount += nReadSize;		}	} while (nCount < len);	zipCloseFileInZip(pEx->hZip);	return 1;}//初始化zip文件int InitZipFile(PExtract pEx,const char *pUnZipName,const char *pZipName,const char *pImagePath){		unz_global_info64 gi;	unz64_file_pos unzFilePos;	char filename_inzip[FILENAME_LENGTH];	unz_file_info64 file_info;	int i;	ListEmpty(pEx->listPos);	ListEmpty(pEx->listPages);	pEx->nPageCount = 0;	ListEmpty(pEx->listUpdatedFils);	if(!IsFileExistent(pUnZipName))			return 0;	strcpy(pEx->szUnZipName, pUnZipName);	if(!strlen(pZipName))		return 0;	strcpy(pEx->szZipName,pZipName);	if(!strlen(pImagePath))		return 0;	strcpy(pEx->szImagePath,pImagePath);	if(pEx->hUnZip == NULL)	{ 		pEx->hUnZip = unzOpen64(pEx->szUnZipName);		if(pEx->hUnZip == NULL)			return 0;	}	if(unzGetGlobalInfo64(pEx->hUnZip,&gi) == UNZ_OK)	{		for (i=0;i<gi.number_entry;i++)		{			if(unzGetCurrentFileInfo64(pEx->hUnZip,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0)== UNZ_OK)			{				if(unzGetFilePos64(pEx->hUnZip, &unzFilePos) == UNZ_OK)				{					ListInsert(pEx->listPos,filename_inzip, &unzFilePos);					if (strstr(filename_inzip,"content.svg") != NULL){						ListInsert(pEx->listPages,filename_inzip,NULL); 						pEx->nPageCount++;					}				}			}			if ((i+1)<gi.number_entry)			{				if(unzGoToNextFile(pEx->hUnZip) != UNZ_OK)					break;			}		}	}	readAllPageStream(pEx,pEx->listPages);	return 1;}//关闭int Close(PExtract pEx){	if(pEx->hUnZip)	{		unzClose(pEx->hUnZip);		pEx->hUnZip = NULL;	}	if(pEx->hZip)	{		zipClose(pEx->hZip, NULL);		pEx->hZip = NULL;	}	strcpy(pEx->szUnZipName,pEx->szZipName);	return 1;}//流复制void CpyBufferFromZip(void* pEx,LinkList e){	void *pStream;	int len;	if(!InUpdatedFiles((PExtract)pEx,e->str)) 	{		len = RetrieveStream((PExtract)pEx,&pStream, e->str);		if( len > 0)		{			InsertStream((PExtract)pEx,pStream,len,e->str);			free(pStream);		}	}}//链表流复制void CpyBufferFromList(void* pEx,LinkList e){	if( e->len > 0)	{		InsertStream((PExtract)pEx,e->data,e->len,e->str);	}}//保存int SaveZipFile(PExtract pEx){	ListTraverse(pEx->listPos,(void *)pEx,CpyBufferFromZip); //拷贝原zip里的文件	ListTraverse(pEx->listUpdatedFils,(void *)pEx,CpyBufferFromList); //拷贝修改后的，以及新增的文件	Close(pEx);	return 1;}//提取位图void GenImage(void* pEx,LinkList e){	void *pStream;	int len;	char* pos;	int offset;	char filename[FILENAME_LENGTH];	memset(filename,0,FILENAME_LENGTH);	pos = strstr(e->str,"/images/");	offset = 8;	//if (pos == NULL){	//	pos = strstr(e->str,"/fonts/");	//	offset = 7;	//}	if (pos != NULL){		len = RetrieveStream((PExtract)pEx,&pStream, e->str);		if( len > 0)		{			FILE *pFile;			strcpy(filename,((PExtract)pEx)->szImagePath/*,strlen(((PExtract)pEx)->szImagePath)*/);			strcpy((char*)filename+strlen(((PExtract)pEx)->szImagePath),(char*)(pos)+offset/*,strlen(pos)-8*/);			pFile = fopen(filename, "wb+");			if (pFile)			{				fwrite(pStream,  1, len, pFile);				fclose(pFile);			}			free(pStream);		}	}}//提取位图和字体资源void GenImageAndFont(void* pEx,LinkList e){	void *pStream;	int len;	char* pos;	char filename[FILENAME_LENGTH];	memset(filename,0,FILENAME_LENGTH);	pos = strstr(e->str,"font_"); //字体文件，则回调	if (pos != NULL){		len = RetrieveStream((PExtract)pEx,&pStream, e->str);		if( len > 0)			((PExtract)pEx)->fillptr(((PExtract)pEx)->render,e->str,pStream,len);	}	else //图片文件,则保存	{		len = RetrieveStream((PExtract)pEx,&pStream, e->str);		if( len > 0)		{			FILE *pFile;			strcpy(filename,((PExtract)pEx)->szImagePath);			strcpy((char*)filename+strlen(((PExtract)pEx)->szImagePath),e->str);			pFile = fopen(filename, "wb+");			if (pFile)			{				fwrite(pStream,  1, len, pFile);				fclose(pFile);			}			free(pStream);		}	}}//生成资源文件int GenerateResFile(PExtract pEx,void *render,fill_fontface_fun_ptr fillptr){	pEx->fillptr = fillptr;	pEx->render = render;	ListTraverse(pEx->listPos,(void *)pEx,GenImageAndFont);	return 1;}//生成位图int GenerateImageFile(PExtract pEx){	ListTraverse(pEx->listPos,(void *)pEx,GenImage);	return 1;}//移除位图void removeImage(void* pEx,LinkList e){	char* pos;	char filename[FILENAME_LENGTH];	memset(filename,0,FILENAME_LENGTH);	pos = strstr(e->str,"/images/");	if (pos != NULL){		strcpy(filename,((PExtract)pEx)->szImagePath/*,strlen(((PExtract)pEx)->szImagePath)*/);		strcpy((char*)filename+strlen(((PExtract)pEx)->szImagePath),(char*)(pos)+8/*,strlen(pos)-8*/);		if(IsFileExistent(filename))			remove(filename);	}}//移除位图文件int RemoveImageFile(PExtract pEx){	ListTraverse(pEx->listPos,(void *)pEx,removeImage);	return 1;}//查找int GetElemByPageNo(LinkList L,int index,LinkList *e) 
{ 
	// L为带头结点的单链表的头指针。
	char filename[32];	LinkList p=L->next; /* p指向第一个结点 */
	memset(filename,0,32);
	sprintf(filename,"page_%d",index-1); //传过来页索引index是以1开始的，page文件夹以0开始，所以减一
	while(p ) 
	{
		if (strstr(p->str,filename) != NULL)			break;		p=p->next;
	}
	if(!p){ /* 元素不存在 */
		*e = NULL;
		return 0;
	}
	*e=p; /* 取第i个元素 */
	return 1;
}//提取svg文件流int getPageStream(PExtract pEx,void **pStream, int index){	LinkList e;	int len = pEx->nPageCount;//ListLength(pEx->listPages);	if(len <= 0 || index > len || index < 1)		return 0;//	if(GetElem(pEx->listPages,index,&e)){	if(GetElemByPageNo(pEx->listPages,index,&e)){		LinkList pos;		if(FindElem(pEx->listUpdatedFils,e->str,&pos)) //如果已经更新过数据，就直接从更新后的数据里复制		{			*pStream = malloc(pos->len);
			if(*pStream == NULL)
				return 0;
			memcpy(*pStream,pos->data,pos->len);
			return pos->len;
		}		else{			*pStream = malloc(e->len);
			if(*pStream == NULL)
				return 0;
			memcpy(*pStream,e->data,e->len);
			return e->len;
//			return RetrieveStream(pEx,pStream,e->str);		}	}	return 0;}//读取所有svg文件流int readAllPageStream(PExtract pEx,LinkList L)
{ 
	LinkList p=L->next; /* p指向第一个结点,有效数据从第一个next开始 */
	while(p) 
	{
		p->len = RetrieveStream(pEx,&p->data,p->str);
		p=p->next;
	}
	return 1;
}//svg文件总数int getPageCount(PExtract pEx){	return pEx->nPageCount;//ListLength(pEx->listPages);}//更新页面流文件int UpdatePageStream(PExtract pEx,const void *pStream,int len,int index){	LinkList page,pos;	if(ListLength(pEx->listPos) <= 0)		return 0;//	if(ListLength(pEx->listPages) <= 0 || index > ListLength(pEx->listPages))//		return 0;	if(!GetElemByPageNo(pEx->listPages,index,&page))		return 0;	if(!FindElem(pEx->listPos,page->str,&pos))		return 0;//	InsertStream(pEx,pStream,len, page->str);//	ListInsert(pEx->listUpdatedFils,page->str,NULL);	ListUpdatedFilsInsert(pEx->listUpdatedFils,page->str,pStream,len); 	return 1;}//插入图片流int InsertImageStream(PExtract pEx,const void *pStream,int len,char * name){	return ListUpdatedFilsInsert(pEx->listUpdatedFils,name,pStream,len); }// testint getPageSvgStream(void **pStream,void **pFontmapStream,int *fontmapslen,int count){	int pagecount;	char* str;	//void *pStream;	int len =0;	PExtract pEx; 	if(InitExtract(&pEx))	{		InitZipFile(pEx,"D:\\h.sep","D:\\hh.sep","D:\\res\\");		GenerateImageFile(pEx);		pagecount = getPageCount(pEx);		if(pagecount > 0){			*fontmapslen = RetrieveStream(pEx,pFontmapStream,"docbase/root/doc_0/properties.xml");			str = (char*)(*pFontmapStream);			len  = getPageStream(pEx,pStream,count <= pagecount ? count : pagecount); //起始页索引为1，不是0			if (len > 0){				UpdatePageStream(pEx,*pStream,len,1);					SaveZipFile(pEx); //save file to disk						//free(pStream);			}		}	//	RemoveImageFile(pEx);		DestroyExtract(&pEx);	}	return len;}