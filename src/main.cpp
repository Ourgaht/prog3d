#include "viewer.h"

#include <filesystem/resolver.h>

void render(Scene* scene, ImageBlock* result, std::string outputName, bool* done)
{
    if(!scene)
        return;

    clock_t t = clock();

    const Camera *camera = scene->camera();
    const Integrator* integrator = scene->integrator();
    integrator->preprocess(scene);

    float tanfovy2 = tan(camera->fovY()*0.5);
    Vector3f camX = camera->right() * tanfovy2 * camera->nearDist() * float(camera->vpWidth())/float(camera->vpHeight());
    Vector3f camY = -camera->up() * tanfovy2 * camera->nearDist();
    Vector3f camF = camera->direction() * camera->nearDist();

    /// TODO:
    ///  1. iterate over the image pixels
    for(int i=0; i<camera->vpWidth(); i++){
        for(int j = 0; j<camera->vpHeight(); j++){
            ///  2. generate a primary ray
            Ray primaryRay;
            Point3f point = Point3f(camera->position());
            primaryRay.origin = point;
            Vector2i viewportCenter(camera->vpWidth(), camera->vpHeight()); // pixel coords, surement (vpWidth/2, vpHeight/2)
            Vector3f d = camF;
            Vector3f xoffset = ((float)(i - viewportCenter[0]) / (camera->vpWidth() / 2)) * camX; 
            Vector3f yoffset = ((float)(j - viewportCenter[1]) / (camera->vpHeight() / 2)) * camY; 

            d += xoffset;
            d += yoffset;
            d.normalize();
            primaryRay.direction = d;
            ///  3. call the integartor to compute the color along this ray

            Color3f color = scene->integrator()->Li(scene, primaryRay);

            ///  4. write this color in the result image
            Vector2f pos = Vector2f((float)(i - viewportCenter[0]) / (camera->vpWidth() / 2), (float)(j - viewportCenter[1]) / (camera->vpHeight() / 2));
            result->put(pos, color);

        }
    }
    

    t = clock() - t;
    std::cout << "Raytracing time : " << float(t)/CLOCKS_PER_SEC << "s"<<std::endl;

    *done = true;
}

int main(int argc, char *argv[])
{
    getFileResolver()->prepend(DATA_DIR);

    try {
        nanogui::init();
        Viewer *screen = new Viewer();;

        if (argc == 2) {
            /* load file from the command line */
            filesystem::path path(argv[1]);

            if(path.extension() == "scn") { // load scene file
                screen->loadScene(argv[1]);
            }else if(path.extension() == "exr") { // load OpenEXR image
                screen->loadImage(argv[1]);
            }
        }

        /* Enter the application main loop */
        nanogui::mainloop();

        delete screen;
        nanogui::shutdown();
    } catch (const std::exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
        return -1;
    }
    return 0;
}
