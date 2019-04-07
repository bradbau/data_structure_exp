#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include<iostream>
#include<stack>
#include<queue>
#include<vector>
using namespace std;
//#define testblock

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


#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
#define NEMELENGTH 20

typedef struct TreeNodetype
{
	ElemType data;
	struct TreeNodetype *lchild;
	struct TreeNodetype *rchild;
} TreeNode;

typedef struct
{
	TreeNode *root = NULL;
	char * Treename = NULL;
} BinaTree;

typedef struct
{
	BinaTree **contents = NULL;
	BinaTree *curBT = NULL;
	int Tnum;
	int Fsize;
} Forest;

status InitBiTree(BinaTree **T);
status DestroyBiTree(BinaTree *T);
status ClearBiTree(BinaTree *T);
status CreateBiTree( BinaTree *T, const char * def, ElemType earray);//?(二元结构数组)或者输入带空节点的输入序列

status BiTreeEmpty(BinaTree *T);
int BiTreeDepth(BinaTree *T);

TreeNode* TreeRoot(BinaTree *T); 
TreeNode* LocateByValue(BinaTree* T, char * value, int valuetype);// valuetype :name, score, id ， key /int or const char?
status dpvalue(TreeNode *e);//display the detail of the node
status Assign(BinaTree *T, TreeNode *e, ElemType value); //get TreeNode by locatebyvalue
TreeNode* Parent(BinaTree *T, TreeNode *e);
TreeNode* LeftChild(BinaTree *T, TreeNode *e);
TreeNode* RightChild(BinaTree *T, TreeNode *e);
TreeNode* LeftSibling(BinaTree *T, TreeNode *e);
TreeNode* RightSibling(BinaTree *T, TreeNode *e);
status InsertChild(BinaTree * T, TreeNode * pe, int LR, BinaTree * C);//
status DelateChild(BinaTree *T, TreeNode *pe, int LR);
status PreOrderTraverse(TreeNode *N, status(*visit)(TreeNode *e));//visit是否需要接触指针，决定这里的visit传什么参
status InOrderTraverse(TreeNode *N, status(*visit)(TreeNode *e));
status PostOrderTraverse(TreeNode *N, status(*visit)(TreeNode *e));
status LevelOrderTraverse(BinaTree *T, status(*visit)(TreeNode *e), int mode);//mode是否返回层数深度


//自定义函数 
status rcltNode(TreeNode *e);

status InitBiForest(Forest **FR, BinaTree ** T, bool state);

status Pass(TreeNode *e);
int CreateTreePre(TreeNode * *node, ElemType elist);
int CreateTreeIn(TreeNode * *node, ElemType elist);
int CreateTreePost(TreeNode * *node, ElemType elist);
status EnlargeForest(Forest *FR, int delta);
status ShiftCurBT(BinaTree **T, Forest * FR);
status compare(const char * nodevalue, char *input);//相等输出true
status ChildCopy(TreeNode* p, TreeNode *q);
status formatline(vector<int> V);
status brfprint(TreeNode * node);
status TreePrint(BinaTree *T);
void dpTreeBrief(BinaTree * Tree);
status InfoReadBT(char *filename, Forest *FR);
status InfoWriteBT(char *filename, Forest * FR);
const char * GeneTname(int num);
bool GeneJSON(ElemType & e, char *divname, char *divID, char *divtele, char *divemail);
status GeneTree_Pre(TreeNode * node, cJSON * array);

const char *ElemName(cJSON * elem);
const char *ElemID(cJSON * elem);


