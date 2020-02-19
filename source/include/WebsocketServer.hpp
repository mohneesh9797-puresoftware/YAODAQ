#pragma once

#include <string>
#include <unordered_map>

#include "Infos.hpp"
#include "IXWebSocketServer.h"

class WebsocketServer
{
public:
  WebsocketServer(const int& port = ix::SocketServer::kDefaultPort,const std::string& host = ix::SocketServer::kDefaultHost,const int& backlog = ix::SocketServer::kDefaultTcpBacklog,const std::size_t& maxConnections = ix::SocketServer::kDefaultMaxConnections,const int& handshakeTimeoutSecs = 3);
  ~WebsocketServer();
  void start();
  void stop();
  void wait();
  void listen();
private:
  ix::WebSocketServer m_Server;
  std::map<Infos,std::shared_ptr<ix::WebSocket>> m_Clients;
  void erase(const std::shared_ptr<ix::WebSocket>& socket);
};
