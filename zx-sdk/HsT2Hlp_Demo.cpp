//����֤ȯ���׽ӿڵ���ʾ����
//�籨��Get machine ID failed����,�뻻�ù���Ա������С�
//�籨"The license infomation is error"������ݴ�����ʾ����cslog/cserror.log�ļ��н� machine ID �������������͵�zhuangf@citics.com��Ȩ��
//�����������⣬�ɽ�cslog/����.log�ļ���������������ҳ���������������������͸�zhuangf@citics.com

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

const char fund_account[32]  = "600511005";//�����ʽ��˺�
//const char fund_account[32]  = "8100000013";//���Կ��ٽ����ʽ��˺�
const char password[32]      ="606869";//�����ʽ�����
const char op_entrust_way[4] = "3";//�ⲿ����ͻ�������3��Զ��ί�У�����������Ҫ��Ӫҵ����ʹ�õ��ʽ��˺ſ�ͨ3Ȩ��
const char ClientName[16] = "ZXZQ"; //�ͻ���ƣ�һ����ƴ������ĸ,������޸ģ�

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
		printf("��ȡ����IPCʧ�ܣ����ֶ���������IP����Ӧ�ַ�����\n");
	}
	if(!GetFirstMac(MAC))
	{
		printf("��ȡ����MACʧ�ܣ����ֶ����뱾��MAC����Ӧ�ַ�����\n");
	}

	//���ݽ������涨��op_station��Ҫ�ϸ������й�����д
	strcat(op_station,"TYJR-");//Ĭ��ǰ׺���벻Ҫ�޸�
	strcat(op_station,ClientName);
	strcat(op_station," IP.");
	strcat(op_station,IP);
	strcat(op_station," MAC.");
	strcat(op_station,MAC);
	
	iRet = CITICs_HsHlp_LoadConfig(&hConfig1, "Hsconfig.ini");
	if (iRet)
	{
		printf("���������ļ�ʧ��[Hsconfig.ini] ErrorCdoe=(%d)....\n", iRet);
		return -1;
	}
	
	/// ���Լ��ض�������ļ�����ʼ����������ļ����
	/*iRet = CITICs_HsHlp_LoadConfig(&hConfig2, ".\\Hsconfig2.ini");
	if (iRet)
	{
		printf("���������ļ�ʧ��[Hsconfig2.ini] ErrorCdoe=(%d)....\n", iRet);
		//goto __RET;
	}*/


	/// ����ʹ�ò�ͬ�����þ����ʼ�����Ӿ���������������ӵ���ͬ��ͨ���м��
	iRet = CITICs_HsHlp_Init(&HlpHandle, hConfig1); 
	if (iRet)
	{
		CITICs_HsHlp_GetErrorMsg(HlpHandle, NULL, szMsg);
		printf("��ʼ�����Ӿ��ʧ�� ErrorCdoe=(%d)....\n", iRet);
		return -2;
	}
	
	/// ���ӷ�����
	iRet = CITICs_HsHlp_ConnectServer(HlpHandle);
	if (iRet)
	{ 
		CITICs_HsHlp_GetErrorMsg(HlpHandle, NULL, szMsg);
		printf("����Serverʧ��: ErrorCdoe=(%d) %s....", iRet, szMsg);
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
	printf("\n\n---===�ͻ���¼Login===---");
	//��ע���������ʹ�ᶳ���˻���ʮ���Ӻ����Լ��ɡ�
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

	//����½�ķ���ֵ�����������Ժ����еı��Ķ���Ҫ
	CITICs_HsHlp_GetValue(HlpHandle, "client_id", client_id);
	CITICs_HsHlp_GetValue(HlpHandle, "user_token", user_token);
	CITICs_HsHlp_GetValue(HlpHandle, "branch_no", branch_no);
	CITICs_HsHlp_GetValue(HlpHandle, "asset_prop", asset_prop);
	CITICs_HsHlp_GetValue(HlpHandle, "sysnode_id", sysnode_id);

	ShowAnsData(HlpHandle);
	
	/// ��ѯ�ͻ��ɶ���Ϣ
	printf("\n\n---===��ѯ�ͻ��ɶ���Ϣ===---");
	iFunc = 331300; 
	SetNecessaryParam(HlpHandle);
	iRet = CITICs_HsHlp_BizCallAndCommit(HlpHandle, iFunc, NULL);
	if (iRet)
	{
		ShowErrMsg(HlpHandle, iFunc);
	 	return -5;
	}
	ShowAnsData(HlpHandle);
	//������ȡ�Ϻ�A�ɹɶ�Ϊ����ʵ����������Ҫ�����µ�ί�������г���ȡ��Ӧ�Ĺɶ�
	CITICs_HsHlp_GetValue(HlpHandle, "stock_account", stock_account);
	//////////////////////////////////////////////////////////////////////////
	/// ��ѯ�ʽ���Ϣ
	printf("\n\n---===��ѯ�ʽ���Ϣ===---");
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

		printf("\n\n---===��ͨί��===---");
		iFunc = 333002;

		SetNecessaryParam(HlpHandle);
	
		//CITICs_HsHlp_SetValue(HlpHandle, "stock_account", "E013624198");
		//CITICs_HsHlp_SetValue(HlpHandle, "stock_account", stock_account);
		CITICs_HsHlp_SetValue(HlpHandle, "stock_account", "A127254532");
		CITICs_HsHlp_SetValue(HlpHandle, "exchange_type", "1");//1��ʾ�Ϻ�A�ɣ�2��ʾ����A��
		CITICs_HsHlp_SetValue(HlpHandle, "stock_code", "600696");//��Ʊ����
		CITICs_HsHlp_SetValue(HlpHandle, "entrust_amount", "100");//����
		CITICs_HsHlp_SetValue(HlpHandle, "entrust_price", "100.00");//�۸�
		CITICs_HsHlp_SetValue(HlpHandle, "entrust_bs", "1");//1�� 2��
		CITICs_HsHlp_SetValue(HlpHandle, "entrust_prop", "0");// ��ͨ����
		CITICs_HsHlp_SetValue(HlpHandle, "batch_no", "0");//0��ʾ���ʶ���
		CITICs_HsHlp_SetValue(HlpHandle, "entrust_type", "0");
		
		iRet = CITICs_HsHlp_BizCallAndCommit(HlpHandle, iFunc, NULL);
		
		if (iRet)
		{
			ShowErrMsg(HlpHandle, iFunc);
			//return HlpHandle;
		}

		//��ȡί�б�ţ����������ѯί��״̬
		CITICs_HsHlp_GetValue(HlpHandle, "entrust_no", entrust_no);

		ShowAnsData(HlpHandle);

	}
	
	

	//------------------------------
	// ��ѯί��״̬
	printf("\n\n---===��ѯί��״̬===---");
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
	 *ί��״̬��'0'	δ��
	  '1'	����
	  '2'	�ѱ�
	  '3'	�ѱ�����
	  '4'	���ɴ���
	  '5'	����
	  '6'	�ѳ�
	  '7'	����
	  '8'	�ѳ�
	  '9'	�ϵ�
	 */
	//Ŀǰ���Ի�����Ϸ�ʽΪ�½�����ģʽ������ί�и����мۼ�ȫ���ɽ���������ȫ�����ɽ��������෴��
	CITICs_HsHlp_GetValue(HlpHandle, "entrust_status", entrust_status);
	ShowAnsData(HlpHandle);

	//-----------------------------------------------------
	// ������ʾ����ί�У�ֻ��entrust_statusΪ2��7ʱ���ܳ�������Ȼ�ᱨ��ί��״̬����
	printf("\n\n---===ί�г���===---");
	iFunc = 333017;
	CITICs_HsHlp_BeginParam(HlpHandle);
	SetNecessaryParam(HlpHandle);
	CITICs_HsHlp_SetValue(HlpHandle, "entrust_no", entrust_no);
	CITICs_HsHlp_SetValue(HlpHandle, "batch_flag", "0");// 0�����ʳ���
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
	printf("����ҵ��ʧ��[%d]: Error=(%d) %s\n", iFunc, nErr, szMsg);
}

