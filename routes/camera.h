#pragma once
#include "crow.h"

std::string camera(){
    std::string page = crow::mustache::load_text("camera.html");
    return page;
}