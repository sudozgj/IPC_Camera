#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Alarm.h"

#include <iostream>
#include <string.h>

#include <fstream>

#define fPath "/home/shc/DZG/IPC_Camera/test.txt"

using namespace std;

int main(int argc,char *argv[]){
	
	// ifstream f(fPath,ios::in);

	// if(!f){
	// 	printf("打开文件失败！\n");
	// 	return 0;
	// }
	
	// f<<"文件内容..."<<endl;
	// f.close();

	char *a = "sad";
	char *b = "sa";
	printf(strcmp(a,b));

    return 0; 
}