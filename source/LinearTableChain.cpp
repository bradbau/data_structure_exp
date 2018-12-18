/************
 * 一开始就做成可以兼容多表读写的，设计多表的数据格式
 * 用cJSON来存储数据项，那么怎么解决数据封装的问题：要把cJSON暴露在外面吗？
 * 我觉得应该不要，所以我的所有的cJSON都应该出现在函数中，并用函数获取cJSON的内容
 * 但是我仍然在所有场景用cJSON存储信息，并定义elemtype为cJSON
 * 
 * 
 * ************/
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include"cJSON.h"
/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2



typedef int status;
typedef cJSON* ElemType; //数据元素类型定义

/*-------page 22 on textbook -------*/
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

typedef struct CNode
{
	ElemType elem;
	struct CNode *pnext;
} CNode;
//这里注意定义的规范
typedef struct
{
	CNode *head;
	int length;
	char * listname;
} CList;
typedef struct
{
	CList **contents, *curList;
	int gnumber;
	int gsize;
} CListG;

//
status InitiaList(CList **L);
status DestroyList(CList *L);
status ClearList(CList *L);
status ListEmpty(CList L);
status ListLength(CList L);
status GetElem(CList L, int i, ElemType &e);
int LocateElembyName(CList L,  char *inname); //简化过
status PriorElem(CList L, char *inname, ElemType &pre_e);
status NextElem(CList L, char *inname, ElemType &next_e);
status ListInsert(CList *L, int i, ElemType e);
status ListDelate(CList *L, int i, ElemType &e);
status ListTraverse(CList L); //简化过

//自定义函数
status InitiaListG(CListG &ListGroup, CList **L, bool state);//内部调用传0

status InfoRead(char *filename, CListG & Lg);
status InfoWrite(char *filename, CListG Lg);

int LocateElemByRef(CListG Lg, CList *L);
status Elemdelate(CListG &Lg, int seq);
status printInfo(ElemType elem);
status dpgroup(CListG Lg);
status ShiftCur(CList **L, CListG &Lg);
status reclctNode(CNode *node);
status Enlargebuffer(char * buffer, int size, int delta);
status EnlargeGroup(CListG &ListGroup, int delta);
const char *ElemName(cJSON * elem);
const char *ElemSex(cJSON * elem);
const char *ElemTele(cJSON * elem);
const char *ElemEmail(cJSON * elem);
bool GeneJSON(ElemType & e, char *divname, char *divsex, char *divtele, char *divemail);
const char * Genelname(int num);//num是当前链表组中可以保存文件的链表的序号



