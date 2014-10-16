//����֤ȯ���׽ӿڵ���ʾ����
//�籨��Get machine ID failed����,�뻻�ù���Ա������С�
//�籨"The license infomation is error"������ݴ�����ʾ����cslog/cserror.log�ļ��н� machine ID �������������͵�zhuangf@citics.com��Ȩ��
//�����������⣬�ɽ�cslog/����.log�ļ���������������ҳ���������������������͸�zhuangf@citics.com


#include "ZxHsSdk.h"
#include <boost/format.hpp>

using namespace std;

#pragma comment(lib,"Netapi32.lib")
#pragma comment(lib, "CITICs_HsT2Hlp.lib")
#pragma comment(lib,"wsock32.lib")

//��ȡ����IP��
bool ZxHsSdk::get_local_ip(string& ip)
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
	ip.append(inet_ntoa(*(in_addr*)*host->h_addr_list));
	return true;
}
//�ú���ֻ��ȡ��һ��������mac��ַ�����ж��������������Ӧ�޸ġ�
bool ZxHsSdk::get_first_mac (string& mac)
{
	ASTAT Adapter;
	NCB ncb;
	//
	memset( &ncb, 0, sizeof(ncb) );
	memset( &Adapter, 0, sizeof(Adapter) );
	//
	ncb.ncb_command = NCBRESET;
	ncb.ncb_lana_num = 0;
	// ���ȶ�ѡ������������һ��NCBRESET����Ա���г�ʼ��
	UCHAR uRetCode = Netbios( &ncb );
	memset( &ncb, 0, sizeof(ncb) );
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = 0; // ָ��������
	//
	strcpy( (char *)ncb.ncb_callname, "* " );
	ncb.ncb_buffer = (unsigned char *) &Adapter;
	// ָ�����ص���Ϣ��ŵı���
	ncb.ncb_length = sizeof(Adapter);
	// ���ţ����Է���NCBASTAT�����Ի�ȡ��������Ϣ
	uRetCode = Netbios( &ncb );
	if( uRetCode == 0 ){
		boost::format fmter = boost::format("%02X%02X%02X%02X%02X%02X");
		fmter % Adapter.adapt.adapter_address[0];
		fmter % Adapter.adapt.adapter_address[1];
		fmter % Adapter.adapt.adapter_address[2];
		fmter % Adapter.adapt.adapter_address[3];
		fmter % Adapter.adapt.adapter_address[4];
		fmter % Adapter.adapt.adapter_address[5];
		//
		mac = fmter.str();
	}
	return uRetCode==0;
}
int ZxHsSdk::init(){
	//
	char szMsg[512];
	//
	if(m_strIP.length() == 0){
		if(!get_local_ip(m_strIP)){
			printf("��ȡ����IPCʧ�ܣ����ֶ���������IP����Ӧ�ַ�����\n");
		}
	}
	if(m_strMAC.length() == 0){
		if(!get_first_mac(m_strMAC)){
			printf("��ȡ����MACʧ�ܣ����ֶ����뱾��MAC����Ӧ�ַ�����\n");
		}
	}
	//���ݽ������涨��op_station��Ҫ�ϸ������й�����д
	m_strOpStation.append("TYJR-");//Ĭ��ǰ׺���벻Ҫ�޸�
	m_strOpStation.append(m_strClientName);
	m_strOpStation.append(" IP.");
	m_strOpStation.append(m_strIP);
	m_strOpStation.append(" MAC.");
	m_strOpStation.append(m_strMAC);
	// ���������ļ�
	int iRet = CITICs_HsHlp_LoadConfig(&m_HsHlpCfg, m_strCfgFilePath.c_str());
	if (iRet){
		printf("���������ļ�ʧ��[Hsconfig.ini] ErrorCdoe=(%d)....\n", iRet);
		return ZX_TRADE_ERROR_CODE::ZX_TRADE_LOAD_CONFIG_FILE_ERROR;
	}
	// ����ʹ�ò�ͬ�����þ����ʼ�����Ӿ���������������ӵ���ͬ��ͨ���м��
	iRet = CITICs_HsHlp_Init(&m_HsHlpHandle, m_HsHlpCfg); 
	if (iRet){
		CITICs_HsHlp_GetErrorMsg(m_HsHlpHandle, NULL, szMsg);
		printf("��ʼ�����Ӿ��ʧ�� ErrorCdoe=(%d)....\n", iRet);
		return ZX_TRADE_ERROR_CODE::ZX_TRADE_INIT_CONN_HANDLE_ERROR;
	}
	// ���ӷ�����
	iRet = CITICs_HsHlp_ConnectServer(m_HsHlpHandle);
	if (iRet){ 
		CITICs_HsHlp_GetErrorMsg(m_HsHlpHandle, NULL, szMsg);
		printf("����Serverʧ��: ErrorCdoe=(%d) %s....", iRet, szMsg);
		return ZX_TRADE_ERROR_CODE::ZX_TRADE_CONN_SERVER_ERROR;
	}else {
		printf("Connect HSAR OK......\n");
	}
	return ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
}
int ZxHsSdk::fini(){
	if (NULL != m_HsHlpHandle){
		CITICs_HsHlp_DisConnect(m_HsHlpHandle);
		CITICs_HsHlp_Exit(m_HsHlpHandle);
		cout << "fini call CITICs_HsHlp_DisConnect() " << endl;
	}
	m_HsHlpHandle = NULL;
	cout << "fini ok." << endl;
	return ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
}
int ZxHsSdk::get_err_msg(int iFunc,string& msg)
{
	int nErr = 0;
	char szMsg[512] = {0};
	CITICs_HsHlp_GetErrorMsg(m_HsHlpHandle, &nErr, szMsg);
	printf("����ҵ��ʧ��[%d]: Error=(%d) %s\n", iFunc, nErr, szMsg);
	msg = szMsg;
	//
	return nErr;
}
//��ӡ����ֵ������������ĵ���һ�µ�������Բ��Խ��Ϊ׼��
int ZxHsSdk::get_ans_data(ZxTradeAnsData& ans_data)
{
	char szKey[64] = {0};
	char szValue[512] = {0};
	//
	int iRow =CITICs_HsHlp_GetRowCount(m_HsHlpHandle);
	int iCol =CITICs_HsHlp_GetColCount(m_HsHlpHandle);

	for(int i=0; i<iRow; i++){
		if(0 == CITICs_HsHlp_GetNextRow(m_HsHlpHandle)){
			printf("\n[%03d]:------------------------------------\n", i+1);
			map<string,string> cont;
			for(int j=0; j<iCol; j++){
				CITICs_HsHlp_GetColName(m_HsHlpHandle,j, szKey);
				CITICs_HsHlp_GetValueByIndex(m_HsHlpHandle, j, szValue);
				//CITICs_HsHlp_GetValue(HlpHandle, szKey, szValue);
				printf("%s:%s, ", szKey, szValue);
				cont.insert(make_pair(szKey,szValue));
			}
			ans_data.ans_data.push_back(cont);
		}
	}
	printf("\n");
	return 0;
}
//������������˳���½�����б��Ķ���Ҫ�͵Ĳ�����
void ZxHsSdk::set_necessary_param(const char* num){
	CITICs_HsHlp_BeginParam(m_HsHlpHandle);
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "client_id", m_strClientId.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "fund_account", m_strFundAccount.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "sysnode_id", m_strSysNodeId.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "identity_type", "2");	
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "op_branch_no", m_strBranchNo.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "branch_no", m_strBranchNo.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "op_station", m_strOpStation.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "op_entrust_way", m_strOpEntrustWay.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "password_type", "2");
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "password", m_strPassword.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "asset_prop", m_strAssetProp.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "user_token", m_strUserToken.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "request_num", num);//���ֵ���������ķ���������������ͣ���Ĭ��Ϊ50
	return ;
}
int ZxHsSdk::login(string account,string pwd, ZxTradeAnsData& ans_data){
	printf("\n\n---===�ͻ���¼Login===---");
	//��ע���������ʹ�ᶳ���˻���ʮ���Ӻ����Լ��ɡ�
	m_strFundAccount = account;
	m_strPassword = pwd;
	//
	int iFunc = 331100;
	CITICs_HsHlp_BeginParam(m_HsHlpHandle);
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "identity_type", "2");
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "password_type", "2");
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "input_content", "1");
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "op_entrust_way", m_strOpEntrustWay.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "password", m_strPassword.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "account_content", m_strFundAccount.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "op_station", m_strOpStation.c_str());
	int iRet = CITICs_HsHlp_BizCallAndCommit(m_HsHlpHandle, iFunc, NULL);
	if (iRet){
		string msg;
		iRet = get_err_msg(iFunc,msg);
		//
		ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_ACCOUNT_LOGIN_ERROR;
		ans_data.error_msg = msg;
		//
		return ZX_TRADE_ERROR_CODE::ZX_TRADE_ACCOUNT_LOGIN_ERROR;
	}
	//����½�ķ���ֵ�����������Ժ����еı��Ķ���Ҫ
	char client_id[32]     = {0};	
	char user_token[64]    = {0};
	char branch_no[8]      = {0};
	char asset_prop[4]     = {0};
	char sysnode_id[4]     = {0};
	CITICs_HsHlp_GetValue(m_HsHlpHandle, "client_id", client_id);
	CITICs_HsHlp_GetValue(m_HsHlpHandle, "user_token", user_token);
	CITICs_HsHlp_GetValue(m_HsHlpHandle, "branch_no", branch_no);
	CITICs_HsHlp_GetValue(m_HsHlpHandle, "asset_prop", asset_prop);
	CITICs_HsHlp_GetValue(m_HsHlpHandle, "sysnode_id", sysnode_id);
	m_strClientId = client_id;
	m_strUserToken = user_token;
	m_strBranchNo = branch_no;
	m_strAssetProp = asset_prop;
	m_strSysNodeId = sysnode_id;
	//
	iRet = get_ans_data(ans_data);
	//
	ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
	ans_data.error_msg = "SUCCESS";
	//
	return ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
}

