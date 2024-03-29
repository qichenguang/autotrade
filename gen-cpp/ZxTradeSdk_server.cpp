// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "ZxTradeSdk.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

//zx sdk beg
#include "../zx-sdk/ZxHsSdk.h"
//zx sdk end
#pragma comment(lib, "libthrift.lib")

//
using boost::shared_ptr;

typedef map<string,string> MAP_ACCOUNT;
typedef map<string,boost::shared_ptr<ZxHsSdk>> MAP_ZX_SDK;
MAP_ACCOUNT g_account;
MAP_ZX_SDK g_sdk;
//
boost::mutex g_mutex;  

void add_sdk_obj_to_map(string account,string password,ZxTradeAnsData& _return){
	boost::shared_ptr<ZxHsSdk> sdk(new ZxHsSdk());
	int iret = sdk->init();
	iret = sdk->login(account,password,_return);
	if(ZX_TRADE_ERROR_CODE::ZX_TRADE_OK == iret){
		g_sdk.insert(make_pair(account,sdk));
		g_account.insert(make_pair(account,password));
		cout << "add new account to pool !!!!!!!!!!!!!!" << endl;
	}else{
		//new a obj fail,delete old obj.
		g_account.erase(account);
		g_sdk.erase(account);
	}
	return;
}
bool find_sdk_obj_in_map(string account,ZxTradeAnsData& _return,MAP_ZX_SDK::iterator&  out_iter ){
	MAP_ZX_SDK::iterator  iter = g_sdk.find(account);
	if(iter != g_sdk.end()){
		out_iter = iter;
		return true;
	}else{
		_return.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_OBJ_NOT_FIND_ERROR;
		_return.error_msg = "SDK OBJ IS NOT FIND!";
		return false;
	}
	return false;
}

class ZxTradeSdkHandler : virtual public ZxTradeSdkIf {
 public:
  ZxTradeSdkHandler() {
    // Your initialization goes here
  }

  void login(ZxTradeAnsData& _return, const std::string& account, const std::string& password) {
    // Your implementation goes here
    printf("login\n");

	boost::lock_guard<boost::mutex> lock(g_mutex);  

	MAP_ACCOUNT::iterator  iter = g_account.find(account);
	if(iter != g_account.end()){
		//find old obj, check password
		if(iter->second == password){
			//find same password.
			MAP_ZX_SDK::iterator  iter = g_sdk.find(account);
			if(iter != g_sdk.end()){
				//find old obj.
				int iret = iter->second->search_stockholder(_return);
				if(ZX_TRADE_ERROR_CODE::ZX_TRADE_OK == iret){
					//ok. use old obj.
					return;
				}else{
					//old obj is bad.
					//new a obj, add it to map
					add_sdk_obj_to_map(account,password,_return);
					return;
				}
			}else{
				//not find old obj.some error?
				//new a obj, add it to map
				add_sdk_obj_to_map(account,password,_return);
				return;
			}
		}else{
			//not find the passowrd, it is a new password
			//1. first find the old obj.
			MAP_ZX_SDK::iterator  iter = g_sdk.find(account);
			if(iter != g_sdk.end()){
				int iret = iter->second->search_stockholder(_return);
				if(ZX_TRADE_ERROR_CODE::ZX_TRADE_OK == iret){
					//ok. old obj is can work, new password is error.
					_return.error_code = ZX_TRADE_ERROR_CODE::ZX_TRADE_ACCOUNT_PWD_ERROR;
					_return.error_msg = "ACCOUNT PWD IS ERROR!";
					return;
				}else{
					//old obj can not work
					//now add a new one to map
					add_sdk_obj_to_map(account,password,_return);
					return;
				}
			}
		}
	}else{
		//not find account
		//now it must hava not old obj.
		//new obj, add it to map
		add_sdk_obj_to_map(account,password,_return);
	}
  }

  void search_stockholder(ZxTradeAnsData& _return, const std::string& account) {
    // Your implementation goes here
    printf("search_stockholder\n");
	
	boost::lock_guard<boost::mutex> lock(g_mutex);

	MAP_ZX_SDK::iterator iter;
	bool findIt = find_sdk_obj_in_map(account,_return,iter);
	if(true == findIt){
		int iret = iter->second->search_stockholder(_return);
	}
  }

  void search_fund_msg(ZxTradeAnsData& _return, const std::string& account) {
    // Your implementation goes here
    printf("search_fund_msg\n");

	boost::lock_guard<boost::mutex> lock(g_mutex);

	MAP_ZX_SDK::iterator iter;
	bool findIt = find_sdk_obj_in_map(account,_return,iter);
	if(true == findIt){
		int iret = iter->second->search_fund_msg(_return);
	}
  }

  void run_normal_entrust(ZxTradeAnsData& _return, const std::string& account, const std::string& stock_code, 
	  const std::string& shorsz, const std::string& num, const std::string& price, const std::string& buyorsell) {
    // Your implementation goes here
    printf("run_normal_entrust\n");

	boost::lock_guard<boost::mutex> lock(g_mutex);

	MAP_ZX_SDK::iterator iter;
	bool findIt = find_sdk_obj_in_map(account,_return,iter);
	if(true == findIt){
		int iret = iter->second->run_normal_entrust(stock_code,shorsz,num,price,buyorsell,_return);
	}
  }

  void serach_entrust_status(ZxTradeAnsData& _return, const std::string& account, const std::string& entrust_no) {
    // Your implementation goes here
    printf("serach_entrust_status\n");

	boost::lock_guard<boost::mutex> lock(g_mutex);

	MAP_ZX_SDK::iterator iter;
	bool findIt = find_sdk_obj_in_map(account,_return,iter);
	if(true == findIt){
		int iret = iter->second->serach_entrust_status(entrust_no,_return);
	}
  }

  void run_undo_entrust(ZxTradeAnsData& _return, const std::string& account, const std::string& entrust_no) {
    // Your implementation goes here
    printf("run_undo_entrust\n");

	boost::lock_guard<boost::mutex> lock(g_mutex);

	MAP_ZX_SDK::iterator iter;
	bool findIt = find_sdk_obj_in_map(account,_return,iter);
	if(true == findIt){
		int iret = iter->second->run_undo_entrust(entrust_no,_return);
	}
  }
};

int main(int argc, char **argv) {
	//-----------------------------//
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested =MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	//-------------------------------//
	//对上面这段代码做个说明，这是依赖windows的一段代码
	//到2014.9.2官网的稳定版0.9.1，仍需要这段代码才可以在windows下编译通过。
	//但是如果用git clone最新版，这个错误已经修正
	//最新版注释掉这段代码，一样可以在windows下编译通过。
	//备注时间：2014.9.2
	//------------------------------//
  int port = 9090;
  boost::shared_ptr<ZxTradeSdkHandler> handler(new ZxTradeSdkHandler());
  boost::shared_ptr<TProcessor> processor(new ZxTradeSdkProcessor(handler));
  boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

