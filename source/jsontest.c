#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>

#include"cJSON.h"

typedef int status; 
status ReadList(char *inname);
int main(){
    //char * name;
    //name =(char *)malloc(10*sizeof(char));

    char  name[20] = "graph.txt";
    ReadList(name);
    return 0;
}

status ReadList(char *filename)
{
    FILE *pFile;
	long lSize;
	char *buffer;
	size_t result;
	const char *Fn = filename;

    pFile = fopen("graph.txt", "rb");
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
     


    cJSON *cjson = cJSON_Parse(buffer);
    //判断cJSON_Parse函数返回值确定是否打包成功
    if (cjson == NULL)
    {
        printf("json pack into cjson error...");
    }
    else
    { //打包成功调用cJSON_Print打印输出
        char *out  = cJSON_Print(cjson);
        printf("%s\n", out);

       char *ID = 
    }
    return 0;
}