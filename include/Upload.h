#pragma once

class Upload {
    public:
        // OPTION no longer used
        static void upload(crow::response& res);
        // POST no longer used
        static void upload(const crow::request& req, crow::response& res);
        // WebSocket
        static void upload(crow::websocket::connection& conn, const std::string& data, bool is_binary);
};