int main(int argc, char *argv[]){
	Forest * FR=NULL;
	BinaTree *T =NULL;
	int op = 1;

	while(op){
		
		system("cls");
		printf("\n\n");
		if(FR){
		if (FR->Tnum)
		{
			printf("当前选择的树名为：%s\n", FR->curBT->Treename);
			T = FR->curBT;
		}
		}
		printf("      二叉树实验菜单 \n");
		printf("-------------------------------------------------\n");
		printf("		1.初始二叉树		2.销毁二叉树\n");
		printf("		3.创建二叉树		4.清空二叉树\n");
		printf("		5.树是否为空		6.树深度\n");
		printf("		7.获取根节点		8.输出节点信息\n");//8是dpnode里面加locate
		printf("		9.节点赋值  		10.获取双亲结点\n");
		printf("		11.获取左孩子节点  	12.获取右孩子节点\n");
		printf("		13.获取左兄弟节点	14.获取右兄弟节点\n");
		printf("		15.插入子树 		16.删除子树\n");
		printf("		17.前序遍历 		18.中序遍历\n");
		printf("		19.后序遍历 		20.按层遍历\n");
		printf("		21.按页遍历 		22.保存至文件\n");
		if(FR&&FR->Tnum>1){//多树操作界面
		printf("		23.选择操作树  		24.删除树\n");
		printf("		25.展示所有树的简要信息\n");

		}//end of if(FR&&FR->Tnum>1)
		
		printf("		0. Exit\n");
		printf("-------------------------------------------------\n");
		printf("    请输入指令：\n");
		scanf("%d", &op);

		switch(op){
			case 1:{
				if(T){
					printf("当前已存在树\n");
					break;
				}
				else{
					if(InitBiTree(&T)==OK){
						InitBiForest(&FR, &T, 0);
						FR->Tnum = 1;
						T->Treename = (char *)malloc(NEMELENGTH*sizeof(char));
						printf("请输入树名：（输入0则使用默认名）\n");
						scanf("%20s",T->Treename);
						if(!strcmp(T->Treename, "0"))  strcpy(T->Treename, GeneTname(1));
						printf("二叉树创建成功\n");
					}
					else printf("操作失败\n");

				}
				getchar();
				getchar();
				break;
			}
			case 2:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				else if(FR->Tnum>1){
					printf("请使用24号功能完成此操作\n");
				}
				else{
					if(DestroyBiTree(T)) {
						printf("成功销毁当前树\n");
						FR->Tnum--;
						if(FR->Tnum == 0)  FR->curBT = T=NULL;
						else{
							FR->curBT = T = FR->contents[FR->Tnum-1];
						}
					}
					else printf("操作失败\n");
				}
				getchar();
				getchar();
				break;
			}
			case 3:{
				if(!T){
					InitBiForest(&FR, &T, 0);
					T =FR->curBT;
				}
				else{
					while(!T||!BiTreeEmpty(T))
					ShiftCurBT(&T, FR);
				}
				char * filename;
				filename = (char *)malloc(NEMELENGTH*sizeof(char));
				printf("本程序仅支持从JSON文件中读取树，请输入文件名：（后缀为.data）\n");
				scanf("%20s", filename);
				if(InfoReadBT(filename, FR)){
					T = FR->curBT;
					printf("成功读入文件内容到树中，现共有%d个树\n",FR->Tnum);
				}
				getchar();
				getchar();
				break;
			}
			case 4:{
				if(!T){
					printf("当前不存在树\n");getchar();
					getchar();
					getchar();
					break;
				}
				else if(ClearBiTree(T)){
					 printf("已成功清空树内容\n");
					 T->root = NULL;
				}
				getchar();
				getchar();
				break;
			}
			case 5:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				else {
					if(BiTreeEmpty(T)) printf("当前树是空树\n");
					else printf("当前树非空\n");
				}
				getchar();
				getchar();
				break;
			}
			case 6:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				int depth;
				depth = BiTreeDepth(T);
				printf("当前树的深度为%d\n",depth);
				getchar();
				getchar();
				break;
			}
			case 7:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				if(!T->root){printf("当前根节点为空\n");getchar();
				break;}
				else if(!dpvalue(T->root)) printf("操作错误\n");
				getchar();
				getchar();
				break;
			}
			case 8:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				char * s;
				int type;
				s = (char*)malloc(30*sizeof(char));
				printf("请选择要用于检索节点的检索关键字以及检索方式：（名字为1，ID为2）\n");
				scanf("%30s %d", s, &type);
				TreeNode * srhresult;
				if(!(srhresult = LocateByValue(T, s, type))) printf("查找失败\n");
				else dpvalue(srhresult);
				getchar();
				getchar();
				break;
			}
			case 9:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				char *s;
				s = (char *)malloc(31*sizeof(char));
				int type;
				printf("请选择要用于检索节点的检索关键字以及检索方式：（名字为1，ID为2，空格隔开）\n");
				scanf("%30s %d", s, &type);
				TreeNode * srhresult;
				if(!(srhresult = LocateByValue(T, s, type))) printf("查找失败\n");
				else{
					char divname[20], divID[10], divtele[20], divemail[40];
					ElemType e;
					e = cJSON_CreateObject();
					printf("请为该节点赋新值：（姓名 ID 电话 邮箱）\n");
					scanf("%19s %9s %19s %39s", divname,divID, divtele, divemail);
					GeneJSON(e, divname,divID, divtele, divemail);
					srhresult->data = e;
					printf("已成功为节点赋值\n");
				}
				getchar();
				getchar();
				break;
			}
			case 10:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				else{
				char *s;
				s = (char *)malloc(31*sizeof(char));
				int type;
				printf("请选择要用于检索节点的检索关键字以及检索方式：（名字为1，ID为2，空格隔开）\n");
				scanf("%30s %d", s, &type);
				TreeNode * srhresult,* parent;
				if(!(srhresult = LocateByValue(T, s, type))) printf("查找失败\n");
				else {
						#ifdef testblock
						dpvalue(srhresult);
						#endif
					if(!(parent = Parent(T, srhresult))){
						printf("节点没有双亲\n");
						getchar();
						getchar();						
						break;
					}
					else {
						printf("节点的双亲信息如下：\n");
						dpvalue(parent);
					}
				}
				}
				getchar();
				getchar();
				break;
			}
			case 11:{
				if (!T)
				{
					printf("当前不存在树\n");getchar();
					break;
				}
				else
				{
					char *s;
					s = (char *)malloc(31 * sizeof(char));
					int type;
					printf("请选择要用于检索节点的检索关键字以及检索方式：（名字为1，ID为2，空格隔开）\n");
					scanf("%30s %d", s, &type);
					TreeNode *srhresult, *left;
					if (!(srhresult = LocateByValue(T, s, type)))

						printf("查找失败\n");
					else{
						if(!(left = srhresult->lchild)){
							printf("节点没有左孩子\n");
							getchar();
							getchar();
							break;
						}
						printf("节点的左孩子信息如下:\n");
						dpvalue(left);
					}
				}
				getchar();
				getchar();
				break;
			}
			case 12:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				else
				{
					char *s;
					s = (char *)malloc(31 * sizeof(char));
					int type;
					printf("请选择要用于检索节点的检索关键字以及检索方式：（名字为1，ID为2，空格隔开）\n");
					scanf("%s %d", s, &type);
					TreeNode *srhresult, *right;
					if (!(srhresult = LocateByValue(T, s, type)))
						printf("查找失败\n");
					else{
						if(!(right = srhresult->rchild)){
							printf("节点没有右孩子\n");
							getchar();
							getchar();
							break;
						}
						printf("节点的右孩子信息如下:\n");
						dpvalue(right);
					}
				}
				getchar();
				getchar();
				break;
			}
			case 13:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				else
				{
					char *s;
					s = (char *)malloc(31 * sizeof(char));
					int type;
					printf("请选择要用于检索节点的检索关键字以及检索方式：（名字为1，ID为2，空格隔开）\n");
					scanf("%s %d", s, &type);
					TreeNode *srhresult,*leftsib;
					if (!(srhresult = LocateByValue(T, s, type))){
					#ifdef testblock
						dpvalue(srhresult);
					#endif
						printf("查找失败\n");
					}
					
					else{
						if(!(leftsib = LeftSibling(T, srhresult))){
							printf("节点没有左兄弟\n");
							getchar();
							getchar();
							break;
						}
						printf("节点的左兄弟信息如下:\n");
						dpvalue(leftsib);
					}
				}
				getchar();
				getchar();
				break;
			}
			case 14:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				else
				{
					char *s;
					s = (char *)malloc(31 * sizeof(char));
					int type;
					printf("请选择要用于检索节点的检索关键字以及检索方式：（名字为1，ID为2，空格隔开）\n");
					scanf("%s %d", s, &type);
					TreeNode *srhresult, *rightsib;
					if (!(srhresult = LocateByValue(T, s, type)))
						printf("查找失败\n");
					else{
						if(!(rightsib = RightSibling(T, srhresult))){
							printf("节点没有右兄弟\n");
							getchar();
							getchar();
							break;
						}
						printf("节点的右兄弟信息如下:\n");
						dpvalue(rightsib);
					}
				}
				getchar();
				getchar();
				break;
			}
			case 15:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				if((!FR)||(FR->Tnum<=1)){
					printf("当前没有可用于插入的树\n");
					break;
				}
				else {
					int Tnum = FR->Tnum, i, f = 0;
					printf("现在可用于插入的树有（序号 树名）：");
					for(i =0; i<Tnum; ++i){
						if(FR->contents[i]->root->rchild==NULL&FR->contents[i]!=T){
							printf("\t%d %s", i+1, FR->contents[i]->Treename);
							f=1;
						}
					}
					
					if(f==0) {
						printf("(没有满足要求的树)\n");
						break;
						}
					else{
						int seq;
						char *s;
						s = (char *)malloc(31 * sizeof(char));
						int type;
						TreeNode *srhresult;
						printf("\n请输入要插入的树的序号:\n");
						scanf("%d", &seq);
						printf("请输入在当前树中的插入节点信息：{检索关键字} {检索方式}（名字为1，ID为2，空格隔开）\n");
						scanf("%30s %d", s, &type);
						if (!(srhresult = LocateByValue(T, s, type)))
						printf("查找失败\n");
						else{
						//	srhresult->data= srhresult->data->next;
							#ifdef testblock
							dpvalue(srhresult);
							#endif
							getchar();
							int LR;
							printf("请输入插入子树的方式（0代表左子树，1代表右子树）：\n");
							scanf("%d", &LR);
							if(!InsertChild(T, srhresult, LR, FR->contents[seq-1])){
								printf("插入失败\n");
							}
							else
							printf("插入成功\n");
							#ifdef testblock
							dpvalue(srhresult);
							#endif
							
							}
						}
					}
				
				getchar();
				getchar();
				break;
			}

			case 16:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}

				char *s;
				s = (char *)malloc(31 * sizeof(char));
				int type, LR;
				TreeNode *srhresult;
				printf("请输入在当前树中的节点信息：{检索关键字} {检索方式}（名字为1，ID为2，空格隔开）\n");
				scanf("%30s %d", s, &type);
				if (!(srhresult = LocateByValue(T, s, type)))
					printf("查找失败\n");
					else{
						
						printf("请输入删除子树的方式（0代表左子树，1代表右子树）：\n");
						scanf("%d", &LR);
						if(!DelateChild(T, srhresult, LR))printf("删除失败\n");
						else  printf("删除成功\n");
					}
				getchar();
				getchar();
				break;
			}
			case 17:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				PreOrderTraverse(T->root, &dpvalue);
					
				getchar();
				getchar();
				break;
			}
			case 18:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				InOrderTraverse(T->root, &dpvalue);
				getchar();
				getchar();
				break;
			}
			case 19:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				PostOrderTraverse(T->root, &dpvalue);
				getchar();
				getchar();
				break;
			}
			case 20:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				LevelOrderTraverse(T, &dpvalue, 0);
				getchar();
				getchar();
				break;
			}
			case 21:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				TreePrint(T);

				getchar();
				getchar();
				break;
			}
			case 22:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				char *filename;
				filename = (char*)malloc(30*sizeof(char));
				printf("请输入要保存的文件名：");
				scanf("%s", filename);
				if(InfoWriteBT(filename, FR))
					printf("保存文件成功\n");
				else printf("保存失败\n");
				getchar();
				getchar();
				break;
			}
			case 23:{
				if(!T){
					printf("当前不存在树\n");getchar();
					break;
				}
				int type = 1;
				char * inputstr;
				inputstr = (char *)malloc(30*sizeof(char));
				printf("请输入要用于操作的树的序号：\n");
				scanf("%s", inputstr);
				const char * s= inputstr;
				if(type ==1 ){
					T = FR->curBT = FR->contents[atoi(s)-1];
					printf("已设定操作对象为第%d个树", atoi(s) );
				}
				if(type==2){
					int i, found = 0; 
					
					for(i = 0; i<FR->Tnum; ++i){
						if(compare(s,FR->contents[i]->Treename)){//这里树名匹配有引号不匹配的问题
							T = FR->curBT = FR->contents[i];
							printf("已找到第%d个树满足要求，并将其设定为操作对象\n", i+1); 
							found = 1;
							break;
						}
					}
					if(found==0) printf("未找到符合要求的树\n");
				}
				getchar();
				getchar();
				break;

			}
			case 24:{
				int seq, i;
				printf("当前的树的个数有：%d\n",FR->Tnum);
				printf("请输入要删除的树的序号：\n");
				scanf("%d", &seq);
				if(seq>0&&seq<=FR->Tnum){
					if(DestroyBiTree(FR->contents[seq-1])){
						printf("成功销毁第%d号树\n", seq);
					for(i = seq; i<FR->Tnum; ++i){
						FR->contents[i-1] = FR->contents[i];
					}
					FR->Tnum--;
					if(FR->Tnum == 0)  FR->curBT = T=NULL;
					else{
							FR->curBT = T = FR->contents[FR->Tnum-1];
						}
					}
					else printf("操作失败\n");
				}
				else printf("输入值无效\n");
				getchar();
				getchar();
				break;
			}
			case 25:{
				if(!FR||FR->Tnum<=1){
					break;
				}
				else{
					int i;
					for(i = 0; i<FR->Tnum ; ++i){
						printf("以下内容是第%d个树：\n", i+1);
						printf("\n~~~~~~~~~~~~~~~~~~~~~~~\n");
						dpTreeBrief(FR->contents[i]);
						printf("\n~~~~~~~~~~~~~~~~~~~~~~~\n");
					}

				}
				getchar();
				getchar();
				break;
			}
			case 0:{
				break;
			}


		}
	} //end of while (op)

}

