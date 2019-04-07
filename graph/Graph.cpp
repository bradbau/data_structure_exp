#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include<iostream>
#include<queue>
#include"cJSON.h"
using namespace std;




#define DEBUG

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2


typedef int status;
typedef cJSON* InfoType; //数据元素类型定义
typedef int IDType;


#define MAX_VERTEX_NUMBER 20
#define MAX_MESSAGE_LENGTH 30

typedef struct ArcNode{
    IDType adjvex;
    struct ArcNode * nextarc;
   
} ArcNode;
typedef struct vnode{
    InfoType data;
    ArcNode *firstarc;
} VNode, AdjList[MAX_VERTEX_NUMBER];

typedef struct {
    char *name;
    AdjList vertices;
    int vexnum, arcnum;
} DiGraph;//定义有向图

typedef struct {
    DiGraph ** contents, *curGP;
    int size, num;
} GPListtype;


//函数定义
status InitiaG(GPListtype **GL, DiGraph **G);
status DestroyGraph(DiGraph *G);
int LoacteVex(DiGraph * G, char * value, int valuetype);//返回节点在邻接表结构中的序号
InfoType GetVex(DiGraph *G, int serial);//用邻接表中的序号索引得到节点
status AssignVex(DiGraph *G, int v, InfoType data);

int FirstAdjVex(DiGraph * G, int v);
int NextAdjVex(DiGraph * G, int v, int w);
status InsertVex(DiGraph *G, VNode * vex);
status DeleteVex(DiGraph *G, int v);
status InsertArc(DiGraph * G, int tail, int head);
status DeleteArc(DiGraph * G, int tail, int head);
status DFSTraverse(DiGraph *G, int v, status(*visit)(VNode * node));
status DFS(DiGraph *G,int v,int visited[], status(*visit)(VNode * node));
status BFSTraverse(DiGraph *G, int  stp, status(*visit)(VNode * node));

//selfdefine

status RclctNode(ArcNode * node);
status InfoReadG(GPListtype* GL, char *filename);
status InfoWriteG(GPListtype * GL, char *filename);
status dpvalue(VNode * e);
status ShiftCurG(GPListtype* GL, DiGraph *G);
status EnlargeGP(GPListtype* GL, int delta);
status match(const char * nodevalue, char *input);//相等输出true
void GeneNode(VNode * e ,char * ID, char *weight, char *msg);
const char *ElemID(cJSON * elem);
const char *ElemWeight(cJSON * elem);
const char *ElemMessage(cJSON * elem);


