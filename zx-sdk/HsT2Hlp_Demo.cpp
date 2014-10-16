//中信证券交易接口调用示例。
//如报“Get machine ID failed！”,请换用管理员身份运行。
//如报"The license infomation is error"，请根据错误提示，到cslog/cserror.log文件中将 machine ID 拷贝出来，发送到zhuangf@citics.com授权。
//如有其他问题，可将cslog/日期.log文件中有问题的那行找出来，将问题描述清楚发送给zhuangf@citics.com

#include <string.h>
#include <conio.h>
#include <Windows.h>
#include <Wincon.h>
#include <stdio.h>
#include "CITICs_HsT2Hlp.h"
#include <nb30.h>

#pragma comment(lib,"Netapi32.lib")
#pragma comment(lib, "CITICs_HsT2Hlp.lib")
#pragma comment(lib,"wsock32.lib")

void ShowAnsData(HSHLPHANDLE HlpHandle);
void ShowErrMsg(HSHLPHANDLE HlpHandle, int iFunc);
HSHLPHANDLE Run();
void SetNecessaryParam(HSHLPHANDLE HlpHandle);
bool GetLocalIP(char* ip);
bool GetFirstMac (char * mac);

const char fund_account[32]  = "600511005";//测试资金账号
//const char fund_account[32]  = "8100000013";//测试快速交易资金账号
const char password[32]      ="606869";//测试资金密码
const char op_entrust_way[4] = "3";//外部接入客户必须用3（远程委托），生产后需要让营业部对使用的资金账号开通3权限
const char ClientName[16] = "ZXZQ"; //客户简称，一般用拼音首字母,请务必修改！

char client_id[32]     = {0};	
char user_token[64]    = {0};
char branch_no[8]      = {0};
char asset_prop[4]     = {0};
char sysnode_id[4]     = {0};
char entrust_no[16]    = {0};
char entrust_status[8] = {0};
char op_station[128]    = {0};
char stock_account[128]    = {0};
char IP[16]    = {0};
char MAC[16]    = {0};
char HD[16]    = {0};

HSHLPHANDLE HlpHandle = NULL;

int run_zxsdk_init(){
	int iRet;
	char szMsg[512];

	HSHLPCFGHANDLE hConfig1; 
//	HSHLPCFGHANDLE hConfig2;

	int iFunc;

	if(!GetLocalIP(IP))
	{
		printf("获取本机IPC失败，请手动填入内网IP至对应字符串。\n");
	}
	if(!GetFirstMac(MAC))
	{
		printf("获取本机MAC失败，请手动填入本机MAC至对应字符串。\n");
	}

	//根据交易所规定，op_station需要严格按照下列规则填写
	strcat(op_station,"TYJR-");//默认前缀，请不要修改
	strcat(op_station,ClientName);
	strcat(op_station," IP.");
	strcat(op_station,IP);
	strcat(op_station," MAC.");
	strcat(op_station,MAC);
	
	iRet = CITICs_HsHlp_LoadConfig(&hConfig1, "Hsconfig.ini");
	if (iRet)
	{
		printf("加载配置文件失败[Hsconfig.ini] ErrorCdoe=(%d)....\n", iRet);
		return -1;
	}
	
	/// 可以加载多个配置文件，初始化多个配置文件句柄
	/*iRet = CITICs_HsHlp_LoadConfig(&hConfig2, ".\\Hsconfig2.ini");
	if (iRet)
	{
		printf("加载配置文件失败[Hsconfig2.ini] ErrorCdoe=(%d)....\n", iRet);
		//goto __RET;
	}*/


	/// 可以使用不同的配置句柄初始化连接句柄，这样可以连接到不同的通信中间件
	iRet = CITICs_HsHlp_Init(&HlpHandle, hConfig1); 
	if (iRet)
	{
		CITICs_HsHlp_GetErrorMsg(HlpHandle, NULL, szMsg);
		printf("初始化连接句柄失败 ErrorCdoe=(%d)....\n", iRet);
		return -2;
	}
	
	/// 连接服务器
	iRet = CITICs_HsHlp_ConnectServer(HlpHandle);
	if (iRet)
	{ 
		CITICs_HsHlp_GetErrorMsg(HlpHandle, NULL, szMsg);
		printf("连接Server失败: ErrorCdoe=(%d) %s....", iRet, szMsg);
		return -3;
	}
	else 
	{
		printf("Connect HSAR OK......\n");
	}
	return 0;
}
int run_zxsdk_fini(){
	if (HlpHandle!=NULL)
	{
		CITICs_HsHlp_DisConnect(HlpHandle);
		CITICs_HsHlp_Exit(HlpHandle);
	}
	HlpHandle = NULL;
	return 0;
}