//��ӡ����ֵ������������ĵ���һ�µ�������Բ��Խ��Ϊ׼��
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

//������������˳���½�����б��Ķ���Ҫ�͵Ĳ�����
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
	CITICs_HsHlp_SetValue(HlpHandle, "request_num", "10");//���ֵ���������ķ���������������ͣ���Ĭ��Ϊ50
}

//��ȡ����IP��
bool GetLocalIP(char* ip)
{
	//1.��ʼ��wsa
	WSADATA wsaData;
	int ret=WSAStartup(MAKEWORD(2,2),&wsaData);
	if (ret!=0)
	{
		return false;
	}
	//2.��ȡ������
	char hostname[256];
	ret=gethostname(hostname,sizeof(hostname));
	if (ret==SOCKET_ERROR)
	{
		return false;
	}
	//3.��ȡ����ip
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

//�ú���ֻ��ȡ��һ��������mac��ַ�����ж��������������Ӧ�޸ġ�
bool GetFirstMac (char * mac)
{
	NCB ncb;
	UCHAR uRetCode;

	memset( &ncb, 0, sizeof(ncb) );
	ncb.ncb_command = NCBRESET;
	ncb.ncb_lana_num = 0;

	// ���ȶ�ѡ������������һ��NCBRESET����Ա���г�ʼ��
	uRetCode = Netbios( &ncb );
	memset( &ncb, 0, sizeof(ncb) );
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = 0; // ָ��������

	strcpy( (char *)ncb.ncb_callname, "* " );
	ncb.ncb_buffer = (unsigned char *) &Adapter;

	// ָ�����ص���Ϣ��ŵı���
	ncb.ncb_length = sizeof(Adapter);

	// ���ţ����Է���NCBASTAT�����Ի�ȡ��������Ϣ
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