int main(){
    GPListtype * GL=NULL;
    DiGraph *G = NULL;

    int op = 1;

    while(op){
        system("cls");
        printf("\n\n");
        if(GL&&GL->num){
            printf("当前选择的图名为：%s\n", GL->curGP->name);
			G = GL->curGP;
        }
        printf("        图实验菜单  \n");
        printf("--------------------------------------------\n");
        printf("        1.创建图         2.销毁图\n");
        printf("        3.查找顶点        4.获得顶点值\n");
        printf("        5.顶点赋值         6.获得第一邻接点\n");
        printf("        7.获得下一邻接点    8.插入顶点\n");
        printf("        9.删除顶点          10.插入弧\n");
        printf("        11.删除弧           12.深度优先搜索遍历\n");
        printf("        13.广度有限搜索遍历  14.保存至文件\n");
        printf("        15.选择操作对象      0.退出\n");
        printf("---------------------------------------------\n");
        printf("请输入指令：\n");
        scanf("%d", &op);

        switch(op){

            case 1:{
                char * filename;
                filename = (char *)malloc(30*sizeof(char));
                printf("本实验通过文件信息创建图, 请输入文件名：\n");
                scanf("%s", filename);
                if(!G){
                    InitiaG(&GL, &G);
                }
                else ShiftCurG(GL, G);
                if(InfoReadG(GL, filename) ==OK){
                    printf("文件读取成功，现在有%d个图", GL->num);
                    G = GL->curGP;
                }
                else{
                    printf("文件读取失败\n");
                }
                getchar();
                getchar();
                break;
            }
           
            case 2:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                    if(GL->num>1){
                        int i;
                        printf("当前有%d个图，请输入要删除的图序号", GL->num);
                        scanf("%d", &i);
                        GL->curGP = GL->contents[i - 1];
                        if (i > 0 && i <= GL->num)
                        {
                            for (; i < GL->num; ++i)
                            {
                                GL->contents[i - 1] = GL->contents[i];
                            }
                            if (DestroyGraph(GL->curGP) == OK)
                            {
                                printf("销毁图成功\n");
                                GL->num--;
                                G = GL->curGP = GL->contents[GL->num - 1];
                            }
                        }
                    }
                    else {
                        if(DestroyGraph(G)==OK) {
                             printf("销毁图成功\n");
                             GL->num = 0;
                             G = GL->curGP = NULL;
                        }
                    }
                }
                getchar();
                getchar();
                break;
            }
            case 3:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                    int valuetype, vex;
                    char * value;
                    value = (char*)malloc(30*sizeof(char));
                    printf("请输入要查找的顶点关键字以及关键字类型：{关键字} {关键字类型(ID用1表示， 权值用2表示)}\n");
                    scanf("%s %d", value, &valuetype);
                    if(vex = LoacteVex(G, value, valuetype), vex>=0){
                        printf("查找到对应顶点位于数组%d号位", vex);
                    }
                    else printf("顶点查找失败\n");
                }
                getchar();
                getchar();
                break;
            }
            case 4:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                    int v;
                    printf("请输入顶点序号：\n");
                    scanf("%d", &v);
                    if(v>0&&v<=G->vexnum){
                        dpvalue(&(G->vertices[v-1]));
                    }
                    else printf("输入顶点不存在\n");
                }
                getchar();
                getchar();
                break;
            }
            case 5:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                     int valuetype, vex;
                    char * value;
                    value = (char*)malloc(30*sizeof(char));
                    printf("请输入要查找的顶点关键字以及关键字类型：{关键字} {关键字类型(ID用1表示， 权值用2表示)}\n");
                    scanf("%s %d", value, &valuetype);
                    if(vex = LoacteVex(G, value, valuetype), vex>=0){
                        printf("查找到对应顶点位于数组%d号位", vex);
                        printf("请输入该定点的新值:{ID} {weight} {brief message}\n");
                        char * ID ,*weight, *msg;
                        ID = (char*)malloc(30*sizeof(char));
                        weight  = (char*)malloc(30*sizeof(char));
                        msg = (char*)malloc(30*sizeof(char));
                        scanf("%s %s %s ", ID, weight, msg);
                        GeneNode(&(G->vertices[vex]), ID, weight, msg);
                        printf("顶点已成功赋值\n");
                    }
                    else printf("顶点查找失败\n");
                }
                getchar();
                getchar();
                break;
            }
            case 6:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                     int valuetype, vex;
                    char * value;
                    value = (char*)malloc(30*sizeof(char));
                    printf("请输入要查找的顶点关键字以及关键字类型：{关键字} {关键字类型(ID用1表示， 权值用2表示)}\n");
                    scanf("%s %d", value, &valuetype);
                    if(vex = LoacteVex(G, value, valuetype), vex>=0){
                        printf("查找到对应顶点位于数组%d号位", vex);
                        int u = FirstAdjVex(G, vex);
                        if(u>=0){
                            printf("他的第一个邻接点是%d号单元内的节点，内容如下：\n\n", u);
                            dpvalue(&(G->vertices[u]));
                        }
                        else printf("该节点没有邻接点\n");
                    }
                    else printf("顶点查找失败\n");

                }
                getchar();
                getchar();
                break;
            }
            case 7:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                    int valuetype, vex, u, w;
                    char * value;
                    value = (char*)malloc(30*sizeof(char));
                    printf("请输入要查找的顶点关键字以及关键字类型：{关键字} {关键字类型(ID用1表示， 权值用2表示)}\n");
                    scanf("%s %d", value, &valuetype);
                    if(vex = LoacteVex(G, value, valuetype), vex>=0){
                        printf("查找到对应顶点位于数组%d号位", vex);
                        printf("请输入该节点的一个邻接点:\n");
                        scanf("%d", &u);
                       if( w = NextAdjVex(G, vex, u), w>=0){
                           printf("该节点的下一个邻接点是%d号元素，内容如下：\n", w);
                           dpvalue(&(G->vertices[w]));
                       }
                       else {
                           printf("未查找到下一个邻接点\n");
                       }
                    }
                }
                getchar();
                getchar();
                break;
            }
            case 8:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                     printf("请输入插入顶点的值:{ID} {weight} {brief message}\n");
                        char * ID ,*weight, *msg;
                        ID = (char*)malloc(30*sizeof(char));
                        weight  = (char*)malloc(30*sizeof(char));
                        msg = (char*)malloc(30*sizeof(char));
                        scanf("%s %s %s ", ID, weight, msg);

                        VNode * pn;
                        GeneNode(pn, ID, weight, msg);
                        InsertVex(G, pn);
                        printf("已成功插入节点\n");
                }
                getchar();
                getchar();
                break;
            }
            case 9:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                     int valuetype, vex;
                    char * value;
                    value = (char*)malloc(30*sizeof(char));
                    printf("请输入要查找的顶点关键字以及关键字类型：{关键字} {关键字类型(ID用1表示， 权值用2表示)}\n");
                    scanf("%s %d", value, &valuetype);
                    if(vex = LoacteVex(G, value, valuetype), vex>=0){
                        printf("查找到对应顶点位于数组%d号位", vex);
                        if( DeleteVex(G, vex) ==OK) printf("删除成功\n");
                        else printf("删除失败\n");
                    }
                    else printf("顶点查找失败\n");
                }
                getchar();
                getchar();
                break;
            }
            case 10:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                    int divhead, divtail;
                    printf("请输入要插入的弧头与弧尾元素序号:{弧头} {弧尾}\n");
                    scanf("%d %d", &divhead, &divtail);
                    
                    if(InsertArc(G, divtail, divhead)==OK){
                        printf("弧插入成功\n");
                    }
                    else{
                        printf("弧插入失败\n");
                    }
                }
                getchar();
                getchar();
                break;
            }
            case 11:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                    int divhead, divtail;
                    printf("请输入要删除的弧头与弧尾元素序号:{弧头} {弧尾}\n");
                    scanf("%d %d", &divhead, &divtail);
                    
                    if(DeleteArc(G, divtail, divhead)==OK){
                        printf("弧删除成功\n");
                    }
                    else{
                        printf("弧删除失败\n");
                    }
                }
                getchar();
                getchar();
                break;
            }
            case 12:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                    int v;
                    printf("请输入开始遍历的节点在数组中的序号：\n");
                    scanf("%d", &v);
                    printf("以下是深度优先搜索遍历的过程\n");
                    DFSTraverse(G, v, &dpvalue);
                }
                getchar();
                getchar();
                break;
            }
            case 13:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                    int v;
                    printf("请输入开始遍历的节点在数组中的序号：\n");
                    scanf("%d", &v);
                    printf("以下是广度优先搜索遍历的过程\n");
                    BFSTraverse(G, v, &dpvalue);
                }
                getchar();
                getchar();
                break;
            }
            case 14:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                    char * filename;
                    filename = (char *)malloc(30*sizeof(char));
                    printf("请输入文件名:\n");
                    scanf("%s", filename);
                    if(InfoWriteG(GL, filename) ==OK)
                    printf("文件保存成功\n");
                    else printf("文件保存失败\n");
                }
                getchar();
                getchar();
                break;
            }
            case 15:{
                if(!G){
                    printf("当前不存在图\n"); 
                }
                else{
                    if(GL->num>1){
                        int serial;
                        printf("当前有%d个图， 请输入要选择的图的序号：(从一开始)\n");
                        scanf("%d", &serial);
                        if(serial>0&&serial<=GL->num)
                            G = GL->curGP = GL->contents[serial-1];
                        else printf("输入序号不存在\n");
                    }
                    else {
                        printf("当前只有一个图\n");
                    }
                }
                getchar();
                getchar();
                break;
            }
            case 0:
                break;
        }//end of switch




    }// while(op)

}

