#include <conio.h>
#include <winsock2.h>
//#include <Windows.h>
#include <Wincon.h>
#include <stdio.h>
//
#include "../gen-cpp/ZxTradeSdk.h"
#include "../zx-sdk/CITICs_HsT2Hlp.h"
//
#include <nb30.h>
//
#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <list>
#include <vector>
#include <set>
#include <map>

using namespace com::autotrade::zxtradesdk;
using namespace std;


typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER NameBuff [30];
}ASTAT, * PASTAT;

class ZxHsSdk {
public:
	ZxHsSdk(const char* cfg_path = NULL,const char* ip = NULL,const char* mac = NULL){
		//
		m_strOpEntrustWay = "3";
		m_strClientName = "ZXZQ";
		m_HsHlpHandle = NULL;
		m_HsHlpCfg = NULL;
		//
		if(NULL != cfg_path){
			m_strCfgFilePath = cfg_path;
		}else{
			m_strCfgFilePath = "Hsconfig.ini";
		}
		//
		if(NULL != ip){
			m_strIP = ip;
		}
		if(NULL != mac){
			m_strMAC = mac;
		}
	}
	bool get_local_ip(string& ip);
	bool get_first_mac (string& mac);
	int  get_err_msg(int iFunc,string& msg);
	int  get_ans_data(ZxTradeAnsData& ans_data);
	void set_necessary_param(const char* num = "10");
	//
	int init();
	int re_init();
	int fini();
	int login(string username,string password,ZxTradeAnsData& ans_data);
	int search_stockholder(ZxTradeAnsData& ans_data);
	int search_fund_msg(ZxTradeAnsData& ans_data);
	int run_normal_entrust(string stock_code,string shorsz,string num,string price,string buyorsell,ZxTradeAnsData& ans_data);
	int serach_entrust_status(string entrust_no,ZxTradeAnsData& ans_data);
	int run_undo_entrust(string entrust_no,ZxTradeAnsData& ans_data);
	virtual ~ZxHsSdk() {
		fini();
	}

private:
	//---------------------------------------------------------------------
	//���������Ϣ
	//---------------------------------------------------------------------
	string m_strFundAccount;//�����ʽ��˺�		
	string m_strPassword;//�����ʽ�����
	string m_strOpEntrustWay;//�ⲿ����ͻ�������3��Զ��ί�У�����������Ҫ��Ӫҵ����ʹ�õ��ʽ��˺ſ�ͨ3Ȩ��
	string m_strClientName; //�ͻ���ƣ�һ����ƴ������ĸ,������޸ģ�
	//---------------------------------------------------------------------
	string m_strCfgFilePath;
	//--------------------------------------------------------------------
	string m_strClientId;	
	string m_strUserToken;
	string m_strBranchNo;
	string m_strAssetProp;
	string m_strSysNodeId;
	string m_strEntrustNo;
	string m_strEntrustStatus;
	string m_strOpStation;
	map<string,string> m_mapStockAccount;
	string m_strIP;
	string m_strMAC;
	string m_strHD;
	//
	HSHLPHANDLE m_HsHlpHandle;
	HSHLPCFGHANDLE m_HsHlpCfg;
};