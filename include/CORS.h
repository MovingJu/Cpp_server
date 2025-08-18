#pragma once
#include "crow.h"

struct CORS {
    struct context {};
    void before_handle(crow::request&, crow::response&, context&) {}
    void after_handle(crow::request&, crow::response& res, context&){
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    }
};