status InitiaG(GPListtype **GL, DiGraph **G){
    (*GL) = (GPListtype*)malloc(sizeof(GPListtype));
    (*GL)->contents = (DiGraph**)malloc(10*sizeof(DiGraph*));
    (*GL)->curGP = (*G) = (*GL)->contents[0];
    (*GL)->size = 10;
    (*GL)->num = 1;
    (*G)->name = NULL;
    return OK;
}

status DestroyGraph(DiGraph *G){
    free(G->name);
    int i;
    for(i = 0; i< G->vexnum; ++i){
        if(G->vertices[i].firstarc)
        RclctNode(G->vertices[i].firstarc);
    }
    return OK;
}
status RclctNode(ArcNode * node){
    if(node->nextarc){
        RclctNode(node->nextarc);
    }
    else 
        free(node);
        return OK;
}


//返回节点在邻接表结构中的序号  valuetype为1表示ID 2表示 weight (3表示信息关键字用正则匹配)
int LoacteVex(DiGraph * G, char * value, int valuetype){
    int i;
    for(i = 0; i<G->vexnum; ++i){
        //match by ID
        if(valuetype == 1){
            if(match(ElemID(G->vertices[i].data), value)){
                return i;
            }
        }
        //match by weight
        else if(valuetype ==2){
            if(match(ElemWeight(G->vertices[i].data), value)){
                return i;
            }
        }
        //暂不加正则表达式
    }
    return -1;
}

