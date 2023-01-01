#include "muduo/net/http/HttpServer.h"
#include "muduo/net/http/HttpRequest.h"
#include "muduo/net/http/HttpResponse.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/Logging.h"
#include <mutex>
#include <iostream>
#include <map>
#include "TicketManager.h"

using namespace muduo;
using namespace muduo::net;

bool benchmark = false;
int ticket = 10000;
std::mutex _ticket;
void onRequest(const HttpRequest& req, HttpResponse* resp)
{
  std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;
  if (!benchmark)
  {
    const std::map<string, string>& headers = req.headers();
    for (const auto& header : headers)
    {
      std::cout << header.first << ": " << header.second << std::endl;
    }
    std::cout << std::endl;
  }

  if (req.path() == "/")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/html");
    resp->addHeader("Server", "Muduo");
    string now = Timestamp::now().toFormattedString();
    resp->setBody("<html><head><title>This is title</title></head>"
        "<body><h1>Hello</h1>Now is " + now +
        "</body></html>");
  }
  else if (req.path() == "/hello")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/plain");
    resp->addHeader("Server", "Muduo");
    resp->setBody("hello, world!\n");
  }
  else if (req.path() == "/get")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/plain");
    resp->addHeader("Server", "Muduo");
    //TicketManager::getInstance().serve();
    std::lock_guard<std::mutex> _mu{_ticket};
    if (ticket <= 0) {
      resp->setBody("no ticket left\n");
    } else {
      resp->setBody("ticket:"+std::to_string(ticket)+"\n");
      //std::cout << ticket << std::endl;
      --ticket;
    }

  }
  else
  {
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
  }
}

int main(int argc, char* argv[])
{
  int numThreads = 0;
  if (argc > 1)
  {
    benchmark = true;
    Logger::setLogLevel(Logger::WARN);
    numThreads = atoi(argv[1]);
  }
  EventLoop loop;
  HttpServer server(&loop, InetAddress(8000), "dummy");
  server.setHttpCallback(onRequest);
  server.setThreadNum(numThreads);
  server.start();
  loop.loop();
}

