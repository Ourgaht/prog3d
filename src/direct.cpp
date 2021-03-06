#include "integrator.h"
#include "scene.h"

class DirectIntegrator : public Integrator {
public:
    DirectIntegrator(const PropertyList& props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene* scene, const Ray& ray) const {
        /** TODO : Find the surface that is visible in the requested direction
                   Return its ambiant color */

        Hit hit;
        scene->intersect(ray, hit);
        if (hit.shape()) {
            return computeColor(scene, hit, ray, scene->lightList());
        }
        return Color3f(0.f);
    }

    std::string toString() const {
        return "DirectIntegrator[]";
    }

private:
    Color3f computeColor(const Scene* scene, const Hit &hit, const Ray& ray, const LightList& lightList) const {
        const Shape* shape = hit.shape();
        Color3f color;

        const Material * material = shape->material();
        if(material){
            for(const Light *light : lightList){
                Point3f p = ray.origin + hit.t() * ray.direction;
                float ld = std::numeric_limits<float>::max();
                Vector3f l = light->direction(p, &ld);
                if(!inShadows(scene, p + 1e-4 * hit.normal(), *light)){
                    Color3f brdf = material->brdf(ray.direction, l, hit.normal());
                    color += brdf * std::max(l.dot(hit.normal()), 0.0f) * light->intensity(p);
                }
            }
        }

        return color;
    }

    bool inShadows(const Scene* scene, const Point3f &p, const Light &light) const {
        Ray ray = Ray(p, light.direction(p));
        Hit hit;
        scene->intersect(ray, hit);
        return hit.shape();
    }
};

REGISTER_CLASS(DirectIntegrator, "direct")