//用邻接表中的序号索引得到节点信息
InfoType GetVex(DiGraph *G, int serial)
{
    if (serial < 0 || serial >= G->vexnum)
        return ERROR;
    else
        return G->vertices[serial].data;
}

status AssignVex(DiGraph *G, int v, InfoType data)
{
    if (v < 0 || v >= G->vexnum)
        return ERROR;
    else
        G->vertices[v].data = data;
    return OK;
}

int FirstAdjVex(DiGraph * G, int v){

    if(G->vertices[v].firstarc)
        return G->vertices[v].firstarc->adjvex;
    else return -1;
}

int NextAdjVex(DiGraph * G, int v, int w){
    ArcNode * pn = G->vertices[v].firstarc;

    while(pn->adjvex!= G->vertices[w].firstarc->adjvex){
        pn = pn->nextarc;
        w--;
    }
    if(pn = pn->nextarc)
        return pn->adjvex;
    else return -1;
}

status InsertVex(DiGraph *G, VNode * vex){
    if(G->vexnum ==MAX_VERTEX_NUMBER){
        return ERROR;
        printf("数据节点已满\n");
    } 
    G->vertices[G->vexnum].data = cJSON_Duplicate(vex->data, 0);
    G->vertices[G->vexnum].firstarc = NULL;
    G->vexnum++;
    return OK;
}



status DeleteVex(DiGraph *G, int v){
    int i;
    int targetID =atoi( ElemID(G->vertices[v].data));
    
    RclctNode(G->vertices[v].firstarc);
    for(i = v; i< G->vexnum-1; ++i){
        G->vertices[i] = G->vertices[i+1];
    }
    
 //   G->vertices[G->vexnum] delete ;
    G->vexnum--;

    for(i =0; i< G->vexnum; ++i){
        ArcNode *pn = G->vertices[i].firstarc;
        if(pn->adjvex == targetID){
            G->vertices[i].firstarc = pn->nextarc;
            break;
        }
        else {
            if(pn->adjvex>targetID){
                pn->adjvex--;
            }
        }
        while(pn->nextarc){
            if(pn->nextarc->adjvex = targetID){
                ArcNode * temp = pn->nextarc;
                pn->nextarc = pn->nextarc->nextarc;
                free(temp);
            }
            else if(pn->adjvex>targetID){
                pn->adjvex--;
            }
            pn = pn->nextarc;
        }
    }
    return OK;
}