/***************************/
int main(int argc, char *argv[])
{
	CList *L = NULL;
	CListG Lg;
	Lg.contents = NULL;
	Lg.gnumber = Lg.gsize = 0;
	int op = 1;
	bool filemode = 0;

	while (op)
	{
		if (Lg.gnumber > 1)
		{
			system("cls");
			printf("\n\n");
			dpgroup(Lg); //展示所有的链表内容
			printf("   当前存在多个链表，请选择要进行的操作：\n");
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			printf("		1.选择链表		2.删除链表\n");
			printf("		3.添加链表		4.交换链表\n");
			printf("		5.合并链表		6.保存至文件\n");
			int alter = 1;

			printf("请输入操作代号:");
			scanf("%d", &alter);
			switch (alter)
			{
			case 1:
			{
				int seq;

				printf("请输入选择的表的序号：\n");
				scanf("%d", &seq);
				//这里需要让
				Lg.curList = Lg.contents[seq - 1];

				getchar();
				getchar();
				break;
			}
			case 2:
			{
				int seq;
				printf("请输入要删除的链表序号：\n");
				scanf("%d", &seq);
				DestroyList(Lg.contents[seq - 1]);
				if(Elemdelate(Lg, seq)==OK)
					printf("已成功删除第%d个表\n", seq);
				L = Lg.curList;
				getchar();
				getchar();
				break;
			}
			case 3:
			{
				printf("功能待完成\n");
				getchar();
				getchar();
				break;
			}
			case 4:
			{
				printf("功能待完成\n");
				getchar();
				getchar();
				break;
			}
			case 5:
			{
				printf("功能待完成\n");
				getchar();
				getchar();
				break;
			}
			case 6:
			{
				if (!(Lg.gnumber))
				{
					printf("当前不存在链式表\n");
					getchar();
					getchar();
					break;
				}
				char *filename = NULL;
				filename = (char *)malloc(80 * sizeof(char));
				printf("请输入要保存的文件名：");
				scanf("%s", filename);
				Lg.curList = Lg.contents[0];
				if (InfoWrite(filename, Lg))
					printf("文件保存成功\n");
				else
					printf("文件保存失败\n");
				getchar();
				getchar();
				break;
			}

			} //end of first switch
		}	 // end of if(gnumber>1)

		system("cls");
		printf("\n\n");
		if (Lg.gnumber)
		{
			printf("当前选择的表名为：%s\n", Lg.curList->listname);
			L = Lg.curList;
		}
		printf("      Menu for Linear Table On Chain Structure \n");
		printf("-------------------------------------------------\n");
		printf("		1. InitiaList       7. LocateElembyName\n");
		printf("		2. DestroyList      8. PriorElem\n");
		printf("		3. ClearList        9. NextElem \n");
		printf("		4. ListEmpty        10. ListInsert\n");
		printf("		5. ListLength       11. ListDelate\n");
		printf("		6. GetElem          12. ListTraverse\n");
		printf("		13. ReadData        14. Savingdata\n");
		printf("		15. AddList         16. DisplayAll\n");
		printf("		0. Exit\n");
		printf("-------------------------------------------------\n");
		printf("    please choose your opreation[0~15]:");
		scanf("%d", &op);

		switch (op)
		{
		case 1:
			if (L)
			{
				printf("已经存在一个链式表\n");
				getchar();
				getchar();
				break;
			}
			if (InitiaList(&L) == OK)
			{
				InitiaListG(Lg, &L, 0); //内部调用传0
				Lg.gnumber = 1;
				L->listname = (char *)malloc(20 * sizeof(char));
				strcpy(L->listname, Genelname(1));
				printf("链式表创建成功！\n");
			}
			else
				printf("操作失败！\n");
			getchar();
			getchar();
			break;
		case 2:
			if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			if (DestroyList(L) == OK)
			{

				Elemdelate(Lg, LocateElemByRef(Lg,L));
				L = Lg.curList;
				printf("链式表成功销毁！\n");

			}
			else
				printf("操作失败！\n");
			getchar();
			getchar();
			break;
		case 3:
			if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			if (ClearList(L) == OK)
				printf("线性表已置空\n");
			else
				printf("操作失败\n");
			getchar();
			getchar();
			break;
		case 4:
		if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			ListEmpty(*L);
			getchar();
			getchar();
			break;
		case 5:
			if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			else
				ListLength(*L);
			getchar();
			getchar();
			break;
		case 6:{
			if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			int seq;
			ElemType e;
			printf("请输入要得到的元素序号：");
			scanf("%d", &seq);
			if (GetElem(*L, seq, e)){
				printf("查询得到第%d个成员信息如下:\n", seq);
				printInfo(e);
				}
			else
				printf("查询失败\n");
			getchar();
			getchar();
			break;
		}
		case 7:
		{
			if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			char  str[20];
			int seq;
			printf("请输入要定位的个体姓名：");
			scanf("%s", str);
			if (!LocateElembyName(*L, str))
				printf("操作失败\n");
			getchar();
			getchar();
			break;
		}
		case 8:
			if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			ElemType pre_e;
			char divname[20];
			printf("请输入一个成员姓名，程序将返回它的前一个成员：");
			scanf("%19s", divname);
			if (!PriorElem(*L, divname, pre_e))
				printf("查找失败");
			getchar();
			getchar();
			break;
		case 9:{
			if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			ElemType next_e;
			char divname[20];
			printf("请输入成员名，程序将返回它的下一个成员：");
			scanf("%19s", divname);
			if (NextElem(*L, divname, next_e))
			{
				//printf("该成员的下一个成员信息如下：");
				//printInfo(next_e);
			}
			getchar();
			getchar();
			break;
		}
		case 10:
			if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			printf("请输入待插入的成员信息以及插入的位置（用空格分割）：（姓名 性别 电话 邮箱 序号）\n");
			//scaninfo
			ElemType e;
			e = cJSON_CreateObject();
			int seq;
			{
				//如果不能这样初始化，那就只能在函数内新建字符串常量了
				 char divname[20], divsex[10], divtele[20], divemail[40];
				scanf("%19s %9s %19s %39s %d", divname,divsex, divtele, divemail, &seq);
				GeneJSON(e, divname,divsex, divtele, divemail);
			}
			if (ListInsert(L, seq, e) == OK)
			{
				printf("元素已成功插入，新表内容如下\n");
				ListTraverse(*L);
			}
			else
				printf("插入失败\n");
			getchar();
			getchar();
			break;
		case 11:
		{
			if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			ListTraverse(*L);
			printf("请输入要删除的元素序号:");
			int seq;
			ElemType e;
			scanf("%d", &seq);
			if (ListDelate(L, seq, e) == OK)
			{
				printf("第%d个元素删除成功，元素内容是:\n", seq);
				printInfo(e);
				printf("新表内容如下：\n");
				if(ListTraverse(*L)==0) printf("表元素为空\n");
			}
			else
				printf("元素删除失败");
			getchar();
			getchar();
			break;
		}
		case 12:
			if (!L)
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			if (!ListTraverse(*L))
				printf("线性表是空表！\n");
			getchar();
			getchar();
			break;
		case 13:
			char file13[30];
			int tablenum;
			printf("请输入要读入的文件名：");
			scanf("%s", &file13);
			//如果存在表，先销毁  //另一个版本，可以保存多个链表
			//要保证Lg已经初始化，并且curlist指向第一个空Clist成员
			if (!Lg.gnumber)
				InitiaListG(Lg, &L, 1);
			else
				ShiftCur(&L, Lg);
			/*
			Lg.curList = Lg.contents[Lg.gnumber];
			Lg.curList = (CList *)malloc(sizeof(CList));
			以上是冗余代码，因为以上检查是为了防止程序在未初始状态，而initiallist后可以使程序直接到达基准状态
			*/
			if (InfoRead(file13, Lg))
				printf("\n成功读入文件内容到表中,现共有%d个表\n", Lg.gnumber);
			L = Lg.curList;
			//遍历tablenum个表
			getchar();
			getchar();
			break;
		case 14:
		{
			if (!(Lg.gnumber))
			{
				printf("当前不存在链式表\n");
				getchar();
				getchar();
				break;
			}
			char *filename = NULL;
			filename = (char *)malloc(80 * sizeof(char));
			printf("请输入要保存的文件名：");
			scanf("%s", filename);
			Lg.curList = Lg.contents[0];
			if (InfoWrite(filename, Lg))
				printf("文件保存成功\n");
			else
				printf("文件保存失败\n");
			getchar();
			getchar();
			break;
		}
		case 15:
		{
			if (!Lg.gnumber)
				InitiaListG(Lg, &L, 1); //保证基准状态
			else
				ShiftCur(&L, Lg); //此操作完成后仍是基准状态
			strcpy(Lg.curList->listname, Genelname(Lg.gnumber));
			printf("当前链表组中有%d个表，当前操作第%d个\n", Lg.gnumber, Lg.gnumber);

			getchar();
			getchar();
			break;
		}
		case 16:
		{
			dpgroup(Lg);
			getchar();
			getchar();
			break;
		}
		case 0:
			break;
		} //end of switch

	} //end of while
} //end of main

