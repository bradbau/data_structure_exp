/*---------链式存储结构的线性表（下称链表）实验---------*/
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <iostream>

/*---------------------预设状态常数---------------------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

typedef int status;
typedef int ElemType; //数据元素类型定义：定义为整型

/*---------------------预设容量常数---------------------*/
#define GROUPSIZE  12	//并行的链表数
#define FLNM 30	//文件名最大长度

/*---------------------实验数据结构---------------------*/
typedef struct /*节点类型*/LNode{  //链表（线性结构）的定义
	ElemType data;
	LNode * next;
}/*指针类型*/* Linklist;

/*---------------------目标函数---------------------*/
status IntiaList(Linklist & L);		//初始化表
status DestroyList(Linklist & L);		//销毁表
status ClearList(Linklist L);		//清空表
status ListEmpty(Linklist L);			//判定空表
int ListLength(Linklist L);			//求表长
status GetElem(Linklist L,int i,ElemType & e);				//获得元素
status LocateElem(Linklist L,ElemType e);						//查找元素
status PriorElem(Linklist L,ElemType cur,ElemType & pre_e);	//获得前驱
status NextElem(Linklist L,ElemType cur,ElemType & next_e);	//获得后继
status ListInsert(Linklist L,int i,ElemType e);				//插入元素
status ListDelete(Linklist L, int i, ElemType & e);			//删除元素
status ListTrabverse(Linklist L);								//遍历表
status LoadFile(Linklist * L, char *filename, FILE *fp);		//读取链表
status SaveFile(Linklist * L, char *filename, FILE *fp);		//保存链表

/*---------以多单链表操作未以函数形式呈现---------*/