int run_zxsdk_test()
{
	int iRet = 0;
	int iFunc = 0;
	printf("\n\n---===客户登录Login===---");
	//备注：密码多次送错会冻结账户，十分钟后再试即可。
	iFunc = 331100;
	CITICs_HsHlp_BeginParam(HlpHandle);
	CITICs_HsHlp_SetValue(HlpHandle, "identity_type", "2");
	CITICs_HsHlp_SetValue(HlpHandle, "password_type", "2");
	CITICs_HsHlp_SetValue(HlpHandle, "input_content", "1");
	CITICs_HsHlp_SetValue(HlpHandle, "op_entrust_way", op_entrust_way);
	CITICs_HsHlp_SetValue(HlpHandle, "password", password);
	CITICs_HsHlp_SetValue(HlpHandle, "account_content", fund_account);
	CITICs_HsHlp_SetValue(HlpHandle, "op_station", op_station);
	iRet = CITICs_HsHlp_BizCallAndCommit(HlpHandle, iFunc, NULL);
	if (iRet)
	{
		ShowErrMsg(HlpHandle, iFunc);
		return -4;
	}

	//将登陆的返回值保存下来，以后所有的报文都需要
	CITICs_HsHlp_GetValue(HlpHandle, "client_id", client_id);
	CITICs_HsHlp_GetValue(HlpHandle, "user_token", user_token);
	CITICs_HsHlp_GetValue(HlpHandle, "branch_no", branch_no);
	CITICs_HsHlp_GetValue(HlpHandle, "asset_prop", asset_prop);
	CITICs_HsHlp_GetValue(HlpHandle, "sysnode_id", sysnode_id);

	ShowAnsData(HlpHandle);
	
	/// 查询客户股东信息
	printf("\n\n---===查询客户股东信息===---");
	iFunc = 331300; 
	SetNecessaryParam(HlpHandle);
	iRet = CITICs_HsHlp_BizCallAndCommit(HlpHandle, iFunc, NULL);
	if (iRet)
	{
		ShowErrMsg(HlpHandle, iFunc);
	 	return -5;
	}
	ShowAnsData(HlpHandle);
	//这里以取上海A股股东为例，实际生产中需要根据下的委托所属市场来取相应的股东
	CITICs_HsHlp_GetValue(HlpHandle, "stock_account", stock_account);
	//////////////////////////////////////////////////////////////////////////
	/// 查询资金信息
	printf("\n\n---===查询资金信息===---");
	iFunc = 332255;
	SetNecessaryParam(HlpHandle);

	iRet = CITICs_HsHlp_BizCallAndCommit(HlpHandle, iFunc, NULL);
	if (iRet)
	{
		ShowErrMsg(HlpHandle, iFunc);
		return -6;
	}
	ShowAnsData(HlpHandle);

	//for  (int i=601717 ;i<=601717 ;i++)
	{
		char stock_code[7];
		//sprintf(stock_code,"%6d",i);

		printf("\n\n---===普通委托===---");
		iFunc = 333002;

		SetNecessaryParam(HlpHandle);
	
		//CITICs_HsHlp_SetValue(HlpHandle, "stock_account", "E013624198");
		//CITICs_HsHlp_SetValue(HlpHandle, "stock_account", stock_account);
		CITICs_HsHlp_SetValue(HlpHandle, "stock_account", "A127254532");
		CITICs_HsHlp_SetValue(HlpHandle, "exchange_type", "1");//1表示上海A股，2表示深圳A股
		CITICs_HsHlp_SetValue(HlpHandle, "stock_code", "600696");//股票代码
		CITICs_HsHlp_SetValue(HlpHandle, "entrust_amount", "100");//数量
		CITICs_HsHlp_SetValue(HlpHandle, "entrust_price", "100.00");//价格
		CITICs_HsHlp_SetValue(HlpHandle, "entrust_bs", "1");//1买 2卖
		CITICs_HsHlp_SetValue(HlpHandle, "entrust_prop", "0");// 普通买卖
		CITICs_HsHlp_SetValue(HlpHandle, "batch_no", "0");//0表示单笔订单
		CITICs_HsHlp_SetValue(HlpHandle, "entrust_type", "0");
		
		iRet = CITICs_HsHlp_BizCallAndCommit(HlpHandle, iFunc, NULL);
		
		if (iRet)
		{
			ShowErrMsg(HlpHandle, iFunc);
			//return HlpHandle;
		}

		//获取委托编号，用于下面查询委托状态
		CITICs_HsHlp_GetValue(HlpHandle, "entrust_no", entrust_no);

		ShowAnsData(HlpHandle);

	}
	
	

	//------------------------------
	// 查询委托状态
	printf("\n\n---===查询委托状态===---");
	iFunc = 333101;
	SetNecessaryParam(HlpHandle);
	CITICs_HsHlp_SetValue(HlpHandle, "locate_entrust_no", entrust_no);

	iRet = CITICs_HsHlp_BizCallAndCommit(HlpHandle, iFunc, NULL);
	if (iRet)
	{
		ShowErrMsg(HlpHandle, iFunc);
		return -7;
	}
	/**
	 *委托状态。'0'	未报
	  '1'	待报
	  '2'	已报
	  '3'	已报待撤
	  '4'	部成待撤
	  '5'	部撤
	  '6'	已撤
	  '7'	部成
	  '8'	已成
	  '9'	废单
	 */
	//目前测试环境撮合方式为仿交易所模式，买入委托高于市价即全部成交，低于则全部不成交。卖出相反。
	CITICs_HsHlp_GetValue(HlpHandle, "entrust_status", entrust_status);
	ShowAnsData(HlpHandle);

	//-----------------------------------------------------
	// 下面演示撤单委托，只有entrust_status为2或7时才能撤单，不然会报“委托状态错误”
	printf("\n\n---===委托撤单===---");
	iFunc = 333017;
	CITICs_HsHlp_BeginParam(HlpHandle);
	SetNecessaryParam(HlpHandle);
	CITICs_HsHlp_SetValue(HlpHandle, "entrust_no", entrust_no);
	CITICs_HsHlp_SetValue(HlpHandle, "batch_flag", "0");// 0：单笔撤单
	CITICs_HsHlp_SetValue(HlpHandle, "locate_entrust_no", entrust_no);
	iRet = CITICs_HsHlp_BizCallAndCommit(HlpHandle, iFunc, NULL);
	if (iRet)
	{
		ShowErrMsg(HlpHandle, iFunc);
		return -8;
	}

	ShowAnsData(HlpHandle);
	return 0; 

}