//definition of functions

status InitiaList(CList **L)
{

	{
		(*L) = (CList *)malloc(sizeof(CList));
		(*L)->head = (CNode *)malloc(sizeof(CNode));
		(*L)->listname = (char *)malloc(20 * sizeof(char));
		(*L)->length = 0;
		(*L)->head->pnext = NULL;
		return OK;
	}

} //end

status DestroyList(CList *L)
{

	reclctNode(L->head);
	free(L->listname);
	L = NULL;
	return OK;
}
status reclctNode(CNode *node)
{

	if (node->pnext != NULL)

		reclctNode(node->pnext);
	else
	{
		free(node);
		return OK;
	}
	return ERROR;
}

status ClearList(CList *L)
{
	{
		if (L->head->pnext == NULL)
			return OK;
		else{
			reclctNode(L->head);
			L->head->pnext =NULL;
			L->length = 0;
			return OK;
		}
	}
}

status ListEmpty(CList L)
{

	if (L.length == 0)
	{
		printf("链式表是空表\n");
	}
	else
		printf("链式表非空\n");
	return OK;
}

status ListLength(CList L)
{
	printf("链式表长度为%d\n", L.length);
	return OK;
}

status GetElem(CList L, int i, ElemType &e)
{
	if (i > L.length)
	{
		printf("当前链式表中不存在第%d个元素", i);
		return ERROR;
	}
	CNode *ptr = L.head;
	int j = 1;
	for (; j <= i; ++j)
	{
		ptr = ptr->pnext;
	}
	e = ptr->elem;
	return OK;
}
//需要重构， 用人名访问,返回在表中的序号
int LocateElembyName(CList L, char *inname)
{
	const char *s = inname;

	int i = 1;
	CNode *ptr = L.head;
	while (ptr->pnext != NULL)
	{
		ptr = ptr->pnext;
		if (!strcmp(ElemName(ptr->elem), s))
		{
			printf("查找到链表中第一个值符合要求的元素序号是：%d\n", i);
			return OK;
		}
		i++;
	}
	printf("在链表中没有找到符合要求的元素\n");
	return ERROR;
}
//
status PriorElem(CList L, char *inname, ElemType &pre_e)
{
	const char *s = inname;

	bool first = 0;
	CNode *ptr = L.head->pnext;
	if (!strcmp(ElemName(ptr->elem), s))
	{
		first = 1;
	}
	while (ptr->pnext != NULL)
	{
		if (!strcmp(ElemName(ptr->pnext->elem), s))
		{
			pre_e = ptr->elem;
			if (first == 1)
			{
				printf("第一个符合要求的元素是首元素，没有前驱。\n第二个符合要求的元素前驱如下：\n");
				printInfo(pre_e);
				return OK;
			}
			else
			{

				printf("符合要求的元素前驱如下：\n");
				printInfo(pre_e);
				return OK;
			}
		}
		ptr = ptr->pnext;
	}
	if (first == 1)
	{
		printf("符合要求的元素是首元素，没有前驱。\n");
		return FALSE;
	}
	else
		printf("没有找到符合要求的元素\n");
	return FALSE;
}

