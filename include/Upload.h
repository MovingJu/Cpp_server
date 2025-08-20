#pragma once

class Upload {
    public:
        // OPTION no longer used
        static void upload_(crow::response& res);
        // POST no longer used
        static void upload_(const crow::request& req, crow::response& res);
        // WebSocket
        static void upload_(crow::websocket::connection& conn, const std::string& data, bool is_binary);
};