//已确定T未初始
status InitBiTree(BinaTree **T){
	(*T) = (BinaTree *)malloc(sizeof(BinaTree));
	(*T)->Treename = NULL;
	(*T)->root = NULL;
	return OK;
}

status DestroyBiTree(BinaTree *T){
	
	free(T->Treename);
	rcltNode(T->root);   //这样是不对的，因为这个返回的是回收节点的状态，而非删除树的状态,但是free不返回状态
	free(T);
	//return PreOrderTraverse(T, rcltNode());
}

status ClearBiTree(BinaTree *T){
	return rcltNode(T->root);
}

status rcltNode(TreeNode *e){
	if(!e) return OK;
	if(e->lchild!=NULL) rcltNode(e->lchild);
	if(e->rchild!=NULL) rcltNode(e->rchild);
	free(e->data);
	free(e);

	return OK;
}


//输入一个含空object的JSON的array，可以嵌入inforead
//only concren the business inside the array and treenode
status CreateBiTree( BinaTree *T, const char * def, ElemType elist){
	const char *pre = "PreOrder";
	const char *in = "InOrder";
	const char *post = "PostOrder";
//	printf("%s",def);
	if(!strcmp(def, pre)){
		//前序创建树
		CreateTreePre(&(T->root), elist);
	}
	else if(!strcmp(def, in)){
		CreateTreeIn(&(T->root), elist);
	}
	else if(!strcmp(def, post)){
		CreateTreePost(&(T->root), elist);
	}
}