status NextElem(CList L, char *inname, ElemType &next_e)
{

	CNode *ptr = L.head;
	if (ptr->pnext == NULL && !strcmp(ElemName(ptr->elem), inname))
	{
		printf("当前元素是最后一个元素，没有后继\n");
		return ERROR;
	}
	while (ptr->pnext->pnext != NULL)
	{
		ptr = ptr->pnext;
		if (!strcmp(ElemName(ptr->elem), inname))
		{
			next_e = ptr->pnext->elem;
			printf("该元素的后继内容如下：\n");
			printInfo(next_e);
			return OK;
		}
	}
	printf("该元素在表中未找到\n");
	return FALSE;
}

status ListInsert(CList *L, int i, ElemType e)
{
	if (i < 1 || i > L->length + 1)
	{
		printf("输入的序号无效\n");
		return ERROR;
	}
	else{
		CNode *ptr, *pnew;
	 	ptr = L->head;
		pnew = (CNode *)malloc(sizeof(CNode));
		pnew->elem = e;
		if (i == L->length + 1)
		{
			if(i ==1){
				L->head->pnext = pnew;
				L->head->pnext->pnext = NULL;
				L->length++;
				return OK;
			}
			else 
			while (ptr->pnext->pnext != NULL)
			{
				ptr = ptr->pnext;
			}
			ptr->pnext->pnext = pnew;
			ptr->pnext->pnext->pnext = NULL;
			L->length++;
			
		}
		else
		{
			int j = 1;
			for (; j < i; j++)
			{
				ptr = ptr->pnext;
			}
			pnew->pnext = ptr->pnext;
			ptr->pnext = pnew;
			L->length++;			
		}
	}
	return OK;
}

status ListDelate(CList *L, int i , ElemType &e)
{
	if (i < 1 || i > L->length)
	{
		printf("输入的序号无效\n");
		return ERROR;
	}
	int j;
	CNode *ptr = L->head;
	if (i == L->length)
	{
		while (ptr->pnext->pnext != NULL)
			ptr = ptr->pnext;
		e = ptr->pnext->elem;
		free(ptr->pnext);
		ptr->pnext = NULL;
		L->length--;
		return OK;
	}
	else
	{
		for (j=1; j < i; j++)
		{
			ptr = ptr->pnext;
		}
		e = ptr->pnext->elem;//segmentation fault
		CNode *ptemp;
		ptemp = ptr->pnext;
		ptr->pnext = ptr->pnext->pnext;
		free(ptemp);
		L->length--;
		return OK;
	}
}
//没有元素返回0
status ListTraverse(CList L)
{
	if(L.length ==0)
		return 0;
	printf("以下是链表%s的所有元素：\n", L.listname);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	CNode *ptr = L.head;
	while (ptr->pnext != NULL)
	{
		ptr = ptr->pnext;
        char *out  = cJSON_Print(ptr->elem);
        printf("%s\n", out);
	}
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	return OK;
}

