#pragma once

class Upload {
    public:
        static void upload_(const crow::request& req, crow::response& res);
        static void upload_(crow::websocket::connection& conn, const std::string& data, bool is_binary);
};