int CreateTreePre(TreeNode ** node, ElemType elist)
{
	
	if(!cJSON_GetArraySize(elist)) {
		(*node)=NULL;
		return OK;
	}
	//1.创建根节点
	cJSON * data;
	data = (ElemType )malloc(sizeof(cJSON));
	data = cJSON_DetachItemFromArray(elist, 0);
	if(data->child==NULL){
	 (*node) = NULL;//data为空
	 return OK;
	}
	else{
		if(!((*node) = (TreeNode*)malloc(sizeof(TreeNode)))) exit(OVERFLOW);
		const char * s = cJSON_PrintUnformatted(data);
		#ifdef testblock
		printf("%s", s);
		#endif

		(*node)->data = cJSON_Parse(s);
	}
	//2.左右子树
	CreateTreePre(&((*node)->lchild), elist);
	CreateTreePre(&((*node)->rchild), elist);
	
	free(data);
	return OK;

}

int CreateTreeIn(TreeNode * *node, ElemType elist)
{
	
	if(!cJSON_GetArraySize(elist)) {
		(*node)=NULL;
		return OK;
	}
	if(!((*node) = (TreeNode*)malloc(sizeof(TreeNode)))) exit(OVERFLOW);
	//1.左子树
	CreateTreeIn(&((*node)->lchild), elist);
	//2.根节点
	cJSON * data;
	data = (cJSON *)malloc(sizeof(cJSON));
	data = cJSON_DetachItemFromArray(elist, 0);
	if(data->child==NULL){
	 (*node) = NULL;//data为空
	 return OK;
	}
	else{
		
		const char * s = cJSON_PrintUnformatted(data);
		(*node)->data = cJSON_Parse(s);
	}
	//3.右子树
	CreateTreeIn(&((*node)->rchild), elist);
	
	free(data);
	return OK;


}