status dpgroup(CListG Lg){
	int i;
	if(Lg.gnumber ==0){
		printf("当前不存在链表\n");
		return 0;
	}
	for(i = 0; i<Lg.gnumber; ++i){
		printf("第%d个表内容：\n",i+1);
		if(ListTraverse(*(Lg.contents[i]))==0)
		printf("表内容为空\n\n");
	}
	return OK;
}

status InitiaListG(CListG &ListGroup, CList **L, bool state){
	//经过此操作后程序变成基准状态
	ListGroup.contents = (CList** )malloc(10*sizeof(CList*));
	ListGroup.gsize = 10;
	ListGroup.gnumber = 1;
	if(!*L)  InitiaList(L);
	ListGroup.contents[0] = *L;
	ListGroup.curList = *L;
	if(state)//内部调用state为0
	printf("已创建一个可容纳十个链表的链表组\n");
	return OK;

}

//要求在调用函数时已经初始化了一个listgroup，并且curlist指向第一个空Clist成员
//简言之，在基准状态
//在使用这个函数之前只需要做一个简单的赋值即可：对刚初始化的链表组不需要，而如果检查到链表组内存用完，则用内存加配函数
status InfoRead(char *filename, CListG & Lg){
	FILE *pFile;
	long lSize;
	char *buffer;
	size_t result;
	const char *Fn = filename;

    pFile = fopen(Fn, "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		exit(1);
	}

	/* 获取文件大小 */
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

    buffer = (char *)malloc(lSize*sizeof(char));
    if(!(fread(buffer, sizeof(char), lSize, pFile)==lSize)){
        printf("读取文件大小出错\n");
        return 0;
    }

    cJSON * root = cJSON_Parse(buffer);
    //判断cJSON_Parse函数返回值确定是否打包成功
    if (root == NULL)
    {
        printf("读取JSON类型失败\n");
    }
    else
	//把读取到的JSON对象做分割装入每个elem（类型为cJSON）中，
    {
		//分list的组
		char listname[20], divname[20];
		int n = 1 ;//表示是第几个list
		strcpy(listname, Genelname(Lg.gnumber));
		strcpy(divname, Genelname(n));
		cJSON *divlist = cJSON_GetObjectItem(root, divname);
		do
		{
			if (!divlist){
				printf("读表操作错误\n");
				return ERROR;
			}
			//然后将一个divlist中的内容分割为多个cnode
			int divsize = cJSON_GetArraySize(divlist);
			int i = 0;
			
			CNode *ptr = Lg.curList->head;
			
			cJSON *item;
			item = cJSON_CreateObject();
			//循环将cJSON内容读入第n个list中
			for (i = 0 ; i < divsize; i++)
			{
				//获取JSON数组元素,重新读成cJSON元素 
				//以下是重新读入的方式
				//直到infowrite出了半天错，我才意识到是在读入的时候没有断开元素之间的连接导致读取结构错误
				item = cJSON_GetArrayItem(divlist, i);
				char *p = cJSON_PrintUnformatted(item);
				ElemType e = cJSON_Parse(p);
				ListInsert(Lg.curList, i+1 , e);
			}
			Lg.curList->listname = listname;
			//内存检查
			if(Lg.gnumber == Lg.gsize){
				if(!EnlargeGroup(Lg, 10)){
					printf("内存错误\n");
					return ERROR;
				}
			}
			//生成新的表名
				//为可能出现的下一个表准备空间
			ShiftCur(&(Lg.curList), Lg);
			n++;//表序号加一
			strcpy(listname ,Genelname(Lg.gnumber));
			strcpy(divname, Genelname(n));

		}while(divlist = cJSON_GetObjectItem(root, listname));
		//恢复最后一次无效操作造成的gnumber和curlist超前
		Lg.gnumber--;
		Lg.curList = Lg.contents[Lg.gnumber-1];
	}
	return OK;
}
//先把所有的内容都合并为一个cJSON，然后写入文件
status InfoWrite(char *filename, CListG Lg){
	const char * FN = filename;
	char *buffer =(char *) malloc(1000*sizeof(char));
	int bufsize = 1000;
	CList * pl;
	CNode* ptr;
	cJSON * root, *divlist;
	root = cJSON_CreateObject();

	int ln = Lg.gnumber;
	int i;
	for(i=0; i<ln; ++i){
		//把每一个list都放到root下面 
		divlist = cJSON_CreateArray();
		
		pl = Lg.contents[i];
		ptr = pl->head;

		char s1[30];  itoa(i+1, s1, 10); char s0[30] = "list"; strcat(s0, s1); const char * lname = s0;
		cJSON_AddItemToObject(root, lname, divlist);
		while(ptr->pnext!=NULL){//有死循环的while
			ptr = ptr->pnext;
			cJSON_AddItemToArray(divlist, ptr->elem);				

		}
	}

	//把root的内容输出到文件
	//test block
	const char * out = cJSON_PrintUnformatted(root);

	FILE *fp;
	if ((fp = fopen(filename, "w+")) == NULL)
	{
		printf("File open error\n ");
		return ERROR;
	}
	fwrite(out, sizeof(char), strlen(out), fp);
	fclose(fp);

	return OK;
}

