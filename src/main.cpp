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

	//std::cout << "camX: " << camX.toString() << "\ncamY: " << camY.toString() << "\ncamF: " << camF.toString() << std::endl;

    /// TODO:
    ///  1. iterate over the image pixels
	float camWidth_f = (float)camera->vpWidth();
	float camHeight_f = (float)camera->vpHeight();

    for(uint i=0; i < (int)camera->vpWidth(); i++){
        for(uint j = 0; j<camera->vpHeight(); j++){
            ///  2. generate a primary ray
            Ray primaryRay;
            float i_f = (float)i;
            float j_f = (float)j;


            Point3f point = Point3f(camera->position());
            primaryRay.origin = point;
            
            Vector2f viewportCenter(camWidth_f / 2.0f, camHeight_f / 2.0f); // pixel coords, surement (vpWidth/2, vpHeight/2)
            Vector3f xoffset = (i_f - viewportCenter[0]) / (camWidth_f / 2.0f) * camX; 
            Vector3f yoffset = (j_f - viewportCenter[1]) / (camHeight_f / 2.0f) * camY; 
			
			//std::cout << "xoffset: " << xoffset.toString() << "\tyoffset: " << yoffset.toString() << std::endl;
            
            Vector3f d = camF;
            d += xoffset;
            d += yoffset;
            d.normalize();
            primaryRay.direction = d;

            ///  3. call the integartor to compute the color along this ray
            Color3f color = scene->integrator()->Li(scene, primaryRay);

            ///  4. write this color in the result image
            Vector2f pos = Vector2f((float)i , (float)j );

            //Color3f testColor((float)i/(float)camera->vpWidth(), (float)j / (float)camera->vpHeight(), 0.0f);
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
        else
        {
			screen->loadScene("../../../data/troisSpheres.scn");
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
