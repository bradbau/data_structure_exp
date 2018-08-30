#include<stdio.h>
#include<stdlib.h>

//
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

//
typedef int Status;
//
typedef int ElemType;

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

typedef struct {
    ElemType * elem;
    int  length;
    int listsize;

} Sqlist;

Status InitList(Sqlist **L){ 
    *L=(Sqlist *)malloc(sizeof(Sqlist));  
    (*L)->elem=(ElemType *)malloc(LIST_INIT_SIZE*sizeof(ElemType));
    if (! (*L)->elem) exit(OVERFLOW); 
    (*L)->length=0;
    (*L)->listsize=LIST_INIT_SIZE; 
    return OK;   
}


Status DestroyList(Sqlist *L){
    free(L->elem);
    L->length=0;
    L->listsize=0;
    return OK;
}

Status ClearList(Sqlist *L){
    L->length=0;
    return OK;
}

Status ListEmpty(Sqlist *L){
    if(L->length==0) return TRUE;
    else return FALSE;
}

int ListLength(Sqlist *L){
    return L->length;
}
 
Status GetElem(Sqlist *L,ElemType i,ElemType *e){
    if(i>=1||i<=L->length)
       *e=L->elem[i-1];
    else return ERROR;
    return OK; 
}

Status LocateElem(Sqlist *L, ElemType e, Status (*compare())(ElemType, ElemType)){
    int i;
    for(i=1; i<L->length; i++){
        if((*compare)(L->elem[i-1], e))
        return i;
    }
    return 0;
}

Status PriorElem(Sqlist *L, ElemType cur_e, ElemType *pre_e){
    int i;
    for(i=2; i<L->length;++i){
        if(cur_e==L->elem[i-1]){
          *pre_e = L->elem[i-2];
          return OK;
        }
    }
    return FALSE;
}

Status NextElem(Sqlist *L, ElemType cur_e, ElemType *pre_e){
    int i;
    for(i=1;i<=L->length-2; ++i){
        if(cur_e==L->elem[i-1]){
            *pre_e=L->elem[i];
            return OK;
        }
    }
    return FALSE;
}

Status ListInsert(Sqlist *L, ElemType i, ElemType e){
    if(L&&i>=1&&i<=L->length+1){
        if(!(L->length<L->listsize)){
            ElemType * pbase;
            pbase=(Status *)realloc(L->elem, (L->listsize+ LISTINCREMENT) * sizeof(ElemType));
            if(!pbase)
                exit(OVERFLOW);
            L->elem=pbase;
            L->listsize+=LISTINCREMENT;

        }
        L->length++;
        int j;
        for(j=L->length; j>i; j--){
            L->elem[j-1]=L->elem[j-2];
        }
        L->elem[i-1]=e;
        return OK;
    }
    return FALSE;
}

Status ListDelate(Sqlist *L, ElemType i, ElemType *e){
    int j;
    if(L&&i>=1&&i<=L->length){
        *e=L->elem[i-1];
        for(j=i; j<=L->length; j++)
            L->elem[j-1]=L->elem[j];
        L->length--;
        return OK;
    }
    return FALSE;
}

Status ListTraverse(Sqlist L, Status(* visit)(ElemType a)){
    
}

int main()
{
    Sqlist *L;
    InitList(&L);

    printf("%d", L->listsize);
    free(L);
    return 0;

}