status printInfo(ElemType elem){
	char *s = cJSON_Print(elem);
	printf("%s", s);
	return OK;
}

//将当前L内的list保存到Lg内的number表示的最后一个元素，加上表名，并令L表示下一个表元素（重建L）
status ShiftCur(CList **L, CListG &Lg){
	//通过将当前L内保存的内容存到contents内的新一位，然后分配一个新的表，成为当前操作表
	//总之，要建立或者回到一个基准状态
	Lg.contents[Lg.gnumber-1] = *L;//保证当前的表保存了
	(*L)->listname = (char*)malloc(20*sizeof(char));
	strcpy((*L)->listname , Genelname(Lg.gnumber));
	InitiaList(L);//包含重新分配内存给新节点
	Lg.curList  = Lg.contents[Lg.gnumber] = *L;
	(*L)->listname  = (char *)malloc(20*sizeof(char));
	Lg.gnumber++;
	return 0;
}

int  LocateElemByRef(CListG Lg, CList *L){
	int i = 0;int seq = 0;
	for (; i<Lg.gnumber; i++){
		if(Lg.contents[i]==L){
			seq = i+1;
			return seq;
		}
	}
	return 0;
}

status Elemdelate(CListG &Lg, int seq){
	if (seq > Lg.gnumber || seq < 1)
	{
		printf("序号无效\n");
		return ERROR;
	}
	Lg.gnumber--;

	for (; seq <= Lg.gnumber; seq++)
	{
		Lg.contents[seq - 1] = Lg.contents[seq];
	}
	Lg.contents[Lg.gnumber] = NULL;
	if(Lg.gnumber == 0) 
		Lg.curList = NULL;
	else 
		Lg.curList= Lg.contents[Lg.gnumber-1];
	return OK;
}

status Enlargebuffer(char * buffer, int size, int delta){
	char * newbase;
	newbase = (char *)realloc(buffer, (size+delta)*sizeof(char));
	if (!newbase)
	{
		printf("fail to reallocate memory\n");
		return ERROR;
	}
	else {
		buffer = newbase;
		return OK;
	}
}

status EnlargeGroup(CListG &ListGroup, int delta){
	CList **newbase;
	newbase = (CList **)realloc(ListGroup.contents, (ListGroup.gsize + delta) * sizeof(CList*));
	if (!newbase)
	{
		printf("fail to reallocate memory\n");
		return ERROR;
	}
	else
	{
		ListGroup.contents = newbase;
		ListGroup.curList = ListGroup.contents[ListGroup.gnumber];
		return OK;
	}
}

bool GeneJSON(ElemType & e, char *divname, char *divsex, char *divtele, char *divemail){

	cJSON_AddStringToObject(e, "name", divname);
	cJSON_AddStringToObject(e, "sex", divsex);
	cJSON_AddStringToObject(e, "telephone", divtele);
	cJSON_AddStringToObject(e, "email", divemail);
}

const char * Genelname(int num){
	char  numstr[5];
	char *listname;
	listname = (char *)malloc(30*sizeof(char ));
	itoa(num, numstr, 10);
	char  s[10] ="list"; strcat(s, numstr);
	strcpy(listname, s);
	return listname;
}


const char *ElemName(cJSON * elem){ return cJSON_GetObjectItem( elem , "name" )->valuestring; }
const char *ElemSex(cJSON * elem){  return cJSON_GetObjectItem( elem , "sex" )->valuestring;}
const char *ElemTele(cJSON * elem){  return cJSON_GetObjectItem( elem , "telephone" )->valuestring;}
const char *ElemEmail(cJSON * elem){  return cJSON_GetObjectItem( elem , "email" )->valuestring;}