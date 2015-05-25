#ifndef EXTRACT_H
#define EXTRACT_H


#include <stdio.h>
#include "zip.h"
#include "unzip.h"


#define FILENAME_LENGTH 256
#define BUFFER_LENGTH	8192

//链表，共用，略有浪费内存，可以忽略
typedef struct LNode{
	void			*data;
	int				len;
	char			str[FILENAME_LENGTH];
	unz64_file_pos	pos;
	struct LNode	*next;
}LNode, *LinkList;

//typedef struct Extract Extract_;
typedef void (*fill_fontface_fun_ptr)(void *svgrender,char* fontfilename,void* stream,int streamlen);

int InitList(LinkList *L);
void DestroyList(LinkList *L);
void ClearList(LinkList L);
int ListEmpty(LinkList L);
int ListLength(LinkList L);
int FindElem(LinkList L,const char *str,LinkList *e);
//int GetElem(LinkList L,int index,LinkList *e);
int GetElemByPageNo(LinkList L,int index,LinkList *e);
int ListInsert(LinkList L,const char* str, unz64_file_pos *pos);
int ListUpdatedFilsInsert(LinkList L,const char* str,const void* data,int len);
int ListAdd(LinkList L,const void* data,int len);
int ListGenBuf(LinkList L,void** mem);
void ListTraverse(LinkList L,void* pEx,void(*vi)(void*,LinkList));
// 提取文件
typedef struct Extract{
	char		szZipName[FILENAME_LENGTH];
	char		szUnZipName[FILENAME_LENGTH];
	char		szImagePath[FILENAME_LENGTH];
	LinkList	listPages;
	LinkList	listUpdatedFils;
	LinkList	listPos;
	zipFile		hZip;    
	zipFile		hUnZip;    
	int			nPageCount;
	//file font face
	fill_fontface_fun_ptr fillptr;
	void		*render;
}Extract,*PExtract;

int IsFileExistent(const char *pszFileName);
int InUpdatedFiles(PExtract pEx,const char * pszFileName);
int RetrieveStream(PExtract pEx,void **pStream, const char *szFileName);
int InsertStream(PExtract pEx,const void *pStream,int len, const char* szFileName);
void CpyBufferFromZip(void* pEx,LinkList e);
void CpyBufferFromList(void* pEx,LinkList e);
void GenImage(void* pEx,LinkList e);
void GenImageAndFont(void* pEx,LinkList e);
void removeImage(void* pEx,LinkList e);
int Close(PExtract pEx);
int readAllPageStream(PExtract pEx,LinkList L);
int InitExtract(PExtract *pEx);
int DestroyExtract(PExtract *pEx);
int InitZipFile(PExtract pEx,const char *pUnZipName,const char *pZipName,const char *pImagePath);
int GenerateImageFile(PExtract pEx); 
int GenerateResFile(PExtract pEx,void *render,fill_fontface_fun_ptr fillptr);
int RemoveImageFile(PExtract pEx);
int getPageCount(PExtract pEx);
int getPageStream(PExtract pEx,void **pStream, int index); 
int UpdatePageStream(PExtract pEx,const void *pStream,int len,int index);
int InsertImageStream(PExtract pEx,const void *pStream,int len,char * name);
int SaveZipFile(PExtract pEx);
int ZipFileIsChanged(PExtract pEx);
int getPageSvgStream(void **pStream,void **pFontmapStream,int *fontmapslen,int count); //test


#endif