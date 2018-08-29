#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "ray.h"
#include "hitable.h"

vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

// Using Snell's Law and IOR
bool refract(const vec3 &v, const vec3 &n, float ior, vec3 &refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ior * ior * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ior * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0f);
    return p;
}

class material {
    public:
        virtual bool scatter(const ray &r_in,
                             const hit_record &rec,
                             vec3 &attenuation,
                             ray &scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const vec3 &a) : albedo(a) {}
        virtual bool scatter(const ray &r_in, 
                                      const hit_record &rec,
                                      vec3 &attenuation,
                                      ray &scattered) const {
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = ray(rec.p, target - rec.p);
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
};

class metal : public material {
    public:
        metal(const vec3 &a, float m) : albedo(a) { if (m < 1) metallic = m; else metallic = 1; }
        virtual bool scatter(const ray &r_in, 
                                      const hit_record &rec,
                                      vec3 &attenuation,
                                      ray &scattered) const {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + metallic * random_in_unit_sphere());
            attenuation = albedo;
            return dot(scattered.direction(), rec.normal) > 0;
        }

        vec3 albedo;
        float metallic;
};

class dialetric : public material {
    public:
        dialetric(float rf) : ref_index(rf) {}
        virtual bool scatter(const ray &r_in, 
                             const hit_record &rec,
                             vec3 &attenuation,
                             ray &scattered) const {
            vec3 outward_normal;
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            float ior_ratio = 0.0f;
            attenuation = vec3(1, 1, 0);
            vec3 refracted;
            if (dot(r_in.direction(), rec.normal) > 0) {
                outward_normal = -rec.normal;
                ior_ratio = ref_index;
            } else {
                outward_normal = rec.normal;
                ior_ratio = 1.0 / ref_index;
            }
            if (refract(r_in.direction(), outward_normal, ior_ratio, refracted)) {
                scattered = ray(rec.p, refracted);
            } else {
                scattered = ray(rec.p, reflected);
                // return false;
            }
            return true;
        }

        float ref_index;
};

#endif
