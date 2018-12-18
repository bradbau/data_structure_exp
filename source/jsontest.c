#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>

#include"cJSON.h"

typedef int status; 
status ReadList(const char *inname);
int main(){
    //char * name;
    //name =(char *)malloc(10*sizeof(char));

    const char * name = "list1.data";
    ReadList(name);
    return 0;
}

status ReadList(const char *filename)
{
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

        cJSON *forest = cJSON_GetObjectItem(cjson, "forest");
        cJSON *tree1 = cJSON_GetArrayItem(forest, 0);
        cJSON *array = cJSON_GetObjectItem(tree1, "contents");
        cJSON *empty = cJSON_GetArrayItem(array, 1);
        cJSON *normal = cJSON_GetArrayItem(array, 0);
        cJSON *null = cJSON_CreateNull();
    }
    return 0;
}