void ShowErrMsg(HSHLPHANDLE HlpHandle, int iFunc)
{
	int nErr =0;
	char szMsg[512]={0};

	CITICs_HsHlp_GetErrorMsg(HlpHandle, &nErr, szMsg);
	printf("请求业务失败[%d]: Error=(%d) %s\n", iFunc, nErr, szMsg);
}

//打印返回值。如果遇到和文档不一致的情况，以测试结果为准。
void ShowAnsData(HSHLPHANDLE HlpHandle)
{
	int iRow, iCol;
	char szKey[64], szValue[512];
	iRow =CITICs_HsHlp_GetRowCount(HlpHandle);
	iCol =CITICs_HsHlp_GetColCount(HlpHandle);
	
	for(int i=0; i<iRow; i++)
	{
		if(0 == CITICs_HsHlp_GetNextRow(HlpHandle))
		{
			printf("\n[%03d]:------------------------------------\n", i+1);
			for(int j=0; j<iCol; j++)
			{
				CITICs_HsHlp_GetColName(HlpHandle,j, szKey);
				CITICs_HsHlp_GetValueByIndex(HlpHandle, j, szValue);
				//CITICs_HsHlp_GetValue(HlpHandle, szKey, szValue);
				printf("%s:%s, ", szKey, szValue);
			}
		}
	}
	printf("\n");
}

