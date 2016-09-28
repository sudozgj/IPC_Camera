#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "HCNetSDK.h"

/*
Parameters
	lUserID 
		[in] NET_DVR_Login_V40等登录接口的返回值 
	lGatewayIndex 
		[in] 门禁序号，从1开始，-1表示对所有门进行操作 
	dwStaic 
		[in] 命令值：0- 关闭，1- 打开，2- 常开，3- 常关

Return Values
	TRUE表示成功，FALSE表示失败。获取错误码调用NET_DVR_GetLastError

Remarks
	门禁控制功能使用的是该接口，报警主机的辅助功能控制中的电锁、移动门控制通过接口NET_DVR_AlarmHostAssistantControl实现。

*/

int main(int argc,char *argv[])
{

	//初始化
	NET_DVR_Init();
	
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//---------------------------------------
	//注册设备
	LONG lUserID;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	lUserID = NET_DVR_Login_V30("192.168.1.80", 8000, "admin", "admin123", &struDeviceInfo);
	if (lUserID < 0)
	{
		//这里根据后台需要，可以打印标志ERROR表示登陆错误
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return 0;
	}

	DWORD dwStatic = atoi(argv[2]); 

	printf("————门禁远程控制开始————\n");
	LONG lGatewayIndex ;
	sscanf(argv[1],"%ld",&lGatewayIndex);
	BOOL Ctl;
	Ctl = NET_DVR_ControlGateway (lUserID, lGatewayIndex, dwStatic);
	if (!Ctl)
	{
		//这里根据后台需要，可以打印标志ERROR表示非法输入
		printf("	控制失败, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		NET_DVR_Logout(lUserID);
		return 0;
	}
	switch(dwStatic){
		case 0:
			printf("	门操作：关闭\n");
			break;
		case 1:
			printf("	门操作：打开\n");
			break;
		case 2:
			printf("	门操作：常开\n");
			break;
		case 3:
			printf("	门操作：常闭\n");
			break;
	}

	printf("————门禁远程控制结束————\n");
	//注销用户
	NET_DVR_Logout(lUserID);
	//释放 SDK 资源
	NET_DVR_Cleanup();
	return 0;
}