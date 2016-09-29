#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#include "HCNetSDK.h"


#define		PATH   "/home/shc/DZG/user/log"

#define		CAMERA		850

unsigned char is_bit[255];
int camera_total,end_point;


char get_subip(char *src_ip,char *dest_ip,char *dest_subip){
	
	char ip_index = 0;
	char index = 0;
	
	while (index != 3){
		
		if ( src_ip[ip_index] == '.' ){
			
			index ++;
		} 
		
		dest_ip[ip_index] = src_ip[ip_index];
		ip_index ++;
	}

	dest_ip[ip_index] = '\0';
	
	index = 0;
	
	while (src_ip[ip_index]){
		
		dest_subip[index++] = src_ip[ip_index++];
		
		if (index >= 4) {
			
			return -1;
			
		}
	}
	
	dest_subip[index] = '\0';
	
	return 0;
}

char itos(int num,char *str){
	
	int i = 0;
	int j = 0;
	int temp = 0;
	
	while (num > 0){
		
		str[i++] = (num  % 10) + 0x30;	
		num /= 10;
	}
	
	str[i] = '\0';
	
	for (j = 0;j < i/2+1;j++){
		
		i--;
		temp = str[i];str[i] = str[j];str[j] = temp;
	}
	
//	printf("num:%d  str:%s \n",num,str);
	return 0;
}


char cat_ip(char *dest_ip,char *src_ip,char *src_subip){
	
	char i,j;
	
	for (i = 0; src_ip[i]; i++){
		
		dest_ip[i] = src_ip[i];
	}
	
	for (j = 0; src_subip[j]; j++){
		
		dest_ip[i++] = src_subip[j];		
	}
	dest_ip[i] = '\0';
	
	return 0;
}

int login_action(char *sIP){
	
	int iPort = 8000;
	char *sUserName = "admin";
	char *sPassword = "admin123";
	
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	
	int user_id = NET_DVR_Login_V30(sIP, iPort, sUserName, sPassword, &struDeviceInfo);

//	printf("user_id ；%d\n",user_id);
//	printf("wDevType:%d\n",struDeviceInfo.wDevType);
	
	return ((int)(struDeviceInfo.wDevType));
}


int main(int argc,char *argv[]){ 
	
	camera_total = 0;
	end_point = 0;
	/*
	FILE *pfile = NULL;
	char *filename = "/home/shc/DZG/IPC_Camera/hk_log";
	
	if ( !fopen_s(&pfile,filename,"a") ){
		
		printf("open file is fail\n");
		return 0;
	}
	
	*/

	
	FILE *pfile = NULL;
	char *filename = PATH;
	
	pfile = fopen(filename,"w");
	
	
	if (pfile == NULL){
		
		printf("open file is fail\n");
		return -1;
	}
	
//	fputs("second line\n ",pfile);
	

	
	
	char dest_ip[15];
	char dest_subip[4];
	char ip[20];
	
	char is_bit[255] = {0};
	 
//	fprintf(pfile,"********************************************************************");
//	fprintf(pfile,"__LINE__:%d\n",__LINE__);
	
	int i = 1;
	int camera_total = 0;
	
	NET_DVR_Init();
	NET_DVR_SetConnectTime(300, 1);

	get_subip(argv[1],dest_ip,dest_subip);
	
	int ip_num = atoi(dest_subip);
	
	//printf("---------------%d\n",ip_num);
	
	for (i = ip_num ;i < ip_num + 30; i++){
//	fprintf(pfile,"__LINE__:%d  IP:%s   \n",__LINE__,ip);
		itos(i,dest_subip);
		cat_ip(ip,dest_ip,dest_subip);
		
		if (login_action(ip) == CAMERA){
			
			NET_DVR_Cleanup();
			is_bit[i] = 1;
			camera_total ++;
	//		fprintf(pfile,"__LINE__:%d  IP:%s   \n",__LINE__,ip);
		//	printf("i:%d  ip:%s \n",i,ip);
		}
	
	}
	

	if (camera_total == 0){
		
		printf("{\n\"data\":[\n\n]\n}" );

		
		return 0;
	}
	
	end_point = camera_total;
	camera_total = 1;
	
	//fprintf(pfile,"{\n\"data\":[\n" );

	printf("{\n\"data\":[\n" );
	for (i = 1;i < 255; i++){
		
		if ( is_bit[i] && (camera_total == end_point) ){
			
	//		fprintf(pfile,"__LINE__:%d\n  find camera********************\n",__LINE__);
			
			itos(i,dest_subip);
			cat_ip(ip,dest_ip,dest_subip);
			
			//fprintf(pfile,"{\"{#HK_HOSTIP}\":\"");
			//fprintf(pfile,"%s",ip);
			//fprintf(pfile,"\"}");
			
			printf("{\"{#HK_HOSTIP}\":\"");
			printf("%s",ip);
			printf("\"}");
	
			camera_total ++;
			
		} else if (is_bit[i] && (camera_total != end_point) ){
			
			itos(i,dest_subip);
			cat_ip(ip,dest_ip,dest_subip);
			
			//fprintf(pfile,"{\"{#HK_HOSTIP}\":\"");
			//fprintf(pfile,"%s",ip);
			//fprintf(pfile,"\"},");
			
			
			printf("{\"{#HK_HOSTIP}\":\"");
			printf("%s",ip);
			printf("\"},\n");
	
			camera_total ++;
		}
		
	}
	
	//fprintf(pfile,"\n]\n}");
	printf("\n]\n}");
	
//	fprintf(pfile,"__LINE__:%d\n",__LINE__);
//	fprintf(pfile,"********************************************************************");
//	fclose(pfile);
	
	return 0;
}
