#ifndef SPHEREH
#define SPHEREH
#include "hitable.h"

class sphere: public hitable {
    public:
        sphere() {}
        sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
        virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
        vec3 center;
        float radius;
        material *mat_ptr;
};

class moving_sphere: public hitable {
    public:
        moving_sphere() {}
        moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m)
            : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        vec3 center(float time) const;
        vec3 center0, center1;
        float time0, time1;
        float radius;
        material *mat_ptr;
};

// t*t*dot( B, B ) + 2*t*dot( B, A -​ C ) + dot( A - C, A - C ) - R*R = 0
// Return true when the equation has one or two roots for t, so ray is in sphere
bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 OC = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(OC, r.direction());
    float c = dot(OC, OC) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center) / radius;
            rec.mat_prt = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center) / radius;
            rec.mat_prt = mat_ptr;
            return true;
        }
    }
    return false;
}

// t*t*dot( B, B ) + 2*t*dot( B, A -​ C ) + dot( A - C, A - C ) - R*R = 0
// Return true when the equation has one or two roots for t, so ray is in sphere
bool moving_sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 OC = r.origin() - center(r.time()); // only difference is we get the center of the sphere at a certain t
    float a = dot(r.direction(), r.direction());
    float b = dot(OC, r.direction());
    float c = dot(OC, OC) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_prt = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_prt = mat_ptr;
            return true;
        }
    }
    return false;
}

vec3 moving_sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}


#endif