int CreateTreePost(TreeNode **node, ElemType elist)
{
	if(!cJSON_GetArraySize(elist)) {
		(*node)=NULL;
		return OK;
	}
	//1.左子树
	if(!((*node) = (TreeNode*)malloc(sizeof(TreeNode)))) exit(OVERFLOW);
	CreateTreePost(&((*node)->lchild), elist);
	//2.右子树
	CreateTreePost(&((*node)->rchild), elist);
	//3.根节点
	cJSON * data;
	data = (cJSON *)malloc(sizeof(cJSON));
	data = cJSON_DetachItemFromArray(elist, 0);
	if(data->child==NULL){
	 (*node) = NULL;//data为空
	 return OK;
	}
	else{
		
		const char * s = cJSON_PrintUnformatted(data);
		(*node)->data = cJSON_Parse(s);
	}
	
	free(data);
	return OK;


}



status BiTreeEmpty(BinaTree *T){
	return T->root==NULL;
}
int BiTreeDepth(BinaTree *T){
	int depth = 0;
	//层序遍历， 每层“访问一次”
	depth = LevelOrderTraverse(T, &Pass , 1);
	
}
status Pass(TreeNode *e){
	return OK;
}


TreeNode* TreeRoot(BinaTree *T){
	return T->root;
}

TreeNode* LocateByValue(BinaTree* T, char * value, int valuetype){//type是1输入名字，2输入ID
	if(valuetype==1){//按名查找,非递归前序遍历
		TreeNode * p = T->root;

		stack<TreeNode *>S;S.push(T->root);
		while(!S.empty()){
			
			while((p = S.top())&&p){
				S.push(p->lchild);
			}
			S.pop();//这个保证每次必有空指针的操作大有深意
			if(!S.empty()){
				p = S.top();
				if(compare(ElemName(p->data), value)) return p;
				S.pop();
				S.push(p->rchild);
			}
		}
		printf("未找到符合要求的元素\n");
		return ERROR;
	}
	if(valuetype==2){//按id查找,非递归前序遍历
		TreeNode * p = T->root;
		stack<TreeNode *>S;S.push(T->root);
		while(!S.empty()){
			
			while((p = S.top())&&p){
				S.push(p->lchild);
			}
			S.pop();//这个保证每次必有空指针的操作大有深意
			if(!S.empty()){
				p = S.top();
				if(compare(ElemID(p->data), value)) return p;
				S.pop();
				S.push(p->rchild);
			}
		}

	
		printf("未找到符合要求的元素\n");
		return ERROR;
	}
}

status dpvalue(TreeNode *e){
	char *s = cJSON_Print(e->data);
	if(!s) { printf("节点信息为空\n"); return ERROR;}
	printf("%s", s); 
	return OK;
}

status Assign(BinaTree *T, TreeNode *e, ElemType value){
	e->data = value;
	return OK;
}

TreeNode * Parent(BinaTree *T, TreeNode *e){//中序遍历
	TreeNode * p = T->root;
		stack<TreeNode *>S;S.push(T->root);
		while(!S.empty()){
			while(p = S.top()){
				S.push(p->lchild);
			}
			S.pop();
			if(!S.empty()){
				p = S.top(); S.pop();
				if(p->lchild ==e||p->rchild ==e) return p;
				S.push(p->rchild);
			}
		}
		
	return NULL;
}

