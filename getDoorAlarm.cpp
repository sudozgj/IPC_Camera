#include <stdio.h> 
#include <iostream> 
// #include "Windows.h" 
#include "HCNetSDK.h"
#include <unistd.h>
#include <string.h>
#include <fstream>

using namespace std;

#define fPath "/home/shc/DZG/IPC_Camera/doorLog"

void rcFile(NET_DVR_ALARMER *pAlarmer,NET_DVR_ACS_ALARM_INFO struAcsAlarmInfo)
{
	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X",localtime(&t)); 
	ofstream f(fPath,ios::app);
	if(!f){
		printf("  打开文件失败！\n");
		return;
	}
	printf("进入记录函数中__\n");
	if(struAcsAlarmInfo.dwMinor==1)//合法卡
	{	printf("IP-%s\n",pAlarmer->sDeviceIP);
		printf("时间-%d:%d:%d-",struAcsAlarmInfo.struTime.dwHour,struAcsAlarmInfo.struTime.dwMinute,struAcsAlarmInfo.struTime.dwSecond);
		printf("门编号-%d-合法卡认证通过-%d-", struAcsAlarmInfo.struAcsEventInfo.dwDoorNo,struAcsAlarmInfo.dwMinor);
		printf("读卡器编号-%d-",struAcsAlarmInfo.struAcsEventInfo.dwCardReaderNo);
		printf("卡号-%s\n", struAcsAlarmInfo.struAcsEventInfo.byCardNo);
		// rcFile(pAlarmer,);
		f<<pAlarmer->sDeviceIP<<"-"<<tmp<<"-门编号-"<<struAcsAlarmInfo.struAcsEventInfo.dwDoorNo<<"-合法卡认证通过-"<<struAcsAlarmInfo.dwMinor;
		f<<"-读卡器编号-"<<struAcsAlarmInfo.struAcsEventInfo.dwCardReaderNo<<"-卡号-"<<struAcsAlarmInfo.struAcsEventInfo.byCardNo<<endl;
	}
	else if(struAcsAlarmInfo.dwMinor==9)	//无效卡
	{
		printf("时间-%d:%d:%d-",struAcsAlarmInfo.struTime.dwHour,struAcsAlarmInfo.struTime.dwMinute,struAcsAlarmInfo.struTime.dwSecond);
		printf("门编号-%d-无效卡号-%d-",struAcsAlarmInfo.struAcsEventInfo.dwDoorNo,struAcsAlarmInfo.dwMinor);
		printf("读卡器编号-%d-",struAcsAlarmInfo.struAcsEventInfo.dwCardReaderNo);
		printf("卡号-%s\n", struAcsAlarmInfo.struAcsEventInfo.byCardNo);

		f<<pAlarmer->sDeviceIP<<"-"<<tmp<<"-门编号-"<<struAcsAlarmInfo.struAcsEventInfo.dwDoorNo<<"-无效卡号-"<<struAcsAlarmInfo.dwMinor;
		f<<"-读卡器编号-"<<struAcsAlarmInfo.struAcsEventInfo.dwCardReaderNo<<"-卡号-"<<struAcsAlarmInfo.struAcsEventInfo.byCardNo<<endl;
		// break;
	}
	else if(struAcsAlarmInfo.dwMinor==21)//门锁打开
	{
		printf("时间-%d:%d:%d-",struAcsAlarmInfo.struTime.dwHour,struAcsAlarmInfo.struTime.dwMinute,struAcsAlarmInfo.struTime.dwSecond);
		printf("门编号-%d-", struAcsAlarmInfo.struAcsEventInfo.dwDoorNo);
		printf("门锁打开-%d\n",struAcsAlarmInfo.dwMinor);

		f<<pAlarmer->sDeviceIP<<"-"<<tmp<<"-门编号-"<<struAcsAlarmInfo.struAcsEventInfo.dwDoorNo<<"-门锁打开-"<<struAcsAlarmInfo.dwMinor<<endl;
	}
	else if(struAcsAlarmInfo.dwMinor==22)//门锁关闭
	{
		printf("时间-%d:%d:%d-",struAcsAlarmInfo.struTime.dwHour,struAcsAlarmInfo.struTime.dwMinute,struAcsAlarmInfo.struTime.dwSecond);
		printf("门编号-%d-", struAcsAlarmInfo.struAcsEventInfo.dwDoorNo);	
		printf("门锁关闭-%d\n",struAcsAlarmInfo.dwMinor);

		f<<pAlarmer->sDeviceIP<<"-"<<tmp<<"-门编号-"<<struAcsAlarmInfo.struAcsEventInfo.dwDoorNo<<"-门锁关闭-"<<struAcsAlarmInfo.dwMinor<<endl;
	}
	else if(struAcsAlarmInfo.dwMinor==23)//开门按钮打开
	{
		printf("时间-%d:%d:%d-",struAcsAlarmInfo.struTime.dwHour,struAcsAlarmInfo.struTime.dwMinute,struAcsAlarmInfo.struTime.dwSecond);
		printf("门编号-%d-", struAcsAlarmInfo.struAcsEventInfo.dwDoorNo);	
		printf("开门按钮打开-%d\n",struAcsAlarmInfo.dwMinor);

		f<<pAlarmer->sDeviceIP<<"-"<<tmp<<"-门编号-"<<struAcsAlarmInfo.struAcsEventInfo.dwDoorNo<<"-开门按钮打开-"<<struAcsAlarmInfo.dwMinor<<endl;
	}
	else if(struAcsAlarmInfo.dwMinor==24)//开门按钮放开
	{
		printf("时间-%d:%d:%d-",struAcsAlarmInfo.struTime.dwHour,struAcsAlarmInfo.struTime.dwMinute,struAcsAlarmInfo.struTime.dwSecond);
		printf("门编号-%d-", struAcsAlarmInfo.struAcsEventInfo.dwDoorNo);
		printf("开门按钮放开-%d\n",struAcsAlarmInfo.dwMinor);

		f<<pAlarmer->sDeviceIP<<"-"<<tmp<<"-门编号-"<<struAcsAlarmInfo.struAcsEventInfo.dwDoorNo<<"-开门按钮放开-"<<struAcsAlarmInfo.dwMinor<<endl;
	}


}


