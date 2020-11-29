#include "sphere.h"
#include <cmath>
#include <iostream>

Sphere::Sphere(float radius)
    : m_radius(radius)
{
}

Sphere::Sphere(const PropertyList &propList)
{
    m_radius = propList.getFloat("radius",1.f);
    m_center = propList.getPoint("center",Point3f(0,0,0));
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray& ray, Hit& hit) const
{
    
    Vector3f oc = ray.origin - m_center;
    float a = ray.direction.dot(ray.direction);
    float b = 2.0 * ray.direction.dot(oc);
    float c = oc.dot(oc) - m_radius * m_radius;
    
    float discriminant = b*b - 4*a*c;

    if(discriminant >= 0){
        float x1 = (-b - sqrt(discriminant) )/ (2 * a);
        float x2 = (-b + sqrt(discriminant) ) / (2 * a);

        if(x1 < 0 && x2 < 0){
            return false;
        }
        if(x1 < x2){
            if(x1 > 0){
                hit.setT(x1);
            }
            else {
                hit.setT(x2);
            }
            hit.setShape(this);
        } else {
            if(x2 > 0){
                hit.setT(x2);
            }
            else {
                hit.setT(x1);
            }
        }
        hit.setShape(this);
        Vector3f impactPoint = ray.origin + hit.t() * ray.direction;
        Vector3f normal (impactPoint - m_center);
        normal.normalize();
        float side = static_cast<float>( -sgn( normal.dot(ray.direction) ) );

        hit.setNormal(side * normal);

        return true;
    }

    

    return false;
}

REGISTER_CLASS(Sphere, "sphere")
