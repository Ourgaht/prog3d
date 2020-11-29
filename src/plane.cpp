#include "plane.h"
#include <cmath>

Plane::Plane()
{
}

Plane::Plane(const PropertyList &propList)
{
    m_position = propList.getPoint("position",Point3f(0,0,0));
    m_normal = propList.getVector("normal",Point3f(0,0,1));
}

Plane::~Plane()
{
}

bool Plane::intersect(const Ray& ray, Hit& hit) const
{
    float ddn = ray.direction.dot(m_normal);

    if (fabs(ddn) < 1e-5) {
        return false;
    }

    Point3f amo = m_position - ray.origin;

    float amodn = amo.dot(m_normal);
    float t = amodn / ddn;

    if (t < 0) {
        return false;
    }

    hit.setShape(this);
    hit.setT(t);

    float side = static_cast<float>( -sgn(m_normal.dot(ray.direction)) );
    hit.setNormal(side * m_normal);

    return true;
}

REGISTER_CLASS(Plane, "plane")
