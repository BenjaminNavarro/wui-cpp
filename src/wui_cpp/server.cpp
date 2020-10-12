#include <wui-cpp/server.h>
#include <wui-cpp/widgets/widget.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vector>

#include <simple-web-server/server_http.hpp>
#include <nlohmann/json.hpp>

namespace wui {
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

struct Server::pImpl {
    HttpServer server;
    std::thread server_async_thread;

    std::vector<WidgetPtr> widgets;
    std::mutex update_mutex;
};

Server::Server(const std::string& root_path, int port,
               const std::string& address)
    : impl_(std::make_unique<Server::pImpl>()) {
    impl_->server.config.port = port;
    impl_->server.config.address = address;

    auto web_root_path = std::filesystem::canonical(root_path);

    // Default GET-example. If no other matches, this anonymous function will be
    // called. Will respond with content in the web/-directory, and its
    // subdirectories. Default file: index.html Can for instance be used to
    // retrieve an HTML 5 client that uses REST-resources on this server
    impl_->server.default_resource["GET"] =
        [web_root_path](std::shared_ptr<HttpServer::Response> response,
                        std::shared_ptr<HttpServer::Request> request) {
            try {
                auto path = std::filesystem::canonical(web_root_path.string() +
                                                       request->path);
                // Check if path is within web_root_path
                if (std::distance(web_root_path.begin(), web_root_path.end()) >
                        std::distance(path.begin(), path.end()) ||
                    !std::equal(web_root_path.begin(), web_root_path.end(),
                                path.begin()))
                    throw std::invalid_argument(
                        "path must be within root path");
                if (std::filesystem::is_directory(path))
                    path /= "index.html";

                SimpleWeb::CaseInsensitiveMultimap header;

                auto ifs = std::make_shared<std::ifstream>();
                ifs->open(path.string(),
                          std::ifstream::in | std::ios::binary | std::ios::ate);

                if (*ifs) {
                    auto length = ifs->tellg();
                    ifs->seekg(0, std::ios::beg);

                    header.emplace("Content-Length", to_string(length));
                    response->write(header);

                    // Trick to define a recursive function within this scope
                    // (for example purposes)
                    struct FileServer {
                        static void read_and_send(
                            const std::shared_ptr<HttpServer::Response>&
                                response,
                            const std::shared_ptr<std::ifstream>& ifs) {
                            // Read and send 128 KB at a time
                            static std::vector<char> buffer(
                                131072); // Safe when server is running on one
                                         // thread
                            std::streamsize read_length;
                            if ((read_length =
                                     ifs->read(&buffer[0],
                                               static_cast<std::streamsize>(
                                                   buffer.size()))
                                         .gcount()) > 0) {
                                response->write(&buffer[0], read_length);
                                if (read_length == static_cast<std::streamsize>(
                                                       buffer.size())) {
                                    response->send(
                                        [response,
                                         ifs](const SimpleWeb::error_code& ec) {
                                            if (!ec)
                                                read_and_send(response, ifs);
                                            else
                                                std::cerr
                                                    << "Connection interrupted"
                                                    << std::endl;
                                        });
                                }
                            }
                        }
                    };
                    FileServer::read_and_send(response, ifs);
                } else
                    throw std::invalid_argument("could not read file");
            } catch (const std::exception& e) {
                response->write(SimpleWeb::StatusCode::client_error_bad_request,
                                "Could not open path " + request->path + ": " +
                                    e.what());
            }
        };

    impl_->server.on_error =
        [](std::shared_ptr<HttpServer::Request> /*request*/,
           const SimpleWeb::error_code& /*ec*/) {
            // Handle errors here
            // Note that connection timeouts will also call this handle with ec
            // set to SimpleWeb::errc::operation_canceled
        };

    impl_->server.resource["^/set_value$"]["POST"] =
        [this](std::shared_ptr<HttpServer::Response> response,
               std::shared_ptr<HttpServer::Request> request) {
            try {
                auto json = nlohmann::json::parse(request->content);
                auto id = json["id"].get<int>();

                if (id >= impl_->widgets.size()) {
                    auto error = "no widget with ID=" + std::to_string(id) +
                                 " has been registered";
                    std::cerr << "wui::Server: " << error << "\n";
                    response->write(
                        SimpleWeb::StatusCode::client_error_bad_request, error);
                    return;
                }

                try {
                    std::unique_lock<std::mutex> lock(impl_->update_mutex);
                    impl_->widgets.at(id)->setter(json);

                } catch (const std::exception& e) {
                    auto error = "impossible to configure widget with ID=" +
                                 std::to_string(id) + " (" + e.what() + ")";
                    std::cerr << "wui::Server: " << error << "\n";
                    response->write(
                        SimpleWeb::StatusCode::client_error_bad_request, error);
                    return;
                }

                response->write(SimpleWeb::StatusCode::success_ok);
            } catch (const std::exception& e) {
                *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: "
                          << strlen(e.what()) << "\r\n\r\n"
                          << e.what();
            }
        };

    impl_->server.resource["^/get_value$"]["POST"] =
        [this](std::shared_ptr<HttpServer::Response> response,
               std::shared_ptr<HttpServer::Request> request) {
            auto json = nlohmann::json::parse(request->content);
            auto id = json["id"].get<int>();
            try {
                std::unique_lock<std::mutex> lock(impl_->update_mutex);
                response->write(impl_->widgets.at(id)->getter());
            } catch (...) {
                auto error = "no getter with ID=" + std::to_string(id) +
                             " has been registered";
                std::cerr << "wui::Server: " << error << "\n";
                response->write(
                    SimpleWeb::StatusCode::server_error_internal_server_error,
                    error);
            }
        };

    impl_->server.resource["^/get_ui$"]["GET"] =
        [this](std::shared_ptr<HttpServer::Response> response,
               std::shared_ptr<HttpServer::Request> request) {
            using json = nlohmann::json;

            json j;
            j["widgets"] = json::array();
            for (size_t i = 0; i < impl_->widgets.size(); ++i) {
                json widget_desc = *impl_->widgets[i];
                widget_desc["id"] = i;
                j["widgets"].push_back(widget_desc);
            }

            std::string ui_description = j.dump();

            try {
                response->write(ui_description);
            } catch (...) {
                auto error = "cannot generate the UI description";
                std::cerr << "wui::Server: " << error << "\n";
                response->write(
                    SimpleWeb::StatusCode::server_error_internal_server_error,
                    error);
            }
        };
}

Server::~Server() {
    stop();
}

void Server::start() {
    impl_->server_async_thread =
        std::thread([this]() { impl_->server.start(); });
}

void Server::stop() {
    impl_->server.stop();
    if (impl_->server_async_thread.joinable()) {
        impl_->server_async_thread.join();
    }
}

void Server::update() {
    std::unique_lock<std::mutex> lock(impl_->update_mutex);
    for (auto& widget : impl_->widgets) {
        widget->update();
    }
}

void Server::addWidget(WidgetPtr widget) {
    impl_->widgets.push_back(widget);
    widget->id = impl_->widgets.size() - 1;
}

} // namespace wui