/*--------------------------------------------*/
int main(void) {
	Linklist H[GROUPSIZE];	//头指针，不是头节点
	ElemType e, pre_e, next_e;
	int op = 1, i, sn = 0;	//sn链表编号，默认为1，为充分利用空间对应为0
	for (i = 0; i < GROUPSIZE; i++) {
		H[i] = NULL;	//初始化链表头指针(初始化标志)
	}
	/*文件功能*/
	char filename[FLNM];
	FILE *fp = NULL;
	/*文件功能*/
	while (op) {
		system("cls");
		printf("                链表实验操作菜单               \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. 初始化表        9.  获得后继\n");
		printf("    	  2. 销毁表          10. 插入元素\n");
		printf("    	  3. 清空表          11. 删除元素\n");
		printf("    	  4. 判定空表        12. 遍历链表\n");
		printf("    	  5. 求表长          13. 读取链表\n");
		printf("    	  6. 获得元素        14. 保存链表\n");
		printf("    	  7. 查找元素        15. 切换链表\n");
		printf("    	  8. 获得前驱        \n");
		printf("    	  0. 退出\n");
		printf("-------------------------------------------------\n");
		printf("           当前正在操作的是第%d号链表          \n", sn + 1);
		printf("-------------------------------------------------\n");
		printf("请选择你的操作[0~15]:");
		scanf("%d", &op);
		switch (op) {
		case 1:
			/*初始化表*/
			if (H[sn])	//防止重复初始化
				printf("表已初始化，请不要重复初始化\n");
			else {
				if (IntiaList(H[sn]) == OK)	printf("链表创建成功！\n");
				else printf("链表创建失败！\n");
			}
			getchar(); getchar();
			break;
		case 2:
			/*销毁表*/
			if (!H[sn]) printf("链表销毁失败！\n原因：表未初始化\n");
			else {
				if (DestroyList(H[sn]) == OK) printf("链表销毁成功！\n");
				else printf("链表销毁失败！\n");
			}
			getchar(); getchar();
			break;
		case 3:
			/*清空表*/
			if (!H[sn]) printf("链表清空失败！\n原因：表未初始化\n");
			else {
				if (ClearList(H[sn]) == OK) printf("链表清空成功！\n");
				else printf("链表清空失败！\n");
			}
			getchar(); getchar();
			break;
		case 4:
			/*判定空表*/
			if (!H[sn]) printf("判定空表失败！\n原因：表未初始化\n");
			else {
				if (ListEmpty(H[sn])) printf("该链表是空表！\n");
				else printf("该链表不是空表！\n");
			}
			getchar(); getchar();
			break;
		case 5:
			/*求表长*/
			if (!H[sn]) printf("求表长失败！\n原因：表未初始化\n");
			else printf("该链表长为：%d\n", ListLength(H[sn]));
			getchar(); getchar();
			break;
		case 6:
			/*获得元素*/
			if (!H[sn]) printf("获得元素失败！\n原因：表未初始化\n");
			else {
				if (!ListLength(H[sn])) printf("该表为空表,其中无元素\n");	//空表中无元素
				else {
					printf("请输入你想要获得链表中第几个元素:\n");
					if (scanf_s("%d", &i)) {	//输入整数
						if (GetElem(H[sn], i, e) == ERROR)	//输入的i越界时
							printf("请输入介于1与%d之间的整数\n", ListLength(H[sn]));
						else printf("该链表中第%d个元素为：%d\n", i, e);
					}
					else {
						printf("请输入介于1与%d之间的整数\n", ListLength(H[sn]));
						getchar(/*收集这个输入*/);
					}
				}
			}
			getchar(); getchar();
			break;
		case 7:
			/*查找元素*/
			if (!H[sn]) printf("查找元素失败！\n原因：表未初始化\n");
			else {
				status t;	//此处声明生命周期短不冲突
				printf("请输入你要查找的元素\n");
				scanf_s("%d", &e);
				if (t = LocateElem(H[sn], e))	//减少一次运算/返回值t不为0时
					printf("找到目标元素，它位于第%d位序\n", t);
				else printf("未找到目标元素\n");	//即返回值t为0时（越界、输入错误）
			}
			getchar(); getchar();
			break;
		case 8:
			/*获得前驱*/
			if (!H[sn]) printf("获得前驱失败！\n原因：表未初始化\n");
			else {
				printf("请输入你要获得前驱的元素\n");
				scanf_s("%d", &e);
				if (!LocateElem(H[sn], e))	//由于7中已检验输入合法性，故此处无需额外检验（除首元素无前驱外），直接调用LocateElem即可
					printf("它不是链表中元素\n");		//两种情形返回提示不同
				else {
					if(PriorElem(H[sn], e, pre_e))
						printf("链表中元素%d的前驱是%d\n", e, pre_e);
					else printf("链表中元素%d无前驱\n因为它是第一个元素\n", e);
				}
			}
			getchar(); getchar();
			break;
		case 9:
			/*获得后继*/
			if (!H[sn]) printf("获得后继失败！\n原因：表未初始化\n");
			else {
				printf("请输入你要获得后继的元素\n");
				scanf_s("%d", &e);
				if (!LocateElem(H[sn], e))
					printf("它不是链表中元素\n");
				else {
					if (NextElem(H[sn], e, next_e))
						printf("链表中元素%d的后继是%d\n", e, next_e);
					else printf("链表中元素%d无后继\n因为它是最后一个元素\n", e);
				}
			}
			getchar(); getchar();
			break;
		case 10:
			/*插入元素*/
			if (!H[sn]) printf("插入元素失败！\n原因：表未初始化\n");
			else {
				if (!ListLength(H[sn])) printf("输入'1'以确认插入表的第一个元素\n");	//对空表没有元素，对其中元素标号无意义。同时这样提示也无需改变函数的参数结构
				else printf("请输入你要插入元素的位置\n可选位置：第1~%d元素之前（即你插入的元素将成为该表的第几个元素）\n", ListLength(H[sn]) + 1);//第ListLength(L) + 1个元素之前即第ListLength(L)个元素之后
				if(scanf_s("%d",&i)){
					if (i<1 || i>ListLength(H[sn]) + 1) printf("插入元素失败！\n原因：只能在第1与第%d元素之前插入元素\n", ListLength(H[sn]) + 1);
					else {
						printf("请输入你要插入的元素\n");
						scanf_s("%d", &e);
						if(ListInsert(H[sn], i, e)) printf("插入元素成功！\n");
						else printf("插入元素失败！\n");
					}
				}
				else { 
					printf("插入元素失败！\n原因：请输入整数\n");
					getchar(/*收集这个输入*/);
				}
			}
			getchar(); getchar();
			break;
		case 11:
			/*删除元素*/
			if (!H[sn]) printf("删除元素失败！\n原因：表未初始化\n");
			else {
				if (!ListLength(H[sn])) printf("删除元素失败！\n原因：表为空\n");
				else {
					printf("请输入你要删除元素的位置\n可选位置：第1~%d元素\n", ListLength(H[sn]));
					if (scanf_s("%d", &i)) {	//检查输入的i是否合理
						if (i<1 || i>ListLength(H[sn])) printf("删除元素失败！\n原因：只能删除第1与第%d元素之间的元素\n", ListLength(H[sn]));
						else {
							if (ListDelete(H[sn], i, e)) printf("删除元素成功！\n被删除的元素为%d\n", e);
							else printf("删除元素失败！\n");
						}
					}
					else{
						printf("删除元素失败！\n原因：请输入整数\n");
						getchar(/*收集这个输入*/);
					}
				}
			}
			getchar(); getchar();
			break;
		case 12:
			/*遍历元素*/
			if(!H[sn]) printf("遍历元素失败！\n原因：表未初始化\n");
			else {
				if (!ListLength(H[sn])) printf("\n--- all elements ---\n链表为空\n---- end  visit ----\n");
				else ListTrabverse(H[sn]);
			}
			getchar(); getchar();
			break;
		case 13:
			/*读取元素*/
			printf("注意：读取文件会清空当前未保存的数据\n请输入文件名:\n");
			scanf_s("%s", filename, FLNM);
			if (LoadFile(H, filename, fp))	//链表组应整体保存，故传入链表组首地址
				printf("文件读取成功!\n");
			else printf("文件保存失败!\n ");;
			getchar(); getchar();
			break;
		case 14:
			/*保存元素*/
			printf("请输入文件名:\n");
			scanf_s("%s", filename, FLNM);
			if (SaveFile(H, filename, fp))	//链表组应整体保存，故传入链表组首地址
				printf("链表保存成功!\n");
			else printf("链表保存失败!\n ");;
			getchar(); getchar();
			break;
		case 15:
			/*切换链表*/
			printf("请输入你想要操作哪一个链表：\n可选择的表有第1号至第%d号表\n", GROUPSIZE);
			int t;
			if (scanf_s("%d", &t)) {	//防止sn意外改变
				if (t >= 1 && t <= 12) {
					sn = t - 1;	//利用数组的第0位元素
					printf("切换成功。\n当前正在操作的是第%d号链表\n", sn + 1);
				}
				else printf("当前只有%d个链表供操作。请输入1与%d之间的正整数\n", GROUPSIZE, GROUPSIZE);
			}
			else {
				printf("请输入1与%d之间的正整数\n", GROUPSIZE);
				getchar();	//收集多余字符
			}
			getchar(); getchar();
			break;
		case 0:
			break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
	return 0;
}//end of main()
/*-------------------具体函数部分-------------------*/

/*---------------------初始化表---------------------*/
status IntiaList(Linklist & h) {
	h =(Linklist) malloc(sizeof(LNode));
	if (h){	//空间分配成功
		/*头节点数据域无意义，不操作*/
		h->next = NULL;	//新初始化的链表中无元素，故头节点指向为空
		return OK;
	}
	else return OVERFLOW;
}

/*---------------------销毁表---------------------*/
status DestroyList(Linklist & h) {
	Linklist q, p;
	p = h->next;	//主函数中的初始化检验确保了h->next有意义
	if (!p) free(h);	//空表直接释放头节点
	else while (p) {
		q = p->next;
		free(p);
		p = q;
	}
	h = NULL;
	return OK;
}

/*---------------------清空表---------------------*/
status ClearList(Linklist h) {
	Linklist q, p;
	p = h->next;
	while (p) {	//此处while兼具if的功能
		q = p->next;
		free(p);
		p = q;
	}
	//清空不是销毁，无需重置表为未初始化
	h->next = NULL;	//,但需置h->next为NULL
	return OK;
}

/*---------------------判定空表---------------------*/
status ListEmpty(Linklist h) {
	if(!h->next) return TRUE;	//空表即头节点后无元素
	else return FALSE;
}

/*---------------------求表长---------------------*/
status ListLength(Linklist h) {	//不用&防止h在遍历中被改变
	int i = 0;	//表长,初始需为0
	while (h->next) {
		h = h->next;
		i++;
	}
	return i;
}

/*---------------------获得元素---------------------*/
status GetElem(Linklist h, int i, ElemType & e) {	//不用&防止h在遍历中被改变
	if (i >= 1 && i <= ListLength(h)) {	//判断对应位置是否在链表中
		while (i) {	//直接利用i决定指针后移次数
			h = h->next;	
			i--;	//直接利用i决定指针后移次数
		}
		e = h->data; 
		return OK;
	}	
	else return ERROR;
}

/*---------------------查找元素---------------------*/
status LocateElem(Linklist h, ElemType e) {	//不用&防止h在遍历中被改变
	int i = 0;
	while (h->next) {
		h = h->next;
		i++;
		if (h->data == e) return i;	//在单链表中物理位置与逻辑位置一一对应
	}
	return FALSE;	//没找到
}

/*---------------------获得前驱---------------------*/
status PriorElem(Linklist h, ElemType cur, ElemType & pre_e) {
	Linklist p = h, q = h->next;
	while (q&&q->data != cur) {	//未遍历至表尾且未遍历至第一个与cur完全相同的元素
		p = q;
		q = q->next;
	}
	if (q->data == cur && p != h) {	//找到了并结束while循环后,且不是首元素
		pre_e = p->data;
		return TRUE;
	}
	else { //首元素无前驱
		pre_e = 0xcccccccc;	//初始化pre_e,使其无定义
		/*注意语句顺序*/
		return FALSE;	//首元素无前驱
	}	
}

/*---------------------获得后继---------------------*/
status NextElem(Linklist h, ElemType cur, ElemType & next_e) {
	while (h&&h->data != cur) {	//未遍历至表尾且未遍历至第一个与cur完全相同的元素
		h = h->next;	//未对h引用防止其值改变
	}
	if (h->next&&h->data == cur) {	//找到了并结束while循环后其不为表尾
		next_e = h->next->data;
		return TRUE;
	}
	else {
		next_e = 0xcccccccc;	//初始化next_e,使其无定义
		return FALSE;	//尾元素无后继
	}	
}

/*---------------------插入元素---------------------*/
status ListInsert(Linklist h, int i, ElemType e){	//未对h引用防止其值改变
	Linklist t;	//临时指针
	while (--i/*注意此处实为i-1不为0,即第i个节点前*/) {	//因为i的合理性已在main函数中检验，故仅用i即可完成遍历
		h = h->next;	//未对h引用防止其值改变
	}
	t = h->next;
	if (h->next = (Linklist)malloc(sizeof(LNode))) {	//空间分配成功
		h->next->data = e;
		h->next->next = t;
		return OK;
	}
	else exit(OVERFLOW);
}

/*---------------------删除元素---------------------*/
status ListDelete(Linklist h, int i, ElemType & e) {
	Linklist t;	//临时指针
	while (--i/*注意此处实为i-1不为0,即第i-1个节点后*/) {	//因为i的合理性已在main函数中检验，故仅用i即可完成遍历
		h = h->next;	//未对h引用防止其值改变
	}
	t = h->next->next;	//被删除元素的指针域
	e = h->next->data;
	free(h->next);	//被删除元素
	h->next = t;
	return OK;
}

/*---------------------遍历表---------------------*/
status ListTrabverse(Linklist h) {	//未对h引用防止其值改变
	printf("\n------------- all elements --------------\n");
	while (h = h->next)		//头节点数据域无意义
		printf("%d ", h->data);	
	printf("\n------------------ end ------------------\n");
	return OK;
}

/*--------------------读取链表--------------------*/
status LoadFile(Linklist * h, char *filename, FILE *fp) {
	int sn, i;
	if (fopen_s(&fp, filename, "r"))	return ERROR;
	for (sn = 0; sn < GROUPSIZE; sn++) {//整体读取
		int t;
		fread(&t, sizeof(int), 1, fp);	//首先读取表长（初始化情况）
		if (t + 1) {	//初始化的表，即t+1不为0，即t不为-1
			IntiaList(h[sn]);	//已初始化的表
			for (i = 1; i <= t; i++) {	//依据之前记录的表长遍历并插入
				Linklist p = h[sn];		//防止头指针被改变
				ElemType data= p->data;
				fread(&data, sizeof(ElemType), 1, fp);
				p = p->next;	//i在下面会被使用，此处无需使用指针域为空来结束循环
				if (ListInsert(h[sn], i, data));	//正常则继续插入
				else return ERROR;
			}
		}
		else h[sn] = NULL;	//为初始化的表
	}
	fclose(fp);
	return OK;
}

/*--------------------保存链表--------------------*/
status SaveFile(Linklist * h, char *filename, FILE *fp) {
	int sn, not_ini = -1;	//未初始化表标识符
	if (fopen_s(&fp, filename, "w"))	return ERROR;
	for (sn = 0; sn < GROUPSIZE; sn++) {//整体保存
		if (h[sn]) {
			int length= ListLength(h[sn]);
			fwrite(&length, sizeof(int), 1, fp);	//对已初始化表放置放置标识符(即表长)
			while (h[sn] = h[sn]->next){
				ElemType data = h[sn]->data;
				fwrite(&data, sizeof(ElemType), 1, fp);
			}
		}
		else fwrite(&not_ini, sizeof(int), 1, fp);	//对未初始化表放置放置标识符(初始化后的表表长不可能为-1)，且不再写入数据，否则会报错。
	}
	fclose(fp);
	return OK;
}
