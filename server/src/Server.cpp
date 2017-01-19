#include "Server.hpp"
#include <future> // for HTTP handler
#include <fstream>

namespace eMMOji {
Server::Server() {
  mServer.init_asio();

  mServer.set_open_handler(bind(&Server::onOpen, this, websocketpp::lib::placeholders::_1));
  mServer.set_close_handler(bind(&Server::onClose, this, websocketpp::lib::placeholders::_1));
  mServer.set_message_handler(bind(&Server::onMessage, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
  mServer.set_http_handler(bind(&Server::onHttp, this, websocketpp::lib::placeholders::_1));
}

/* HANDLERS */
void Server::onOpen(websocketpp::connection_hdl hdl) {
  mServer.get_con_from_hdl(hdl)->session_id = mNextSessionId++;
}

void Server::onClose(websocketpp::connection_hdl hdl) {
  server::connection_ptr con = mServer.get_con_from_hdl(hdl);
  
  std::cout << "Closing " << con->name << " with id " << con->session_id << std::endl;
}

void Server::onMessage(websocketpp::connection_hdl hdl, server::message_ptr msg) {
  server::connection_ptr con = mServer.get_con_from_hdl(hdl);
  std::cout << "Message from client: " << msg->get_payload();
  mServer.send(hdl, msg->get_payload(), msg->get_opcode());

}
/* NOT HANDLERS */
void Server::gogogo(uint16_t port) {
  mServer.listen(port);
  mServer.start_accept();
  mServer.run();
}
}