//这个函数设置了除登陆外所有报文都需要送的参数。
void SetNecessaryParam(HSHLPHANDLE HlpHandle)
{
	CITICs_HsHlp_BeginParam(HlpHandle);
	CITICs_HsHlp_SetValue(HlpHandle, "client_id", client_id);
	CITICs_HsHlp_SetValue(HlpHandle, "fund_account", fund_account);
	CITICs_HsHlp_SetValue(HlpHandle, "sysnode_id", sysnode_id);
	CITICs_HsHlp_SetValue(HlpHandle, "identity_type", "2");	
	CITICs_HsHlp_SetValue(HlpHandle, "op_branch_no", branch_no);
	CITICs_HsHlp_SetValue(HlpHandle, "branch_no", branch_no);
	CITICs_HsHlp_SetValue(HlpHandle, "op_station", op_station);
	CITICs_HsHlp_SetValue(HlpHandle, "op_entrust_way", op_entrust_way);
	CITICs_HsHlp_SetValue(HlpHandle, "password_type", "2");
	CITICs_HsHlp_SetValue(HlpHandle, "password", password);
	CITICs_HsHlp_SetValue(HlpHandle, "asset_prop", asset_prop);
	CITICs_HsHlp_SetValue(HlpHandle, "user_token", user_token);
	CITICs_HsHlp_SetValue(HlpHandle, "request_num", "10");//这个值决定了最大的返回条数，如果不送，则默认为50
}

//获取本机IP，
bool GetLocalIP(char* ip)
{
	//1.初始化wsa
	WSADATA wsaData;
	int ret=WSAStartup(MAKEWORD(2,2),&wsaData);
	if (ret!=0)
	{
		return false;
	}
	//2.获取主机名
	char hostname[256];
	ret=gethostname(hostname,sizeof(hostname));
	if (ret==SOCKET_ERROR)
	{
		return false;
	}
	//3.获取主机ip
	HOSTENT* host=gethostbyname(hostname);
	if (host==NULL)
	{
		return false;
	}

	strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list));
	return true;
}

typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER NameBuff [30];
}ASTAT, * PASTAT;

ASTAT Adapter;

//该函数只获取第一个网卡的mac地址，如有多个网卡，请做相应修改。
bool GetFirstMac (char * mac)
{
	NCB ncb;
	UCHAR uRetCode;

	memset( &ncb, 0, sizeof(ncb) );
	ncb.ncb_command = NCBRESET;
	ncb.ncb_lana_num = 0;

	// 首先对选定的网卡发送一个NCBRESET命令，以便进行初始化
	uRetCode = Netbios( &ncb );
	memset( &ncb, 0, sizeof(ncb) );
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = 0; // 指定网卡号

	strcpy( (char *)ncb.ncb_callname, "* " );
	ncb.ncb_buffer = (unsigned char *) &Adapter;

	// 指定返回的信息存放的变量
	ncb.ncb_length = sizeof(Adapter);

	// 接着，可以发送NCBASTAT命令以获取网卡的信息
	uRetCode = Netbios( &ncb );

	if( uRetCode == 0 )
	{
		sprintf(mac, "%02X%02X%02X%02X%02X%02X",
			Adapter.adapt.adapter_address[0],
			Adapter.adapt.adapter_address[1],
			Adapter.adapt.adapter_address[2],
			Adapter.adapt.adapter_address[3],
			Adapter.adapt.adapter_address[4],
			Adapter.adapt.adapter_address[5] );
	}

	return uRetCode==0;
}