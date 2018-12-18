/********************
 程序设计思路：
 加上文件读写接口，最后用shell脚本控制读取含有各种范例的文件然后将输出生成新的文件
 数据交换格式为txt 换行符隔开数据
*/

/* Linear Table On Sequence Structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>

/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

typedef int status;
typedef int ElemType; //数据元素类型定义

/*-------page 22 on textbook -------*/
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
typedef struct
{ //顺序表（顺序结构）的定义
	ElemType *elem;
	int length;
	int listsize;
} SqList;
/*-----page 19 on textbook ---------*/
status InitiaList(SqList &L);
status DestroyList(SqList &L);
status ClearList(SqList &L);
status ListEmpty(SqList L);
int ListLength(SqList L);
status GetElem(SqList L, int i, ElemType &e);
status LocateElem(SqList L, ElemType e); //简化过
status PriorElem(SqList L, ElemType cur, ElemType &pre_e);
status NextElem(SqList L, ElemType cur, ElemType &next_e);
status ListInsert(SqList &L, int i, ElemType e);
status ListDelete(SqList &L, int i, ElemType &e);
status ListTraverse(SqList L); //简化过

// 自定义函数
status Txtread(char *filename, ElemType **data);
status SavingFile(SqList L);

/*--------------------------------------------*/
int main(int argc, char *argv[])
{
	SqList L;
	int op = 1;
	ElemType *dataMat;
	bool filemode = 0;
	unsigned int counter = 0;

	// if exist inputfile, then use the content of the file as input
	if (argc > 1)
	{
		if (Txtread(argv[1], &dataMat))
		{
			//输入datamat里的数据
			filemode = TRUE;
		}
		else
		{
			printf("Fail to read the data\nExit\n");
			return ERROR;
		}
	}
	while (op)
	{
		ElemType e;
		int seq;
		int length = 0;
		system("cls");
		if (filemode)
		{
			// 当前的filemode 是输入链表内容（另一种可以读入控制序列） ,而且此时返回的mat最后含有一个NULL
			int cir = 0;
			if (InitiaList(L) == OK)
			{
				while (dataMat[cir] != NULL)
				{
					if (cir < L.listsize)
					{
						L.elem[cir] = dataMat[cir];
						cir++;
					}
					else
					{
						ElemType *newbase;
						newbase = (ElemType *)realloc(L.elem, (L.listsize + LISTINCREMENT) * sizeof(ElemType));
						if (!newbase)
						{
							printf("fail to reallocate memory\n");
							return ERROR;
						}
						else
						{
							L.elem = newbase;
							L.listsize += LISTINCREMENT;
						}
						L.elem[cir] = dataMat[cir];
						cir++;
					}
				}
				L.length = cir;
			}

			printf("\nA list has been loaded\n");
			ListTraverse(L);
			//读取完元素后关闭filemode
			filemode = FALSE;
		}

		printf("\n\n");
		printf("      Menu for Linear Table On Sequence Structure \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. InitiaList       7. LocateElem\n");
		printf("    	  2. DestroyList      8. PriorElem\n");
		printf("    	  3. ClearList        9. NextElem \n");
		printf("    	  4. ListEmpty        10. ListInsert\n");
		printf("    	  5. ListLength       11. ListDelete\n");
		printf("    	  6. GetElem          12. ListTraverse\n");
		printf("    	  13.ReadData         14.Savingdata  \n");
		printf("          0. Exit      \n");
		printf("-------------------------------------------------\n");
		printf("    please choose your opreation[0~12]:");
		scanf("%d", &op);

		switch (op)
		{
		case 1:
			//printf("\n----InitiaList功能待实现！\n");
			if (InitiaList(L) == OK)
				printf("线性表创建成功！\n");
			else
				printf("操作失败！\n");
			getchar();
			getchar();
			break;
		case 2:

			if (DestroyList(L) == OK)
				printf("线性表成功销毁！\n");
			else
				printf("操作失败！\n");
			getchar();
			getchar();
			break;
		case 3:
			if (!(L.listsize))
			{
				printf("There is no list\n");
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
			if (ListEmpty(L) == TRUE)
				printf("线性表是空表\n");

			getchar();
			getchar();
			break;
		case 5:
			if (!(L.listsize))
			{
				printf("There is no list\n");
				getchar();
				getchar();
				break;
			}
			else
			{
				length = ListLength(L);
				printf("线性表长度为%d\n", length);
			}
			getchar();
			getchar();
			break;
		case 6:
			if (!(L.listsize))
			{
				printf("There is no list\n");
				getchar();
				getchar();
				break;
			}
			printf("请输入要得到的元素序号：");
			scanf("%d", &seq);
			if (GetElem(L, seq, e))
				printf("查询得到第%d个元素是%d\n", seq, e);
			else
				printf("查询失败\n");
			getchar();
			getchar();
			break;
		case 7:
			if (!(L.listsize))
			{
				printf("There is no list\n");
				getchar();
				getchar();
				break;
			}
			printf("请输入要定位的元素值：");
			scanf("%d", &e);
			if (seq = LocateElem(L, e))
				printf("查找得到第%d个元素的值符合要求\n", seq);
			else
				printf("操作失败\n");
			getchar();
			getchar();
			break;
		case 8:
			if (!(L.listsize))
			{
				printf("There is no list\n");
				getchar();
				getchar();
				break;
			}
			ElemType pre_e;
			printf("请输入一个元素值，程序将返回它的前一个元素：");
			scanf("%d", &e);
			if (PriorElem(L, e, pre_e))
				printf("元素%d之前的元素为%d", e, pre_e);
			else
				printf("查找失败");
			getchar();
			getchar();
			break;
		case 9:
			if (!(L.listsize))
			{
				printf("There is no list\n");
				getchar();
				getchar();
				break;
			}
			ElemType next_e;
			printf("请输入元素值，程序将返回它的下一个元素值：");
			scanf("%d", &e);
			if (NextElem(L, e, next_e))
				printf("元素%d的下一个元素是%d", e, next_e);
			getchar();
			getchar();
			break;
		case 10:
			if (!(L.listsize))
			{
				printf("There is no list\n");
				getchar();
				getchar();
				break;
			}
			printf("请输入待插入的元素以及插入的位置（用空格分割）：");
			scanf("%d %d", &e, &seq);
			if (ListInsert(L, seq, e) == OK)
			{
				printf("元素已成功插入，新表内容如下\n");
				ListTraverse(L);
			}
			else
				printf("插入失败\n");
			getchar();
			getchar();
			break;
		case 11:
			if (!(L.listsize))
			{
				printf("There is no list\n");
				getchar();
				getchar();
				break;
			}
			ListTraverse(L);
			printf("请输入要删除的元素序号:");
			scanf("%d", &seq);
			if (ListDelete(L, seq, e) == OK)
			{
				printf("第%d个元素删除成功，元素内容是%d\n", seq, e);
				printf("新表内容如下：\n");
				ListTraverse(L);
			}
			else
				printf("元素删除失败");
			getchar();
			getchar();
			break;
		case 12:
			if (!(L.listsize))
			{
				printf("There is no list\n");
				getchar();
				getchar();
				break;
			}
			if (!ListTraverse(L))
				printf("线性表是空表！\n");
			getchar();
			getchar();
			break;
		case 13:
			char file13[30];
			printf("请输入要读入的文件名：");
			scanf("%s", &file13);
			//如果存在表，先销毁  //另一个版本，可以保存多个链表
			if (L.listsize)
				DestroyList(L);
			if (Txtread(file13, &dataMat))
			{
				int cir = 0;
				if (InitiaList(L) == OK)
				{
					while (dataMat[cir] != NULL)
					{
						if (cir < L.listsize)
						{
							L.elem[cir] = dataMat[cir];
							cir++;
						}
						else
						{
							ElemType *newbase;
							newbase = (ElemType *)realloc(L.elem, (L.listsize + LISTINCREMENT) * sizeof(ElemType));
							if (!newbase)
							{
								printf("fail to reallocate memory\n");
								break;
							}
							else
							{
								L.elem = newbase;
								L.listsize += LISTINCREMENT;
							}
							L.elem[cir] = dataMat[cir];
							cir++;
						}
					}
					L.length = cir;
				}

				printf("\nA list has been loaded\n");
				ListTraverse(L);
			}
			getchar();
			getchar();
			break;
		case 14:

			if (SavingFile(L))
				printf("文件保存成功\n");
			else
				printf("文件保存失败\n");
			getchar();
			getchar();
			break;
		case 0:
			break;
		} //end of switch
	}	 //end of while
	printf("goodbye！\n");
	return 0;
} //end of main()
/*--------page 23 on textbook --------------------*/
status InitiaList(SqList &L)
{
	if (L.listsize)
	{
		printf("already exist a list\n");
		return ERROR;
	}
	else
	{
		L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
		if (!L.elem)
			exit(OVERFLOW);
		L.length = 0;
		L.listsize = LIST_INIT_SIZE;
	}
	return OK;
}

status DestroyList(SqList &L)
{
	//
	if (!(L.listsize))
	{
		printf("There is no list\n");
		return ERROR;
	}
	else
	{
		free(L.elem);
		L.length = 0;
		L.listsize = 0;
		return OK;
	}
}

status ClearList(SqList &L)
{
	if (!(L.listsize))
	{
		printf("There is no list\n");
		return ERROR;
	}
	else
	{
		memset(L.elem, 0, L.listsize);
		L.length = 0;
		return OK;
	}
}

status ListEmpty(SqList L)
{
	if (!(L.listsize))
	{
		printf("There is no list\n");
		return ERROR;
	}
	else
	{
		if (L.length == 0)
			return TRUE;
		else
			printf("线性表非空");
		return FALSE;
	}
}

int ListLength(SqList L)
{
	if (!(L.listsize))
	{
		printf("There is no list\n");
		return ERROR;
	}

	else
	{
		return L.length;
	}
}

status GetElem(SqList L, int i, ElemType &e)
{

	if (!(i >= 1 && i <= L.length))
	{
		printf("The list dose not have the %dth element\n", i);
		return ERROR;
	}
	else
	{
		e = L.elem[i - 1];
		return e;
	}
}

status LocateElem(SqList L, ElemType e)
{

	int i = 1;
	for (; i <= L.length; i++)
	{
		if (L.elem[i - 1] == e)
			return i;
	}
	{
		printf("No such element in the list\n");
		return ERROR;
	}
}

status PriorElem(SqList L, ElemType cur, ElemType &pre_e)
{

	int i = 1;

	for (; i <= L.length; i++)
	{
		if (L.elem[i - 1] == cur)
		{
			if (i == 1)
			{
				printf("There is no element before %d\n", cur);
				return ERROR;
			}
			else
			{
				pre_e = L.elem[i - 2];
				return pre_e;
			}
		}
	}
	printf("Could not found the required element\n");
	return ERROR;
}

status NextElem(SqList L, ElemType cur, ElemType &next_e)
{

	int i;
	for (i = 1; i <= L.length; i++)
	{
		if (L.elem[i - 1] == cur)
		{
			if (i == L.length)
			{
				printf("The input element is the last one\n");
				return ERROR;
			}
			else
			{
				next_e = L.elem[i];
				return next_e;
			}
		}
	}
	printf("Could not found the required element\n");
	return ERROR;
}

status ListInsert(SqList &L, int i, ElemType e)
{
	if (!(L.listsize))
	{
		printf("There is no list\n");
		return ERROR;
	}
	//空表插入首元素可以实现
	if ((!L.length) && i == 1)
	{
		L.elem[0] = e;
		L.length = 1;
		return OK;
	}
	if (i > (L.length + 1) || i < 1)
	{
		printf("invalid input\n");
		return ERROR;
	}
	// 具体插入操作
	else
	{
		//内存检查及更新
		if (L.length >= L.listsize)
		{
			ElemType *newbase;
			newbase = (ElemType *)realloc(L.elem, (L.listsize + LISTINCREMENT) * sizeof(ElemType));
			if (!newbase)
			{
				printf("fail to reallocate memory\n");
				return ERROR;
			}
			else
			{
				L.elem = newbase;
				L.listsize += LISTINCREMENT;
			}
		}
		int j = L.length;
		for (; j >= i; j--)
		{
			L.elem[j] = L.elem[j - 1];
		}
		L.elem[i - 1] = e;
		L.length++;
		return OK;
	}
}
status ListDelete(SqList &L, int i, ElemType &e)
{

	if (i > L.length || i < 1)
	{
		printf("invalid input\n");
		return ERROR;
	}
	e = L.elem[i - 1];
	L.length--;

	for (; i <= L.length; i++)
	{
		L.elem[i - 1] = L.elem[i];
	}
	L.elem[L.length] = NULL;
	return OK;
}

status ListTraverse(SqList L)
{
	if (!(L.listsize))
	{
		printf("There is no list\n");
		return ERROR;
	}
	int i;
	printf("\n-----------all elements -----------------------\n");
	for (i = 0; i < L.length; i++)
		printf("%d ", L.elem[i]);
	printf("\n------------------ end ------------------------\n");
	return L.length;
}

status Txtread(char *filename, ElemType **data)
{
	// only for integer number in an array
	FILE *pFile;
	long lSize;
	char *buffer;
	size_t result;
	const char *Fn = filename;

	char c;

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

	*data = (ElemType *)malloc(lSize * sizeof(ElemType) + 1);
	buffer = (char *)malloc(5 * sizeof(char));

	int i = 0;
	//   feof函数的辨析
	while (!feof(pFile)) //这里有问题，feof函数的辨析比最后一个元素延后一个循环；暂时用i-1来解决
	{
		fgets(buffer, 5, pFile); //读取文件中的一行到buf中
		(*data)[i] = atoi(buffer);
		i++;
	}

	int num = i - 1;
	//为了使输出的数组可以被正常读取完，这里加上一个NULL；
	(*data)[i - 1] = NULL;

	fclose(pFile);
	free(buffer);
	return (data != NULL);
}

status SavingFile(SqList L)
{
	FILE *fp;
	int i;
	char LF;
	LF = '\n';
	char filename[40];
	char buf[10];

	printf("请输入要保存的文件名：");
	scanf("%s", &filename);
	//写文件的方法
	if ((fp = fopen(filename, "w+")) == NULL)
	{
		printf("File open error\n ");
		return ERROR;
	}
	for (i = 0; i < L.length; i++)
	{

		itoa(L.elem[i], buf, 10);
		fputs(buf, fp);
		fputc(LF, fp);
	}
	//这里是1次性写入，对于其它物理结构，可通过遍历，逐个访问数据元素
	//并写入到文件中
	fclose(fp);
	return OK;
}