status InsertArc(DiGraph * G, int tail, int head){
    if((tail<0||tail>G->vexnum)||( head>G->vexnum|| head<0)){
        return ERROR;
    }
    ArcNode* pn = G->vertices[tail].firstarc;
    while(pn->nextarc){
        if(head == pn->adjvex ){
            printf("重复对弧赋值\n");
            return ERROR;
        }
        pn = pn->nextarc;
    }
        pn->nextarc = (ArcNode *)malloc(sizeof(ArcNode));
        pn->nextarc->adjvex  = head;
        pn->nextarc->nextarc = NULL;
    return OK;
}

status DeleteArc(DiGraph * G, int tail, int head){
    
    ArcNode * fnode = G->vertices[tail].firstarc;
    if(fnode->adjvex == head){
        G->vertices[tail].firstarc = fnode->nextarc;
        G->arcnum--;
        return OK;
    }
    else{
        while(fnode->nextarc){
            if(fnode->nextarc->adjvex == head){
                fnode->nextarc = fnode->nextarc->nextarc;
                G->arcnum--;
                return OK;
            }
        }
    }
    return FALSE;
}


status DFSTraverse(DiGraph *G,int v, status(*visit)(VNode * node)){
    int visited[G->vexnum] = {0};
    int i;
    for(i = 0; i <G->vexnum; ++i){
        if(!visited[(v+i)%G->vexnum]){
            DFS(G, (v+i)%G->vexnum, visited, visit);
        }
    }
    return OK;
}

status DFS(DiGraph *G,int v,int visited[], status(*visit)(VNode* node)){
    visited[v] = TRUE; visit(&(G->vertices[v]));
    int w;
    for(w = FirstAdjVex(G, v); w >= 0; w = NextAdjVex(G, v, w)){
         if(!visited[w]){
            DFS(G, w, visited, visit);
    }
    }
   return OK;
}

status BFSTraverse(DiGraph *G, int  stp, status(*visit)(VNode * node)){
    bool visited[G->vexnum] = {0};
    queue<int> Q;
    int v, w, i;
    for(i = 0; i<G->vexnum; ++i){
        v = (i+stp)%G->vexnum;
        if(!visited[v]){
            visit(&(G->vertices[v])) ;visited[v] = TRUE;  Q.push(v);
            while(!Q.empty()){
                int u = Q.front(); Q.pop();
                for(w = FirstAdjVex(G, u); w>=0 ;w = NextAdjVex(G, u, w)){
                    if(!visited[w]){
                        visit(&(G->vertices[w])); visited[w] = TRUE; Q.push(w);
                    }
                }
            }
        }
    }
}




