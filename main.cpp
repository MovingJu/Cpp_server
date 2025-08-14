#include "crow.h"
#include "routes/routes.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return index();
    });

    CROW_ROUTE(app, "/camera")([](){
        return camera();
    });

    app.port(8000)
       .run();
}
