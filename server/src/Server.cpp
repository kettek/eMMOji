#include "Server.hpp"
#include <future> // for HTTP handler
#include <fstream>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace eMMOji {
Server::Server() {
  mServer.init_asio();
  // disable logging for now
  mServer.clear_access_channels(websocketpp::log::alevel::all);

  mServer.set_open_handler(bind(&Server::onOpen, this, websocketpp::lib::placeholders::_1));
  mServer.set_close_handler(bind(&Server::onClose, this, websocketpp::lib::placeholders::_1));
  mServer.set_message_handler(bind(&Server::onMessage, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
  std::cout << "Running temporary HTTP handler" << std::endl;
  mServer.set_http_handler(bind(&Server::onHttp, this, websocketpp::lib::placeholders::_1));
  // Load our data
  loadData("entities", "player");
  loadData("maps", "start");
  // Create our start/lobby Map
  mZoneStartId = createZone();
  mZones[mZoneStartId].activeMap.loadData(&mData["maps"]["start"]);
}

/* HANDLERS */
void Server::onOpen(websocketpp::connection_hdl hdl) {
  server::connection_ptr con = mServer.get_con_from_hdl(hdl);

  con->session_id = mIdPool.getId();
  std::cout << "Opening new connection with id " << con->session_id << std::endl;
}

void Server::onClose(websocketpp::connection_hdl hdl) {
  server::connection_ptr con = mServer.get_con_from_hdl(hdl);

  std::cout << "Closing " << con->name << " with id " << con->session_id << std::endl;
  mIdPool.revokeId(con->session_id);

  // remove Client if it exists
  auto it = mIdToClients.find(con->session_id);
  if (it != mIdToClients.end()) {
    mIdToClients.erase(it);
  }
}

void Server::onMessage(websocketpp::connection_hdl hdl, server::message_ptr msg) {
  server::connection_ptr con = mServer.get_con_from_hdl(hdl);

  rapidjson::Document data;

  switch (mIdToClients[con->session_id].clientState) {
    case ClientState::CONNECTING: {
      std::cout << "Message from client: " << msg->get_payload().c_str() << std::endl;
      // IF OK
      if (data.Parse(msg->get_payload().c_str()).HasParseError()) {
        std::cout << "error reading, kick player" << std::endl;
        mServer.close(hdl, websocketpp::close::status::unsupported_data, "BOGUS");
        return;
      }
      // TODO: verify data
      // add the client
      mIdToClients.emplace(con->session_id, Client{});
      mIdToClients[con->session_id].name = data["name"].GetString();
      // let the client know it can continue
      mIdToClients[con->session_id].clientState = ClientState::LOGGED_IN;
      mServer.send(hdl, "OK", websocketpp::frame::opcode::TEXT);
      std::cout << "Client upgraded to LOGGED_IN" << std::endl;
      // Now we:
      //  * add Client to startZone or LastZone
      //  * add Entity to Client + Zone
      //  * send net client the map and entities
      //  Add client to zone
      Zone* zone = &mZones[mZoneStartId];
      zone->connectedClients.insert(std::pair<int, Client*>(con->session_id, &mIdToClients[con->session_id]));
      // TODO: copy player and add the custom player-specific stuff
      mIdToClients[con->session_id].entityId = zone->activeMap.addEntity(&mData["entities"]["player"]);
      // send player the current zone map
      for (auto const& entity : zone->activeMap.activeEntities) {
        std::cout << "parsing entity" << std::endl;
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        entity.data.Accept(writer);
        // Output {"project":"rapidjson","stars":11}
        std::cout << buffer.GetString() << std::endl;
      }
      break;
    }
    case ClientState::LOGGED_IN: {
      if (msg->get_payload().compare("OK")) {
        // upgrade client
        mIdToClients[con->session_id].clientState = ClientState::READY;
      } else {
        mServer.close(hdl, websocketpp::close::status::unsupported_data, "BOGUS");
        return;
      }
      break;
    }
    case ClientState::READY: {
      // TODO: handle the appropriate Zone logic!
      // At this point our commands must be JSON
      if (data.Parse(msg->get_payload().c_str()).HasParseError()) {
        std::cout << "error reading, kick player" << std::endl;
        mServer.close(hdl, websocketpp::close::status::unsupported_data, "BOGUS");
        return;
      }
      // At this point, zone should(!) be valid
      Zone* zone = &mZones[mIdToClients[con->session_id].zoneId];
      zone->addCommands(con->session_id, data.GetObject());
      break;
    }
  }
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
/* MAP-related */
void Server::loadData(std::string section, std::string entry) {
	std::stringstream	output;
	std::ifstream		file;
	std::string			filename;

	// open file
	filename = entry + ".json";
	file.open("../data/"+section+"/"+filename);
	// 404
	if (!file.good()) {
    std::cout << "ERR: " << section << "/" << filename << " doesn't exist" << std::endl;
		return;
  }
	output << file.rdbuf();
  if (mData[section][entry].Parse(output.str().c_str()).HasParseError()) {
    std::cout << "ERR: " << section << "/" << filename << " did not parse" << std::endl;
  }

}
/* ZONE-related */
int Server::createZone() {
  int id = mZonePool.getId();
  mZones.emplace(id, Zone{});
  return id;
}
}
