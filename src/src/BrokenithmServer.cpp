#include "BrokenithmServer.hpp"

#include <thread>
#include <string>
#include <vector>

#include "uws/App.h"
#include "uws/Loop.h"
#include "spdlog/spdlog.h"

#include "AsyncFileStreamer.hpp"
#include "ControllerState.hpp"

struct BrokenithmServer::Impl
{
    int m_port;

    void *m_uws_loop;
    void *m_uws_socket_token;
    std::thread m_thread;

    bool m_running;

    ControllerState m_controller_state;

    Impl(int port);
    ~Impl();

    void start_server_async();
    void start_server();
    void stop_server();
};

BrokenithmServer::BrokenithmServer(int port)
    : m_impl(std::make_unique<Impl>(port)){};

BrokenithmServer::~BrokenithmServer() = default;

void BrokenithmServer::start_server()
{
    m_impl->start_server_async();
}

void BrokenithmServer::stop_server()
{
    m_impl->stop_server();
}

uint64_t BrokenithmServer::get_controller_state()
{
    return m_impl->m_controller_state.m_button_state;
}

struct ConnectionData
{
    static int s_connection_counter;
    static std::vector<ConnectionData *> s_connections;

    int m_uid;
    uWS::WebSocket<false, true> *m_websocket;

    void static close_all_connections();

    ConnectionData() : m_websocket(nullptr)
    {
        m_uid = s_connection_counter;

        s_connection_counter++;
        s_connections.resize(s_connection_counter);
        s_connections[m_uid] = this;
    }

    ~ConnectionData()
    {
        s_connections[m_uid] = nullptr;
    }

    void save_socket(uWS::WebSocket<false, true> *websocket)
    {
        m_websocket = websocket;
    }
};

int ConnectionData::s_connection_counter = 0;
std::vector<ConnectionData *> ConnectionData::s_connections(0);

void ConnectionData::close_all_connections()
{
    for (auto connection : s_connections)
    {
        if (connection != nullptr)
        {
            connection->m_websocket->end(uWS::CLOSE, "");
        }
    }
}

BrokenithmServer::Impl::Impl(int port) : m_port(port),
                                         m_uws_loop(nullptr),
                                         m_uws_socket_token(nullptr),
                                         m_thread(),
                                         m_running(false){};

BrokenithmServer::Impl::~Impl()
{
    if (m_running)
    {
        stop_server();
    }
}

void BrokenithmServer::Impl::start_server_async()
{
    m_thread = std::thread([&] { start_server(); });
}

void BrokenithmServer::Impl::start_server()
{
    spdlog::info("Starting server...");

    AsyncFileStreamer asyncFileStreamer("res/www/");

    m_uws_loop = uWS::Loop::get();

    uWS::App()
        .get(
            "/",
            [&asyncFileStreamer](auto *res, auto *req) {
                res->writeStatus(uWS::HTTP_200_OK);
                asyncFileStreamer.streamFile<false>(res, "index.html");
            })
        .get(
            "/config.js",
            [&asyncFileStreamer](auto *res, auto *req) {
                res->writeStatus(uWS::HTTP_200_OK);
                asyncFileStreamer.streamFile<false>(res, "config.js");
            })
        .get(
            "/app.js",
            [&asyncFileStreamer](auto *res, auto *req) {
                res->writeStatus(uWS::HTTP_200_OK);
                asyncFileStreamer.streamFile<false>(res, "app.js");
            })
        .get(
            "/favicon.ico",
            [&asyncFileStreamer](auto *res, auto *req) {
                res->writeStatus(uWS::HTTP_200_OK);
                asyncFileStreamer.streamFile<false>(res, "favicon.ico");
            })
        .ws<ConnectionData>(
            "/ws",
            {uWS::DISABLED,
             16 * 1024 * 1024,
             10,
             16 * 1024 * 1024,
             // Open handler
             [](auto *ws, auto *req) {
                 spdlog::info("Controller ID {} connected", ((ConnectionData *)ws->getUserData())->m_uid);
                 ((ConnectionData *)ws->getUserData())->save_socket(ws);
             },
             // Message handler
             [&](auto *ws, std::string_view message, uWS::OpCode opCode) {
                 if (opCode == uWS::TEXT)
                 {
                     if (message.size() == 39 && message[0] == 'b')
                     {
                         m_controller_state.start();

                         for (int i = 0; i < 38; i++)
                         {
                             if (message[i] == '1')
                             {
                                 m_controller_state.add_button(i);
                             }
                         }
                         m_controller_state.end();
                     }
                     else if (message == "alive?")
                     {
                         ws->send("alive", uWS::TEXT);
                     }
                 }
             },
             // Drain handler
             [](auto *ws) {},
             // Ping handler
             [](auto *ws) {},
             // Pong handler
             [](auto *ws) {},
             // Close handler
             [](auto *ws, int code, std::string_view message) {
                 spdlog::info("Controller ID {} disconnected", ((ConnectionData *)ws->getUserData())->m_uid);
             }})
        .listen(
            m_port,
            [&](auto *token) {
                if (token)
                {
                    spdlog::info("Server listening at port {}", m_port);
                    m_running = true;
                    m_uws_socket_token = token;
                }
            })
        .run();

    m_running = false;
}

void BrokenithmServer::Impl::stop_server()
{
    spdlog::info("Stopping server...");
    if (m_uws_loop)
    {
        ((uWS::Loop *)m_uws_loop)->defer([&] {
            ConnectionData::close_all_connections();
            if (m_uws_socket_token)
            {
                us_listen_socket_close(0, (us_listen_socket_t *)m_uws_socket_token);
            }
        });
    }

    if (m_thread.joinable())
    {
        m_thread.join();
    }
    m_running = false;

    spdlog::info("Server stopped");
}