TreeNode* LeftChild(BinaTree *T, TreeNode *e){
	return e->lchild;
}
TreeNode* RightChild(BinaTree *T, TreeNode *e){
	return e->rchild;
}
TreeNode* LeftSibling(BinaTree *T, TreeNode *e){//前序遍历
	TreeNode * p = T->root;
		stack<TreeNode *>S;S.push(T->root);
		while(!S.empty()){
			while(p = S.top()){
				S.push(p->lchild);
			}
			S.pop();
			if(!S.empty()){
				p = S.top(); S.pop();
				if(p->rchild ==e&&p->lchild) return p->lchild;
				S.push(p->rchild);
			}
		}
		
	return NULL;
}
TreeNode* RightSibling(BinaTree *T, TreeNode *e){
		TreeNode * p = T->root;
		stack<TreeNode *>S;S.push(T->root);
		while(!S.empty()){
			while(p->lchild){
				p = p->lchild;
				S.push(p);
			}
			if(!S.empty()){
				p = S.top();
				if(p->lchild ==e&&p->rchild) return p->rchild;
				S.pop();
				if(p->rchild)
				S.push(p->rchild);
				
			}
		}
	return ERROR;
}

status InsertChild(BinaTree * T, TreeNode * pe, int LR, BinaTree * C){
	
	if(LR){
		TreeNode *ptemp =pe->rchild;
		pe->rchild = (TreeNode*)malloc(sizeof(TreeNode));
		ChildCopy(pe->rchild, C->root);
		pe->rchild->rchild = ptemp;
		
		return OK;
	}
	else{
		TreeNode *ptemp =pe->lchild;
		pe->lchild = (TreeNode*)malloc(sizeof(TreeNode));
		ChildCopy(pe->lchild, C->root);
		pe->lchild->rchild = ptemp;
		
		return OK;
	}
}

status ChildCopy(TreeNode* p, TreeNode *q){
	//把q的内容复制给p
	p->data = cJSON_Duplicate(q->data, 1);
	if(q->lchild) {
		p->lchild = (TreeNode *)malloc(sizeof(TreeNode));
		ChildCopy(p->lchild, q->lchild);
	}
	else{p->lchild = NULL;}
	if(q->rchild) {
		p->rchild = (TreeNode *)malloc(sizeof(TreeNode));
		ChildCopy(p->rchild, q->rchild);
	}
	else p->rchild = NULL;
	return OK;

	
}

status DelateChild(BinaTree *T, TreeNode *pe, int LR){
	if(LR){
		if(!pe->rchild) return OK;
		if(!rcltNode(pe->rchild)) return ERROR;
		pe->rchild = NULL;
		return OK;
	}
	else{
		if(!pe->lchild) return OK;
		if(!rcltNode(pe->lchild)) return ERROR;
		pe->lchild = NULL;
		return OK;
	}
}

status compare(const char * nodevalue, char *input){//相等输出true
	const char * s = input;
	return !strcmp(nodevalue, s);
}

//递归
status PreOrderTraverse(TreeNode *N, status(*visit)(TreeNode *e)){
	
	if(N){
		if(!visit(N))  printf("节点访问失败\n");
		if(!PreOrderTraverse(N->lchild, visit)) return ERROR;
		if(!PreOrderTraverse(N->rchild, visit)) return ERROR;
	}
	return OK;
}

status InOrderTraverse(TreeNode *N, status(*visit)(TreeNode *e)){
	if(N){
		if(!InOrderTraverse(N->lchild, visit)) return ERROR;
		if(!visit(N))  printf("节点访问失败\n");
		if(!InOrderTraverse(N->rchild, visit)) return ERROR;
	}
	return OK;
}

status PostOrderTraverse(TreeNode *N, status(*visit)(TreeNode *e)){
	if(N){
		if(!PostOrderTraverse(N->lchild, visit)) return ERROR;
		if(!PostOrderTraverse(N->rchild, visit)) return ERROR;
		if(!visit(N))  printf("节点访问失败\n");
	}
	return OK;
}
//当mode为1时每层记一次数，同时visit参数是null，在depth中调用让它返回最大深度
//实现方法是按层依次将节点入队列
status LevelOrderTraverse(BinaTree *T, status(*visit)(TreeNode *e), int mode){
	int  maxdepth = 0, treesize = 0;
	int i, cursize;
	TreeNode *N = T->root;
	if(mode ==0 && !N){
		return ERROR;
	}
	queue<TreeNode *> Q; Q.push(N);   
	while(!Q.empty()){
		cursize = Q.size();
		for(i = 0; i< cursize; ++i){
			if((N = Q.front())&&N)
			visit(N); 
			Q.pop(); ++treesize; 
			if((N->lchild)!=NULL) Q.push(N->lchild);  
			if((N->rchild)!=NULL)  Q.push(N->rchild);
		}
		maxdepth++;
	}
	if(mode ==1) return maxdepth;
	if(mode ==2) return treesize;
	return OK;
	
}


