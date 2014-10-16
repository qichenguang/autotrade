/////////////////////////////////////////////////////////////////////////////////////////////
// client.cpp
/////////////////////////////////////////////////////////////////////////////////////////////
#include "../gen-cpp/ZxTradeSdk.h"
#include <transport/TSocket.h>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <string>

#pragma comment(lib, "libthrift.lib")//Á´½Ó¿âÎÄ¼þ

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace com::autotrade::zxtradesdk;
using namespace std;

std::vector<map<std::string,std::string>>::iterator  vec_iter;
std::map<std::string,std::string>::iterator  map_iter;

int show_return_msg(ZxTradeAnsData& ans_data){
	std::cout << "error_code = " << ans_data.error_code << endl;
	std::cout << "error_msg = " << ans_data.error_msg << endl;
	for(vec_iter = ans_data.ans_data.begin(); vec_iter != ans_data.ans_data.end(); vec_iter++){
		for(map_iter = vec_iter->begin(); map_iter != vec_iter->end(); map_iter++){
			std::cout << map_iter->first << "..." << map_iter->second << std::endl;
		}
		std::cout << "----------------------------------------------------------------------------------------" << endl;
	}
	return 0;
}
int main(int argc, char** argv) {
	int port = 9090;
	boost::shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 9090));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	ZxTradeSdkClient client(protocol);
	try{
		transport->open();
		//
		ZxTradeAnsData ans_data;
		string entrust_no;
		{
			client.login(ans_data,"600511005","606869");
			//
			cout << "return from login beg--------------" << endl;
			show_return_msg(ans_data);
			cout << "return from login end--------------" << endl;
		}
		{
			client.search_stockholder(ans_data,"600511005");
			//
			cout << "return from search_stockholder beg--------------" << endl;
			show_return_msg(ans_data);
			cout << "return from search_stockholder end--------------" << endl;
		}
		{
			client.search_fund_msg(ans_data,"600511005");
			//
			cout << "return from search_fund_msg beg--------------" << endl;
			show_return_msg(ans_data);
			cout << "return from search_fund_msg end--------------" << endl;
		}
		{
			client.run_normal_entrust(ans_data,"600511005","600696","1","100","100.00","1");
			//
			cout << "return from normal_entrust beg--------------" << endl;
			show_return_msg(ans_data);
			entrust_no = ans_data.entrust_no;
			cout << "return from normal_entrust end--------------" << endl;
		}
		{
			client.serach_entrust_status(ans_data,"600511005",entrust_no);
			//
			cout << "return from serach_entrust_status beg--------------" << endl;
			show_return_msg(ans_data);
			cout << "return from serach_entrust_status end--------------" << endl;
		}
		{
			//
			client.run_undo_entrust(ans_data,"600511005",entrust_no);
			//
			cout << "return from undo_entrust beg--------------" << endl;
			show_return_msg(ans_data);
			cout << "return from undo_entrust end--------------" << endl;
		}

		//
		transport->close();
	}catch(TException& tx){
		printf("ERROR:%s\n",tx.what());
	}
	//getchar();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////