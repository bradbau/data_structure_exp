#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
//
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2


typedef int Status;
//
typedef int ElemType;

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

typedef struct  ListNode{
    ElemType data;
    struct ListNode *next;
}ListNode, *pListNode;

int e;
ListNode L;
pListNode pL= &L;

Status InitList(pListNode **pL){
    *pL = (ListNode *)malloc(sizeof(ListNode)*LIST_INIT_SIZE);
    (*pL)->data = 0;
    (*pL)->next = NULL;
    return OK;
}

Status DestroyList(pListNode **pL){
    if(!pL) return ERROR;
    (*pL)->data= 0;
    
}