BOOL CALLBACK MSesGCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* 
pUser)
{  //回调函数中不可有耗时较长的操作，不能调用该 SDK（HCNetSDK.dll）本身的接口。  
	//以下代码仅供参考，实际应用中不建议在回调函数中直接处理数据保存文件，  
	//例如可以使用消息的方式(PostMessage)在消息响应函数里进行处理。 
	// printf("lCommand = %s\n", lCommand);

	// printf("————————————————————————————————————————————————————\n");
	// printf("lCommand:%d\n", lCommand);	//20482
	// printf("%s\n", pAlarmInfo);	//P
	// printf("%d\n", strlen(pAlarmInfo));	//2
	// printf("门禁主机报警信息：COMM_ALARM_ACS = %s\n", COMM_ALARM_ACS);
	// printf("门禁身份证刷卡信息：COMM_ID_INFO_ALARM = %s\n", COMM_ID_INFO_ALARM);
	// printf("门禁通行人数信息：COMM_PASSNUM_INFO_ALARM = %s\n", COMM_PASSNUM_INFO_ALARM);
	
	// printf("序列号：%s\n",pAlarmer->sSerialNumber);	//DS-K2601...
	// printf("设备通信端口：%d\n", pAlarmer->wLinkPort);//8000
	// printf("IP地址：%s\n", pAlarmer->sDeviceIP );	//192.168.1.80

	switch (lCommand)
	{  
		case COMM_ALARM_ACS://门禁主机报警信息：COMM_ALARM_ACS	20482
		{
			// printf("————门禁主机报警信息————COMM_ALARM_ACS:%d\n", COMM_ALARM_ACS);
			NET_DVR_ACS_ALARM_INFO struAcsAlarmInfo = {0}; 
			memcpy(&struAcsAlarmInfo, pAlarmInfo, sizeof(struAcsAlarmInfo));    //按需处理报警信息结构体中的信息...... 
			// if(struAcsAlarmInfo.dwMajor!=3){
			if(struAcsAlarmInfo.dwMajor==5){
				rcFile(pAlarmer,struAcsAlarmInfo);
				// printf("报警主类型-%d-报警次类型-%d-",struAcsAlarmInfo.dwMajor,struAcsAlarmInfo.dwMinor);
				// printf("报警时间-%d:%d:%d-",struAcsAlarmInfo.struTime.dwHour,struAcsAlarmInfo.struTime.dwMinute,struAcsAlarmInfo.struTime.dwSecond);
				// printf("卡号-%s-读卡器编号-%d-门编号-%d\n", struAcsAlarmInfo.struAcsEventInfo.byCardNo,struAcsAlarmInfo.struAcsEventInfo.dwCardReaderNo,struAcsAlarmInfo.struAcsEventInfo.dwDoorNo);
				// printf("————————————————————————————————————————————————————\n");
				printf("\n");
			}
		 	break;
		}
		case COMM_ID_INFO_ALARM://门禁身份证刷卡信息 
		{
			// 改%类型
			printf("————门禁身份证刷卡信息————COMM_ID_INFO_ALARM:%d\n", COMM_ID_INFO_ALARM);
		 	NET_DVR_ID_CARD_INFO_ALARM struID_CardInfo = {0}; 
		 	memcpy(&struID_CardInfo, pAlarmInfo, sizeof(struID_CardInfo));    //按需处理报警信息结构体中的信息...... 
		 	printf("struID_CardInfo.dwMajor：%d\n", struID_CardInfo.dwMajor );
		 	printf("struID_CardInfo.dwMinor%d\n", struID_CardInfo.dwMinor );
		 	// printf("struID_CardInfo.struSwipeTime  :%d,%d,%d\n", struID_CardInfo.struSwipeTime.dwHour,struID_CardInfo.struSwipeTime.dwMinute,struID_CardInfo.struSwipeTime.dwSecond);
		 	printf("struID_CardInfo.byNetUser  :%s\n", struID_CardInfo.byNetUser  );
			printf("struID_CardInfo.struRemoteHostAddr :%s\n", struID_CardInfo.struRemoteHostAddr);
			printf("struID_CardInfo.dwCardReaderNo  :%s\n", struID_CardInfo.dwCardReaderNo );
			printf("struID_CardInfo.dwDoorNo   :%s\n", struID_CardInfo.dwDoorNo  );

		 	break;
		}  
		case COMM_PASSNUM_INFO_ALARM://门禁通行人数信息 
		{ 
			printf("————门禁通行人数信息————COMM_PASSNUM_INFO_ALARM:%d\n", COMM_PASSNUM_INFO_ALARM);
			NET_DVR_PASSNUM_INFO_ALARM struPassnumInfo = {0}; 
			memcpy(&struPassnumInfo, pAlarmInfo, sizeof(struPassnumInfo));    //按需处理报警信息结构体中的信息...... 
			break;
		} 
		default: 
			printf("————default————\n");
		 	break; 
	} 
	/*
	printf("————门禁通行人数信息————\n");
	NET_DVR_PASSNUM_INFO_ALARM struID_CardInfo = {0}; 
	memcpy(&struID_CardInfo, pAlarmInfo, sizeof(struID_CardInfo));    //按需处理报警信息结构体中的信息...... 
	printf("结构体大小：%d\n",struID_CardInfo.dwSize);
	printf("人员通道号：%d\n",struID_CardInfo.dwAccessChannel);
	printf("刷卡时间：%d,%d,%d\n",struID_CardInfo.struSwipeTime.byHour,struID_CardInfo.struSwipeTime.byMinute,struID_CardInfo.struSwipeTime.bySecond);
	printf("网络操作的用户名：%s\n",struID_CardInfo.byNetUser);
	printf("远程主机地址：%d\n",struID_CardInfo.struRemoteHostAddr);
	printf("人员进入次数：%d\n",struID_CardInfo.dwEntryTimes);
	printf("人员离开次数：%d\n",struID_CardInfo.dwExitTimes);
	printf("人员出入总次数：%d\n",struID_CardInfo.dwTotalTimes);	*/		

	return true; 
} 
int main(int argc,char *argv[]) 
{ 
	printf("————1————\n");
 	//---------------------------------------  //初始化 
 	NET_DVR_Init(); 
 	  //获取控制台窗口句柄 
 	// HMODULE hKernel32 = GetModuleHandle("kernel32"); 
 	// GetConsoleWindow = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow"); 
 	  //设置连接时间与重连时间 
 	NET_DVR_SetConnectTime(2000, 1); 
 	NET_DVR_SetReconnect(10000, true); 
 	 
 	 printf("————2————\n");
 	//---------------------------------------  //注册设备 
 	LONG lUserID; 
 	NET_DVR_DEVICEINFO_V30 struDeviceInfo; 
 	lUserID = NET_DVR_Login_V30(argv[1], 8000, "admin", "admin123", &struDeviceInfo); 
 	printf("————3————\n");
 	if (lUserID < 0) 
 	{ 
 		printf("Login error, %d\n", NET_DVR_GetLastError()); 
 		NET_DVR_Cleanup();  
 		return 0; 
 	} 
 	// printf("lUserID%d\n",lUserID);	//0
 	// printf("序列号:%s\n",struDeviceInfo.sSerialNumber);//DS-K2601...

 	//设置报警回调函数 
 	NET_DVR_SetDVRMessageCallBack_V31(MSesGCallback, NULL);  //启用布防 
 	NET_DVR_SETUPALARM_PARAM struSetupParam={0}; 
 	struSetupParam.dwSize=sizeof(NET_DVR_SETUPALARM_PARAM); 
 	printf("————4————\n");
 	LONG  lHandle = NET_DVR_SetupAlarmChan_V41(lUserID,&struSetupParam); 
 	if (lHandle < 0) 
 	{ 
 		printf("NET_DVR_SetupAlarmChan_V41 error, %d\n", NET_DVR_GetLastError()); 
 		NET_DVR_Logout(lUserID);
 		NET_DVR_Cleanup();
 		return 0;
 	} 

 	// printf("准备sleep...\n");
 	  //等待 30s 
 	//sleep(30000);  //撤销布防 
	while(1){	
		sleep(2);
	}

 	/*
 	if (!NET_DVR_CloseAlarmChan_V30(lHandle)) 
 	{ 
 		printf("NET_DVR_CloseAlarmChan_V30 error, %d\n", NET_DVR_GetLastError()); 
 		NET_DVR_Logout(lUserID); 
 		NET_DVR_Cleanup();  
 		return 0; 
 	}  //注销用户 
 	NET_DVR_Logout(lUserID);  //释放 SDK 资源 
 	NET_DVR_Cleanup(); */
 	return 0; 
} 