/*  version 1
{
	vector<int> lineposition; 
	stack<TreeNode *> S; S.push(T->root); 
	TreeNode *p = T->root;
	int leftin = 0, leftexist = 0;
	//leftin 用于标记当前vector内最后一个值标记的左子树根节点是否输出
	//leftexist用于标记lineposition里的组后一个元素是否应与下一个length合并
	//if(p->data) printf("%s", ElemName(p->data));

	lineposition.push_back(0);
	//if(T->root->lchild) leftexist = 1;
		while(!S.empty()||p){
			while (p)
			{

				brfprint(p);
				if (p->lchild)
				{
					leftexist = 1;
					leftin = 0;
				}
				else
				{
					leftexist = 0;
				}
				if (leftin == 1&&!(p->lchild||p->rchild))
					lineposition.pop_back();

				if ( leftexist == 1 && leftin == 0 ){
					lineposition.push_back(strlen(ElemName(p->data)) + 4 + lineposition.back());
					leftin = 0;
				}
				if (leftexist == 0&&(p->lchild||p->rchild))
				{
					int flen = lineposition.back();
					lineposition.pop_back();
					lineposition.push_back(strlen(ElemName(p->data)) + 4 + flen);
				}
				
				
				if (p = p->rchild)
					S.push(p);
			}

			if(!S.empty()){
				p = S.top();
				S.pop();
				if(p = p->lchild)
					S.push(p);
				leftin = 1;
				leftexist = 0;
//换行加tab输出
			if(!S.empty())
				printf("\n");
				formatline(lineposition);
				
			}
		}

}
*/
/*vertion2*/
//逆前序遍历
status TreePrint(BinaTree *T)
{
	int lastmerge = 1;//对第一个元素默认上一个位置信息0是没有左子树的
	int isleft = 0;//一开始不是左子树
	vector<int> lineposition;
	lineposition.push_back(0);

	stack<TreeNode *> S; S.push(T->root); 
	TreeNode *p = T->root;
	while(!S.empty()){
		while((p = S.top()) && p){ 
			//visit p
			brfprint(p);
			if(p->lchild){
		
				if(lastmerge==0){
					lineposition.push_back(strlen(ElemName(p->data)) + 4 + lineposition.back());
				}
				else{
					int flen = lineposition.back();
					lineposition.pop_back();
					lineposition.push_back(strlen(ElemName(p->data)) + 4 + flen);
				}
				lastmerge = 0;
			} 
			else {
				if(p->rchild){
				if(lastmerge == 0 ){
					lineposition.push_back(strlen(ElemName(p->data)) + 4 + lineposition.back());
				}
				else{
					int flen = lineposition.back();
					lineposition.pop_back();
					lineposition.push_back(strlen(ElemName(p->data)) + 4 + flen);
				}
				lastmerge = 1;
				}
				else if(isleft){
					lineposition.pop_back();
				}
			}

			S.push(p->rchild);
			isleft = 0;
			}
		S.pop();
		
		if(!S.empty()){
			p = S.top();
			S.pop();
			S.push(p->lchild);
			if(p->lchild ){
				printf("\n");
				formatline(lineposition);
				lastmerge = 1;
				isleft = 1;
			}
		}
	}
	



}

status formatline(vector<int> V){

	int i, j, door = V.size();

	for(i = 0; i<door; i++){
		int spacelen;
		if(i) spacelen = V[i]-V[i-1];
		else spacelen = V[i];
		
		for(j = 0; j< spacelen-1; ++j){
			printf(" ");
		}
		printf("|");
	}
	return OK;
}

status brfprint(TreeNode * node){
	printf("----%s", ElemName(node->data));
	return OK;
}

void dpTreeBrief(BinaTree * Tree){
	printf("树名：%s", Tree->Treename);
	if(!LevelOrderTraverse(Tree, &brfprint, 0))
		printf("树内容为空\n");
	
}




//自定义函数 
//结束后把curbt赋值给T

status InfoReadBT(char *filename, Forest *FR){
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

	// 获取文件大小 
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

    buffer = (char *)malloc(lSize*sizeof(char));
    if(!(fread(buffer, sizeof(char), lSize, pFile)==lSize)){
        printf("读取文件大小出错\n");
        return 0;
    }

    cJSON * JSONroot = cJSON_Parse(buffer);
	cJSON * forest=cJSON_CreateObject();
    //判断cJSON_Parse函数返回值确定是否打包成功
    if (JSONroot == NULL)
    {
        printf("读取JSON类型失败\n");
    }
	//JSON格式见treefile.data
	
    else//开始读取forest的部分
    {
	forest  = cJSON_GetObjectItem(JSONroot, "forest");
	
	int treenum = cJSON_GetArraySize(forest);
	#ifdef testblock1
	char *out  = cJSON_Print(forest);
	printf("%s", out);
	#endif

	int i = 0;

	cJSON * treeitem = NULL;
	for(i = 0; i<treenum; ++i){//循环输入每个树的内容

		treeitem = cJSON_GetArrayItem(forest, i);
		#ifdef testblock
		char *out  = cJSON_Print(treeitem);
		printf("%s", out);
		#endif

		if(!FR->curBT->Treename){//当没有名字时才会赋予新名字
			FR->curBT->Treename = (char *)malloc(NEMELENGTH*sizeof(char));
			strcpy(FR->curBT->Treename , ( cJSON_GetObjectItem(treeitem, "name")->valuestring));
		}
		const char *order =(cJSON_GetObjectItem(treeitem, "order")->valuestring);
		cJSON *data = cJSON_GetObjectItem(treeitem, "contents");
		
		if(!CreateBiTree(FR->curBT, order, data)) {
			printf("读取树出错\n");
			break;
		};
		
		//内存检查
		if(FR->Tnum ==FR->Fsize) {
			if(!EnlargeForest(FR, 10)){
				printf("内存错误\n");
				return ERROR;
			}
		}
		//操作对象后移
		if(i!=treenum-1) ShiftCurBT(&(FR->curBT), FR);
		//cJSON_Delete(treeitem);
		//cJSON_Delete(data);
	}
	//读取完文件，释放空间
	//cJSON_Delete(forest);

//	cJSON_Delete(JSONroot);
	
	}
	return OK;
}

