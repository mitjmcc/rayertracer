#include <iostream>
#include <fstream>
#include "stdlib.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "float.h"

using namespace std;

string filename = "hello.ppm";

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0f);
    return p;
}

vec3 color(const ray &r, hitable *world) {
    hit_record rec;
    if (world->hit(r, 0.001f, MAXFLOAT, rec)) { // t_min set to near 0 to avoid shadow acne
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5f * color(ray(rec.p, target - rec.p), world);
    } else {
        // Background color: Lerp from blue to white
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    // Dimensions
    int nx = 200;
    int ny = 100;

    // Number of samples
    int ns = 100;

    // Output file
    ofstream imgRender(filename);

    // PPM Header
    imgRender << "P3\n" << nx << " " << ny << "\n255\n";
    
    // Object list
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5f);
    list[1] = new sphere(vec3(0,-100.5f,-1), 100);
    hitable *world = new hitable_list(list, 2);
    camera cam;

    // Loop through pixels
    for (int j = ny - 1; j > 0; --j) {
        for (int i = 0; i < nx; ++i) {
            vec3 col(0, 0, 0);
            // samples per pixel
            for (int s = 0; s < ns; ++s) {
                // Horizontal and vertical offset vectors
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                // Ray through each sample of each of pixel
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0f);
                col += color(r, world);
            }
            // Average color samples
            col /= float(ns);
            // Gamma 2 correction, color^(1/2)
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            // Color scaled to 0 to 255
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            // Write color to image
            imgRender << ir << " " << ig << " " << ib << "\n";
        }
    }

    imgRender.close();
}
