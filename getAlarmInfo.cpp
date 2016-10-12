#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "HCNetSDK.h"

#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>


#define		ERROR		0x200
#define		OK			0x300

#define fPath "/home/shc/DZG/IPC_Camera/almLog"

using namespace std;

int key_0=0;
int key_1=0;

// time_t time;

char *almIP_0="";	//ip
int type_0;		//type
time_t time_0;	//time

char *almIP_1="";	//ip
int type_1;		//type
time_t time_1;	//time


void rcdFile(NET_DVR_ALARMER *pAlarmer,NET_DVR_ALARMINFO_V40 struAlarmInfo,char tmp[64],time_t now_time){
	ofstream f(fPath,ios::app);

	//printf("|||||||| : %d\n", struAlarmInfo.pAlarmData);	//可能要用到
	// dwAlarmChanNum 发生报警通道数据个数，用于从NET_DVR_ALARMINFO_V40的pAlarmData(变长数据部分)计算出所有发生的报警通道号，四字节表示一个报警通道 


	int i;
	if(!f){
		printf("  打开文件失败！\n");
		return;
	}
	printf("+-进入记录函数中\n");
	if(struAlarmInfo.struAlarmFixedHeader.dwAlarmType==3){
		printf("+---进入3中\n");
		printf("+-----通道 :%d\n",struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwAlarmInputNo);
		f<<pAlarmer->sDeviceIP<<"-"<<"移动侦测报警"<<"-"<<tmp<<"-"<<now_time<<"-"<<struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwAlarmInputNo<<";";
		// printf("--struIOAlarm.dwAlarmInputNo 发生报警的报警输入通道号，一次只有一个:%d\n", struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwAlarmInputNo);		
		// printf("--struIOAlarm.dwTrigerAlarmOutNum  触发的报警输出个数:%d--\n", struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwTrigerAlarmOutNum );
		// printf("--struIOAlarm.dwTrigerRecordChanNum   触发的录像通道个数:%d--\n", struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwTrigerRecordChanNum  );
		// printf("--struAlarmChannel.dwAlarmChanNum    发生报警通道数据个数:%d--\n", struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struAlarmChannel.dwAlarmChanNum   );
	}
	if(struAlarmInfo.struAlarmFixedHeader.dwAlarmType==6){
		printf("+---进入6中\n");
		printf("+-----通道 :%d\n",struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwAlarmInputNo);		
		f<<pAlarmer->sDeviceIP<<"-"<<"遮挡报警"<<"-"<<tmp<<"-"<<now_time<<"-"<<struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwAlarmInputNo<<";";
		// printf("--struIOAlarm.dwAlarmInputNo 发生报警的报警输入通道号，一次只有一个:%d\n", struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwAlarmInputNo);		
		// printf("--struIOAlarm.dwTrigerAlarmOutNum  触发的报警输出个数:%d--\n", struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwTrigerAlarmOutNum );
		// printf("--struIOAlarm.dwTrigerRecordChanNum   触发的录像通道个数:%d--\n", struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwTrigerRecordChanNum  );
		// printf("--struAlarmChannel.dwAlarmChanNum    发生报警通道数据个数:%d--\n", struAlarmInfo.struAlarmFixedHeader.uStruAlarm.struAlarmChannel.dwAlarmChanNum   );
	}

	//IP-alarm Type-时间-时间戳
	// f<<&p<<endl;
	f.close();
}


void CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void*
pUser)
{
	//----------系统时间------------
	time_t now_time; 
	now_time = time(NULL);
	// cout<<now_time<<endl; 		//时间戳

	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X",localtime(&t) ); 
	// printf("%s\n", tmp);			//时间

	int i;
	NET_DVR_ALARMINFO_V40 struAlarmInfo;
	memcpy(&struAlarmInfo, pAlarmInfo, sizeof(NET_DVR_ALARMINFO_V40));

	// cout<<"_______________________start_______________________"<<endl;
	// cout<<"		条件0："<<almIP_0<<"____"<<pAlarmer->sDeviceIP<<endl;
	// cout<<"		条件0："<<type_0<<"____"<<struAlarmInfo.dwAlarmType<<endl;
	// cout<<"		条件0："<<now_time<<"____"<<time_0<<endl;

	switch (struAlarmInfo.struAlarmFixedHeader.dwAlarmType)
	{
		case 3: //移动侦测报警
		{
			if(key_0==0){
				cout<<"NO.1——第1次进入移动侦测记录日志"<<endl;
				rcdFile(pAlarmer,struAlarmInfo,tmp,now_time);
				almIP_0 = pAlarmer->sDeviceIP;	//ip
				type_0 = struAlarmInfo.struAlarmFixedHeader.dwAlarmType;		//type
				time_0 = now_time;	//time
				key_0++;
			}
			else if(strcmp(almIP_0,pAlarmer->sDeviceIP)==0 && (now_time-time_0)>60 ){
				cout<<"NO.2——超过1分钟进入移动侦测记录日志..."<<endl;
				rcdFile(pAlarmer,struAlarmInfo,tmp,now_time);
				
				almIP_0 = pAlarmer->sDeviceIP;	//ip
				type_0 = struAlarmInfo.struAlarmFixedHeader.dwAlarmType;		//type
				time_0 = now_time;	//time
				key_0++;
			}
			else{
				cout<<"NO.3——在1分钟内移动侦测重复，不记录日志..."<<endl;
				almIP_0 = pAlarmer->sDeviceIP;	//ip
				type_0 = struAlarmInfo.struAlarmFixedHeader.dwAlarmType;		//type
				time_0 = now_time;	//time
				key_0++;
			}
			break;
		}
		case 6: //遮挡报警报警
		{
			if(key_1==0){
				cout<<"NO.1——第1次进入遮挡报警记录日志"<<endl;
				rcdFile(pAlarmer,struAlarmInfo,tmp,now_time);
				almIP_1 = pAlarmer->sDeviceIP;	//ip
				type_1 = struAlarmInfo.struAlarmFixedHeader.dwAlarmType;		//type
				time_1 = now_time;	//time
				key_1++;
			}
			else if(strcmp(almIP_1,pAlarmer->sDeviceIP)==0 && (now_time-time_1)>60 ){
				cout<<"NO.2——超过1分钟进入遮挡报警记录日志..."<<endl;
				rcdFile(pAlarmer,struAlarmInfo,tmp,now_time);
				
				almIP_1 = pAlarmer->sDeviceIP;	//ip
				type_1 = struAlarmInfo.struAlarmFixedHeader.dwAlarmType;		//type
				time_1 = now_time;	//time
				key_1++;
			}
			else{
				cout<<"NO.3——在1分钟内遮挡报警重复，不记录日志..."<<endl;
				almIP_1 = pAlarmer->sDeviceIP;	//ip
				type_1 = struAlarmInfo.struAlarmFixedHeader.dwAlarmType;		//type
				time_1 = now_time;	//time
				key_1++;
			}
			break;
		}
		default:{
			printf("--其他--\n");
			printf("%d\n", struAlarmInfo.struAlarmFixedHeader.dwAlarmType);
			break;
		}
	}
	cout<<"\n";
}



int main(int argc,char *argv[]){
	time_t now_time;
	now_time = time(NULL);
	time_0=now_time;

	printf("COMM_ALARM: %d\n", COMM_ALARM);
	printf("COMM_ALARM_V30: %d\n", COMM_ALARM_V30);
	printf("COMM_ALARM_V40: %d\n", COMM_ALARM_V40);
	printf("\n");

	//---初始化---
	NET_DVR_Init();


	//---设置连接时间与重连时间---
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//---设置报警回调函数---
	NET_DVR_SetDVRMessageCallBack_V30(MessageCallback, NULL);

	//---启用监听---
	LONG lHandle;
	lHandle = NET_DVR_StartListen_V30(NULL,7200, MessageCallback, NULL);
	if (lHandle < 0)
	{
		printf("NET_DVR_StartListen_V30 error, %d\n", NET_DVR_GetLastError());
		// NET_DVR_Logout(user_id);
		NET_DVR_Cleanup();
		return 0;
	}
	while(1)
	{
		sleep(2);
	}
	// printf("==4==\n");

	// //---停止监听---
	// if (!NET_DVR_StopListen_V30(lHandle))
	// {
	// 	printf("==5==\n");
	// 	printf("NET_DVR_StopListen_V30 error, %d\n", NET_DVR_GetLastError());
	// 	// NET_DVR_Logout(user_id);
	// 	NET_DVR_Cleanup();
	// 	return 0;
	// }
	// printf("==6==\n");

	//---注销用户---
	// NET_DVR_Logout(user_id);

	//---释放 SDK 资源---
	// NET_DVR_Cleanup();

	return 0;
}