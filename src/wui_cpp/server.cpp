#include <wui-cpp/server.h>
#include <wui-cpp/widgets/widget.h>

#include <pid/rpath.h>

// for JSON
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

namespace wui {
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

Server::Server(int port, const std::string& address) {
	server_.config.port = port;
	server_.config.address = address;

	// Default GET-example. If no other matches, this anonymous function will be called.
	// Will respond with content in the web/-directory, and its subdirectories.
	// Default file: index.html
	// Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
	server_.default_resource["GET"] =
		[](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
			try {
				auto web_root_path = boost::filesystem::canonical(PID_PATH("wui-cpp/web"));
				auto path = boost::filesystem::canonical(web_root_path / request->path);
				// Check if path is within web_root_path
				if(std::distance(web_root_path.begin(), web_root_path.end()) > std::distance(path.begin(), path.end()) ||
				   !std::equal(web_root_path.begin(), web_root_path.end(), path.begin()))
					throw std::invalid_argument("path must be within root path");
				if(boost::filesystem::is_directory(path))
					path /= "index.html";

				SimpleWeb::CaseInsensitiveMultimap header;

				auto ifs = std::make_shared<std::ifstream>();
				ifs->open(path.string(), std::ifstream::in | std::ios::binary | std::ios::ate);

				if(*ifs) {
					auto length = ifs->tellg();
					ifs->seekg(0, std::ios::beg);

					header.emplace("Content-Length", to_string(length));
					response->write(header);

					// Trick to define a recursive function within this scope (for example purposes)
					struct FileServer {
						static void read_and_send(const std::shared_ptr<HttpServer::Response> &response, const std::shared_ptr<std::ifstream> &ifs) {
							// Read and send 128 KB at a time
							static std::vector<char> buffer(131072);                              // Safe when server is running on one thread
							std::streamsize read_length;
							if((read_length = ifs->read(&buffer[0], static_cast<std::streamsize>(buffer.size())).gcount()) > 0) {
								response->write(&buffer[0], read_length);
								if(read_length == static_cast<std::streamsize>(buffer.size())) {
									response->send([response, ifs](const SimpleWeb::error_code &ec) {
										if(!ec)
											read_and_send(response, ifs);
										else
											std::cerr << "Connection interrupted" << std::endl;
									});
								}
							}
						}
					};
					FileServer::read_and_send(response, ifs);
				}
				else
					throw std::invalid_argument("could not read file");
			}
			catch(const std::exception &e) {
				response->write(SimpleWeb::StatusCode::client_error_bad_request, "Could not open path " + request->path + ": " + e.what());
			}
		};

	server_.on_error =
		[](std::shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
			// Handle errors here
			// Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
		};

	server_.resource["^/set_value$"]["POST"] =
		[this](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
			try {
				boost::property_tree::ptree pt;
				read_json(request->content, pt);

				auto id = pt.get<int>("id");
				try {
					std::unique_lock<std::mutex> lock(update_mutex_);
					widgets_.at(id)->setter(pt);
				}
				catch(...)  {
					auto error = "no setter with ID=" + std::to_string(id) + " has been registered";
					std::cerr << "wui::Server: " << error << "\n";
					response->write(SimpleWeb::StatusCode::client_error_bad_request, error);
				}

				response->write(SimpleWeb::StatusCode::success_ok);
			}
			catch(const std::exception &e) {
				*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n"
				          << e.what();
			}
		};

	server_.resource["^/get_value$"]["POST"] =
		[this](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
			boost::property_tree::ptree pt;
			read_json(request->content, pt);

			auto id = pt.get<int>("id");
			try {
				std::unique_lock<std::mutex> lock(update_mutex_);
				response->write(widgets_.at(id)->getter());
			}
			catch(...)  {
				auto error = "no getter with ID=" + std::to_string(id) + " has been registered";
				std::cerr << "wui::Server: " << error << "\n";
				response->write(SimpleWeb::StatusCode::server_error_internal_server_error, error);
			}
		};

	server_.resource["^/get_ui$"]["GET"] =
		[this](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
			using json = nlohmann::json;

			json j;
			j["widgets"] = json::array();
			for (size_t i = 0; i < widgets_.size(); ++i) {
				json widget_desc = *widgets_[i];
				widget_desc["id"] = i;
				j["widgets"].push_back(widget_desc);
			}

			std::string ui_description = j.dump();

			try {
				response->write(ui_description);
			}
			catch(...)  {
				auto error = "cannot generate the UI description";
				std::cerr << "wui::Server: " << error << "\n";
				response->write(SimpleWeb::StatusCode::server_error_internal_server_error, error);
			}
		};

}

Server::~Server() = default;

void Server::startSync() {
	server_.start();
}

void Server::startAsync() {
	server_asyn_thread_ = std::thread([this](){server_.start();});
}

void Server::stop() {
	server_.stop();
	if(server_asyn_thread_.joinable()) {
		server_asyn_thread_.join();
	}
}

void Server::update() {
	std::unique_lock<std::mutex> lock(update_mutex_);
	for(auto& widget: widgets_) {
		widget->update();
	}
}

}