int ZxHsSdk::search_stockholder(ZxTradeAnsData& ans_data){
	/// ��ѯ�ͻ��ɶ���Ϣ
	printf("\n\n---===��ѯ�ͻ��ɶ���Ϣ===---");
	int iFunc = 331300; 
	set_necessary_param();
	int iRet = CITICs_HsHlp_BizCallAndCommit(m_HsHlpHandle, iFunc, NULL);
	if (iRet){
		string msg;
		iRet = get_err_msg(iFunc,msg);
		//
		ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_SEARCH_STOCKHOLDER_ERROR;
		ans_data.error_msg = msg;
		//
		return ZX_TRADE_ERROR_CODE::ZX_TRADE_SEARCH_STOCKHOLDER_ERROR;
	}
	//
	iRet = get_ans_data(ans_data);
	//
	typedef std::vector<std::map<std::string,std::string>>  VEC_ANS_DATA;
	typedef std::map<std::string,std::string>  MAP_ANS_DATA;
	VEC_ANS_DATA::iterator  vec_iter;
	MAP_ANS_DATA::iterator  map_iter;
	for(vec_iter = ans_data.ans_data.begin(); vec_iter != ans_data.ans_data.end(); vec_iter++){
		string account;
		string exchange_type;
		for(map_iter = vec_iter->begin(); map_iter != vec_iter->end(); map_iter++){
			if(map_iter->first == "stock_account"){
				account = map_iter->second;
			}
			if(map_iter->first == "exchange_type"){
				exchange_type = map_iter->second;
			}
		}
		m_mapStockAccount.insert(make_pair(exchange_type,account));
	}
	//
	ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
	ans_data.error_msg = "SUCCESS";
	//
	return ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
}
int ZxHsSdk::search_fund_msg(ZxTradeAnsData& ans_data){
	//////////////////////////////////////////////////////////////////////////
	/// ��ѯ�ʽ���Ϣ
	printf("\n\n---===��ѯ�ʽ���Ϣ===---");
	int iFunc = 332255;
	set_necessary_param();
	int iRet = CITICs_HsHlp_BizCallAndCommit(m_HsHlpHandle, iFunc, NULL);
	if (iRet){
		string msg;
		iRet = get_err_msg(iFunc,msg);
		//
		ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_SEARCH_FUND_MSG_ERROR;
		ans_data.error_msg = msg;
		//
		return ZX_TRADE_ERROR_CODE::ZX_TRADE_SEARCH_FUND_MSG_ERROR;
	}
	//
	iRet = get_ans_data(ans_data);
	//
	ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
	ans_data.error_msg = "SUCCESS";
	//
	return ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
}
int ZxHsSdk::run_normal_entrust(string stock_code,string exchange_type,string num,string price,string buyorsell,ZxTradeAnsData& ans_data){
	
	//
	printf("\n\n---===��ͨί��===---");
	int iFunc = 333002;
	set_necessary_param();
	//
	typedef std::map<string,string> MAP_ACCOUNT;
	MAP_ACCOUNT::iterator iter = m_mapStockAccount.find(exchange_type);
	if(iter == m_mapStockAccount.end()){
		//
		ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_USE_EXCHANGE_TYPE_NOT_FIND_ACCOUNT_ERROR;
		ans_data.error_msg = "USE EXCHANGE_TYPE NOT FIND ACCOUNT !!!";
		//
		return ZX_TRADE_ERROR_CODE::ZX_TRADE_USE_EXCHANGE_TYPE_NOT_FIND_ACCOUNT_ERROR;
	}
	//
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "stock_account", (iter->second).c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "exchange_type", exchange_type.c_str());//1��ʾ�Ϻ�A�ɣ�2��ʾ����A��
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "stock_code", stock_code.c_str());//��Ʊ����
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "entrust_amount", num.c_str());//����
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "entrust_price", price.c_str());//�۸�
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "entrust_bs", buyorsell.c_str());//1�� 2��
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "entrust_prop", "0");// ��ͨ����
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "batch_no", "0");//0��ʾ���ʶ���
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "entrust_type", "0");
	//		
	int iRet = CITICs_HsHlp_BizCallAndCommit(m_HsHlpHandle, iFunc, NULL);
	if (iRet){
		string msg;
		iRet = get_err_msg(iFunc,msg);
		//
		ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_RUN_NORMAL_ENTRUST_ERROR;
		ans_data.error_msg = msg;
		//
		return ZX_TRADE_ERROR_CODE::ZX_TRADE_RUN_NORMAL_ENTRUST_ERROR;
	}
	//
	iRet = get_ans_data(ans_data);
	//
	ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
	ans_data.error_msg = "SUCCESS";
	//
	return ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
}
int ZxHsSdk::serach_entrust_status(string entrust_no,ZxTradeAnsData& ans_data){
	printf("\n\n---===��ѯί��״̬===---");
	int iFunc = 333101;
	set_necessary_param();
	//
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "locate_entrust_no", entrust_no.c_str());
	//
	int iRet = CITICs_HsHlp_BizCallAndCommit(m_HsHlpHandle, iFunc, NULL);
	if (iRet){
		string msg;
		iRet = get_err_msg(iFunc,msg);
		//
		ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_SERACH_ENTRUST_STATUS_ERROR;
		ans_data.error_msg = msg;
		//
		return ZX_TRADE_ERROR_CODE::ZX_TRADE_SERACH_ENTRUST_STATUS_ERROR;
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
	//
	iRet = get_ans_data(ans_data);
	//
	ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
	ans_data.error_msg = "SUCCESS";
	//
	return ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
};
int ZxHsSdk::run_undo_entrust(string entrust_no,ZxTradeAnsData& ans_data){
	// ������ʾ����ί�У�ֻ��entrust_statusΪ2��7ʱ���ܳ�������Ȼ�ᱨ��ί��״̬����
	printf("\n\n---===ί�г���===---");
	int iFunc = 333017;
	set_necessary_param();
	//
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "entrust_no", entrust_no.c_str());
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "batch_flag", "0");// 0�����ʳ���
	CITICs_HsHlp_SetValue(m_HsHlpHandle, "locate_entrust_no", entrust_no.c_str());
	int iRet = CITICs_HsHlp_BizCallAndCommit(m_HsHlpHandle, iFunc, NULL);
	if (iRet){
		string msg;
		iRet = get_err_msg(iFunc,msg);
		//
		ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_RUN_UNDO_ENTRUST_ERROR;
		ans_data.error_msg = msg;
		//
		return ZX_TRADE_ERROR_CODE::ZX_TRADE_RUN_UNDO_ENTRUST_ERROR;
	}
	//
	iRet = get_ans_data(ans_data);
	//
	ans_data.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
	ans_data.error_msg = "SUCCESS";
	//
	return ZX_TRADE_ERROR_CODE::ZX_TRADE_OK;
}







