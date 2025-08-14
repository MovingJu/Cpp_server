#pragma once

std::string index(){
    std::string page = crow::mustache::load_text("index.html");
    return page;
}
