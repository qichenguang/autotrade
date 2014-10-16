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

using boost::shared_ptr;

using namespace  ::com::autotrade::zxtradesdk;

class ZxTradeSdkHandler : virtual public ZxTradeSdkIf {
 public:
  ZxTradeSdkHandler() {
    // Your initialization goes here
  }

  void login(ZxTradeAnsData& _return, const std::string& account, const std::string& password) {
    // Your implementation goes here
    printf("login\n");
  }

  void search_stockholder(ZxTradeAnsData& _return, const std::string& account) {
    // Your implementation goes here
    printf("search_stockholder\n");
  }

  void search_fund_msg(ZxTradeAnsData& _return, const std::string& account) {
    // Your implementation goes here
    printf("search_fund_msg\n");
  }

  void run_normal_entrust(ZxTradeAnsData& _return, const std::string& account, const std::string& stock_code, const std::string& shorsz, const std::string& num, const std::string& price, const std::string& buyorsell) {
    // Your implementation goes here
    printf("run_normal_entrust\n");
  }

  void serach_entrust_status(ZxTradeAnsData& _return, const std::string& account, const std::string& entrust_no) {
    // Your implementation goes here
    printf("serach_entrust_status\n");
  }

  void run_undo_entrust(ZxTradeAnsData& _return, const std::string& account, const std::string& entrust_no) {
    // Your implementation goes here
    printf("run_undo_entrust\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<ZxTradeSdkHandler> handler(new ZxTradeSdkHandler());
  shared_ptr<TProcessor> processor(new ZxTradeSdkProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