status InfoWriteBT(char *filename, Forest * FR){
	const char * FN = filename;
	char *buffer =(char *) malloc(1000*sizeof(char));
	int bufsize = 1000;
	BinaTree * pt;
	TreeNode* pnode;
	cJSON * root, *forest;
	root = cJSON_CreateObject();
	forest= cJSON_CreateArray();
	cJSON_AddItemToObject(root, "forest", forest);
	int ln = FR->Tnum;
	int i;
	for(i=0; i<ln; ++i){
		cJSON *divtree = cJSON_CreateObject();
		//把每一个tree都放到forest下面 
		pt = FR->contents[i];
		pnode = pt->root;
		//1.name

		const char *divname = pt->Treename;
		cJSON_AddStringToObject(divtree, "name", divname);
		//2.order   only preorder for now
		cJSON_AddStringToObject(divtree, "order", "PreOrder");

		//3.contents
		cJSON * divcontent = cJSON_CreateArray();
		GeneTree_Pre(pt->root, divcontent);
		#ifdef testblock
			char *out  = cJSON_Print(divcontent);
			printf("%s", out);
		#endif
		cJSON_AddItemToObject(divtree, "contents", divcontent);
		//4
		cJSON_AddItemToArray(forest, divtree);

	}

	//把root的内容输出到文件
	//test block
	const char * out = cJSON_PrintUnformatted(root);
#ifdef testblock
printf("%s", out);
#endif
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

status GeneTree_Pre(TreeNode * node, cJSON * array){
	cJSON* nodeobj = cJSON_CreateObject();
	if(node){
		nodeobj = cJSON_Duplicate(node->data, 1);
	}
	cJSON_AddItemToArray(array, nodeobj);
	if(!node) return OK;
	else{
		GeneTree_Pre(node->lchild, array);
		GeneTree_Pre(node->rchild, array);
	}

}


status InitBiForest(Forest **FR, BinaTree ** T, bool state){
	(*FR) = (Forest *)malloc(sizeof(Forest));
	(*FR)->contents = (BinaTree **)malloc(10*sizeof(BinaTree *));
	memset((*FR)->contents, 0, 10*sizeof(BinaTree *));
	(*FR)->Tnum = 0;
	(*FR)->Fsize = 10;
	if(!*T){ InitBiTree(T); (*FR)->Tnum = 1;}
	(*FR)->contents[0] = *T;
	(*FR)->curBT = *T;
	if(state)    printf("已创建一个可容纳十个二叉树的森林\n");
	return OK;
}

status ShiftCurBT(BinaTree **T, Forest *FR){
	//通过将当前T内保存的内容存到contents内的新一位，然后分配一个新的表，成为当前操作表
	//总之，要建立或者回到一个基准状态
	//FR->contents[FR->Tnum-1] = *T;//保证当前的表保存了      (x) 这样，如果当前操作树不是最后一个就会把后面的覆盖掉
	InitBiTree(T);//包含重新分配内存给新节点
//	strcpy((*T)->Treename , GeneTname(FR->Tnum));
	
	FR->curBT  = FR->contents[FR->Tnum] = *T;
	FR->curBT->Treename = NULL;
	FR->Tnum++;
	return 0;
}

const char * GeneTname(int num){
	char  numstr[5];
	char *Treename;
	Treename = (char *)malloc(30*sizeof(char ));
	itoa(num, numstr, 10);
	char  s[10] ="tree"; strcat(s, numstr);
	strcpy(Treename, s);
	return Treename;
}



status EnlargeForest(Forest *FR, int delta){
	BinaTree **newbase;
	newbase = (BinaTree **)realloc(FR->contents, (FR->Fsize + delta) * sizeof(BinaTree*));
	if (!newbase)
	{
		printf("fail to reallocate memory\n");
		return ERROR;
	}
	else
	{
		FR->contents = newbase;
		FR->curBT = FR->contents[FR->Tnum];
		return OK;
	}
}


bool GeneJSON(ElemType & e, char *divname, char *divID, char *divtele, char *divemail){

	cJSON_AddStringToObject(e, "name", divname);
	cJSON_AddStringToObject(e, "ID", divID);
	cJSON_AddStringToObject(e, "telephone", divtele);
	cJSON_AddStringToObject(e, "email", divemail);
}


const char *ElemName(cJSON * elem){ return cJSON_GetObjectItem( elem , "name" )->valuestring; }
const char *ElemID(cJSON * elem){  return cJSON_GetObjectItem( elem , "ID" )->valuestring;}