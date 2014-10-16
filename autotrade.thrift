namespace cpp com.autotrade.zxtradesdk
namespace php com.autotrade.zxtradesdk

enum ZX_TRADE_ERROR_CODE {
	ZX_TRADE_OK = 0,
	ZX_TRADE_LOAD_CONFIG_FILE_ERROR = 1,
	ZX_TRADE_INIT_CONN_HANDLE_ERROR = 2, 
	ZX_TRADE_CONN_SERVER_ERROR = 3,
	ZX_TRADE_ACCOUNT_LOGIN_ERROR = 4, 
	ZX_TRADE_SEARCH_STOCKHOLDER_ERROR = 5,
	ZX_TRADE_SEARCH_FUND_MSG_ERROR = 6,
	ZX_TRADE_RUN_NORMAL_ENTRUST_ERROR = 7,
	ZX_TRADE_SERACH_ENTRUST_STATUS_ERROR = 8,
	ZX_TRADE_RUN_UNDO_ENTRUST_ERROR = 9,
	ZX_TRADE_ACCOUNT_PWD_ERROR = 21,
	ZX_TRADE_OBJ_NOT_FIND_ERROR = 22,
	ZX_TRADE_USE_EXCHANGE_TYPE_NOT_FIND_ACCOUNT_ERROR = 23
}

struct ZxTradeAnsData {
	1: required i32 error_code;
	2: required string error_msg;
	3: required list<map<string,string>> ans_data;
	4: optional string entrust_no; 
}
service ZxTradeSdk {
	ZxTradeAnsData login(1: string account,2: string password); 
	ZxTradeAnsData search_stockholder(1: string account); 
	ZxTradeAnsData search_fund_msg(1: string account); 
	ZxTradeAnsData run_normal_entrust(1: string account,2: string stock_code,3: string shorsz,4: string num,5: string price,6: string buyorsell); 
	ZxTradeAnsData serach_entrust_status(1: string account,2: string entrust_no); 
	ZxTradeAnsData run_undo_entrust(1: string account,2: string entrust_no); 
}