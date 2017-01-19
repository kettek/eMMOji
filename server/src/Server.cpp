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

void Server::onHttp(websocketpp::connection_hdl hdl) {
	// FIXME: This is both insecure and temporary -- websockets++ is not meant for HTTP, but it is easier for testing to handle here
	server::endpoint::connection_ptr con = mServer.get_con_from_hdl(hdl);
	std::stringstream	output;
	std::ifstream		file;
	std::string			filename;

	// open file
	filename = con->get_resource();
	if (filename == "/" || filename == "") {
		filename = "/index.html";
	}
	file.open("../client"+filename);
	// 404
	if (!file.good()) {
		con->set_status(websocketpp::http::status_code::not_found);
		return;
	}
	// send file
	con->set_status(websocketpp::http::status_code::ok);
	output << file.rdbuf();
	con->set_body(output.str());
	file.close();
}
/* NOT HANDLERS */
void Server::gogogo(uint16_t port) {
  mServer.listen(port);
  mServer.start_accept();
  mServer.run();
}
}
