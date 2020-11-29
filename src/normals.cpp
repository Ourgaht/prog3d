#include "integrator.h"
#include "scene.h"

class NormalsIntegrator : public Integrator {
public:
    NormalsIntegrator(const PropertyList& props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene* scene, const Ray& ray) const {
        /** TODO : Find the surface that is visible in the requested direction
                   Return its ambiant color */

        Hit hit;
        scene->intersect(ray, hit);
        if (hit.shape()) {
            const Shape* shape = hit.shape();
            Color3f color;
			
			color.r() = hit.normal()[0] * 0.5f + 0.5f;
			color.g() = hit.normal()[1] * 0.5f + 0.5f;
			color.b() = hit.normal()[2] * 0.5f + 0.5f;
            //color.r() = fabs(hit.normal()[0]);
			//color.g() = fabs(hit.normal()[1]);
			//color.b() = fabs(hit.normal()[2]);
            return color;
        }
        return Color3f(0.f);
    }

    std::string toString() const {
        return "NormalsIntegrator[]";
    }
};

REGISTER_CLASS(NormalsIntegrator, "normals")