status InfoReadG(GPListtype *GL, char *filename){
    FILE *pFile;
	long lSize;
	char *buffer = NULL;
	size_t result;
	const char *Fn = filename;




    pFile = fopen(Fn, "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		return ERROR;
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
    cJSON * data = cJSON_GetObjectItem(JSONroot, "contents");
    cJSON_Delete(JSONroot);
    int i, size = cJSON_GetArraySize(data);
   
    for(i = 0; i<size; ++i){
        //读取一个图内容
        cJSON * divg = cJSON_CreateObject();
        divg = cJSON_GetArrayItem(data, i);
        //1.name
        GL->curGP->name = (char*)malloc(30*sizeof(char));
        strcpy(GL->curGP->name , cJSON_GetObjectItem(divg, "name")->valuestring);
        //2.node
        cJSON *nodelist = cJSON_GetObjectItem(divg, "vertices");
        GL->curGP->vexnum = cJSON_GetArraySize(nodelist);
        if(GL->curGP->vexnum > MAX_VERTEX_NUMBER){
            printf("节点数超出限制\n");
            return ERROR;
        }
        int j;
        for(j = 0; j<GL->curGP->vexnum; ++j){
            //逐个添加节点
            GL->curGP->vertices[j].data = cJSON_GetArrayItem(nodelist, j);
            GL->curGP->vertices[j].firstarc = NULL;
        }
        cJSON_Delete(nodelist);
        //3.edge
        cJSON * edge = cJSON_GetObjectItem(data, "arcs");
        GL->curGP->arcnum = cJSON_GetArraySize(edge);
        for(j = 0; j< GL->curGP->arcnum; ++j){
            cJSON * divedge = cJSON_GetArrayItem(edge, j);
            int archead = LoacteVex(GL->curGP, cJSON_GetObjectItem(divedge, "head")->valuestring, 1);
            int arctail = LoacteVex(GL->curGP, cJSON_GetObjectItem(divedge, "tail")->valuestring, 1);//这里可能有类型转换错误
           if(InsertArc(GL->curGP, arctail, archead)!=OK){
               printf("弧添加错误\n");
               return ERROR;
           }
           cJSON_Delete(divedge);
        }
        GL->num++;
        ShiftCurG(GL, GL->curGP);
    }//end of for

}

status InfoWriteG(GPListtype * GL, char *filename){
    const char * FN  = filename;
    DiGraph * G;
    cJSON *root, *contents;
    root = cJSON_CreateObject();
    contents = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "contents", contents);
    int i, num = GL->num;
    for(i = 0; i<num ;++i){
        //create one graph and add to JSON contents
        cJSON * divG;
        divG = cJSON_CreateObject();
        G =  GL->contents[i];
        //1.add name
        const char * divname = G->name;
        cJSON_AddStringToObject(divG, "name", divname);
        //2. add vertices
        cJSON * divvex;
        divvex = cJSON_CreateArray();
        int vexnum = G->vexnum;
        int j;
        for(j = 0; j< vexnum ; ++j){
            cJSON_AddItemToArray(divvex, G->vertices[j].data);
        }
        cJSON_AddItemToObject(divG, "vertices", divvex);
        cJSON_Delete(divvex);
        //3. add arcs
        cJSON * arcs;
        arcs = cJSON_CreateArray();
        for(j =0; j<vexnum; ++j){
            ArcNode * arcto = G->vertices[j].firstarc;
            while(arcto){
                cJSON * divarc;
                divarc = cJSON_CreateObject();
                cJSON_AddStringToObject(divarc, "head", ElemID(G->vertices[arcto->adjvex].data));
                cJSON_AddStringToObject(divarc, "tail", ElemID(G->vertices[j].data));
                cJSON_AddItemToArray(arcs, divarc);
                cJSON_Delete(divarc);
                arcto = arcto->nextarc;
            }
        }
        cJSON_AddItemToObject(divG, "arcs", arcs);
        cJSON_Delete(arcs);

        //add divgraph to contents JSON
        cJSON_AddItemToArray(contents, divG);
        cJSON_Delete(divG);
    }
    char * out = cJSON_Print(root);
    #ifdef DEBUG
    printf("%s", out);
    #endif // DEBUG
    cJSON_Delete(root);
    FILE* fp;
    if ((fp = fopen(filename, "w+")) == NULL)
	{
		printf("File open error\n ");
		return ERROR;
	}
	fwrite(out, sizeof(char), strlen(out), fp);
	fclose(fp);

    return OK;

}

status ShiftCurG(GPListtype *GL, DiGraph *G){
    if(GL->num==GL->size){
        EnlargeGP(GL, 10);
    }
    G = GL->curGP = GL->contents[GL->num];
    GL->num++;
}

status dpvalue(VNode * e){
	char *s = cJSON_Print(e->data);
	if(!s) { printf("节点信息为空\n"); return ERROR;}
	printf("%s", s); 
	return OK;
}

status EnlargeGP(GPListtype* GL, int delta){
    
    GL->contents = (DiGraph**)realloc(GL->contents, (GL->size+delta)*sizeof(DiGraph*));
    GL->size +=delta;
}
status match(const char * nodevalue, char *input){//相等输出true
	const char * s = input;
	return !strcmp(nodevalue, s);
}

void GeneNode(VNode * e ,char * ID, char *weight, char *msg){
    cJSON *data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "ID", ID);
    cJSON_AddStringToObject(data, "weight", weight);
    cJSON_AddStringToObject(data, "message", msg);
    
    e->firstarc = NULL;
    e->data = data;
}

const char *ElemID(cJSON * elem){ return cJSON_GetObjectItem( elem , "ID" )->valuestring; }
const char *ElemWeight(cJSON * elem){  return cJSON_GetObjectItem( elem , "weight" )->valuestring;}
const char *ElemMessage(cJSON * elem){  return cJSON_GetObjectItem( elem , "message" )->valuestring;}