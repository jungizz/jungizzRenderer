#include "Application.h"
#include "scenes/BRDF_Scene.h"
#include "scenes/SSSSS_Scene.h"

int main(){
    Application app(1080, 720, "Rendering Test");

    //app.setScene(new SSSSS_Scene());
    app.setScene(new BRDF_Scene());

    app.run();
    return 0;
}