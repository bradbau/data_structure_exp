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


Status DestroyList(Sqlist **L){
    free((*L));
    *L=NULL;
    return OK;
}

Status ClearList(Sqlist **L){
    (*L)->length=0;
    (*L)->listsize=0;
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

int LocateElem(Sqlist *L, ElemType e, Status (*compare())(ElemType, ElemType)){
    int i;
    for(i=1; i<L->length; i++){
        if(compare(L->elem[i-1], e))
        return i;
    }
    return 0;
}

Status PriorElem(Sqlist *L, ElemType cur_e, ElemType *pri_e){
    int i;
    for(i=2; i<= L->length; ++i){
        if(cur_e==L->elem[i-1]){
          *pri_e = L->elem[i-2];
          return OK;
        }
    }
    return FALSE;
}

Status NextElem(Sqlist *L, ElemType cur_e, ElemType *next_e){
    int i;
    for(i=1; i<L->length; ++i){
        if(cur_e==L->elem[i-1]){
            *next_e=L->elem[i];
            return OK;
        }
    }
    return FALSE;
}

Status ListInsert(Sqlist **L, ElemType i, ElemType e){
    if((*L)&&i>=1&&i<=(*L)->length+1){
        if(!((*L)->length<(*L)->listsize)){
            ElemType * pbase;
            pbase=(Status *)realloc((*L)->elem, ((*L)->listsize+ LISTINCREMENT) * sizeof(ElemType));
            if(!pbase)
                exit(OVERFLOW);
            (*L)->elem=pbase;
            (*L)->listsize+=LISTINCREMENT;

        }
        (*L)->length++;
        int j;
        for(j=(*L)->length; j>i; j--){
            (*L)->elem[j-1]=(*L)->elem[j-2];
        }
        (*L)->elem[i-1]=e;
        return OK;
    }
    return FALSE;
}

Status ListDelate(Sqlist **L, ElemType i, ElemType *e){
    int j;
    if(*L&&i>=1&&i<=(*L)->length){
        *e=(*L)->elem[i-1];
        for(j=i; j<=(*L)->length; j++)
            (*L)->elem[j-1]=(*L)->elem[j];
        (*L)->length--;
        return OK;
    }
    return FALSE;
}

Status ListTraverse(Sqlist *L, Status(* visit)(ElemType a)){
    
}

Status compare(ElemType x, ElemType y){
    return x==y;
}
void PrintList(Sqlist *L){
    int len=L->length;
    int i;
    printf("\n********LIST ELEMENT*******\n");
    for(i=0; i<len; i++)
        printf(" %d", L->elem[i]);
    printf("\n***************************\n");
    
}
int main()
{
    Sqlist *L;
   // InitList(&L);
        int alter=1;
        int stat;
        int i, len;
        ElemType e, cur_e, pri_e, next_e;
    while(alter){
        
        printf("\n");
        printf("Alternative for linear list:\n");
        printf("1.InitList      2.DectroyList   3.CLearList\n");
        printf("4.ListEmpty     5.ListLength    6.GetElem\n");
        printf("7.LocateElem    8.PriorElem     9.NextElem\n");
        printf("10.ListInert    11.ListDelet    12.ListTraverse\n");
        printf("0.Exit \nplease entry the code for action\n");
        scanf("%d", &alter);
        switch(alter){
            case 1:
                if(InitList(&L)==OK)   printf("initialize sequential linear list successfully\n");
                else printf("Fail\n");
                break;
            case 2:
                if(DestroyList(&L)==OK)   printf("Destroy list successfully\n");
                else printf("Fail\n");
                break;
            case 3:
                if(ClearList(&L)==OK)     printf("Clear list successfully\n");
                else printf("Fail\n");
                break;
            case 4:
                if(ListEmpty(L)==OK)    printf("List is empty\n");
                else printf("The list is not empty\n");
                break;
            case 5:
               
                len=ListLength(L);
                printf("The length of the list is %d\n.", len);
                break;
            case 6:
               
                printf("entry the serial number:\n");
                scanf("%d",&i);
                if(GetElem(L, i, &e)==OK)  printf("successfully extract the element number %d: %d\n", i, e);//没法用elemtype来决定这里的输出类型？
                else printf("fail\n");
                break;
            case 7:
                printf("entry the number to be located:\n");
                scanf("%d", &e);
                if((i = LocateElem(L, e, compare)))  printf("the serial number of the element %d is %d\n", e, i );
                else printf("fail\n");
                break;
            case 8:
               
                printf("entry the current element\n");
                scanf("%d",&cur_e);
                if(PriorElem(L, cur_e, &pri_e)==OK)   printf("the elemment before the first %d is %d", cur_e, pri_e);
                else printf("fail\n");
                break;
            case 9:
               
                printf("entry the current element\n");
                scanf("%d",&cur_e);
                if(NextElem(L, cur_e, &next_e)==OK)   printf("the elemment after the first %d is %d", cur_e, next_e);
                else printf("fail\n");
                break;
            case 10:
                printf("entry the number to be insert and the sequense of it\n");
                scanf("%d %d",&e, &i);
               stat=ListInsert(&L, i, e);
                if(stat==OK) printf("success\n");
                else if(stat==FALSE) printf("fail\n");
                break;
            case 11:
                printf("entry the serial number of the element to be deleted:\n");
                scanf("%d", &i);
                if(ListDelate(&L, i, &e)==OK)  printf("number %d element %d is deleted\n", i, e);
                else printf("fail\n");
                break;
            case 12:
                break;
            case 0:
                return 0;
        }
        //  print the list
        if(L)
          PrintList(L);
        else printf("There is no list.\n");
    }
   // free(L);
    return 0;

}
