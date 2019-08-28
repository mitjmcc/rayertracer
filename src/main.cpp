#include <iostream>
#include <fstream>
#include "stdlib.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "float.h"

using namespace std;

string filename = "hello.ppm";

hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
    int i = 1;
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float choose_mat = drand48();
            vec3 center(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
            if ((center - vec3(4,0.2f,0)).length() > 0.9f) {
                if (choose_mat < 0.8f) // diffuse
                    list[i++] = new moving_sphere(center, center + vec3(0, 0.5f*drand48(), 0), 0.0f, 1.0f, 0.2f,
                                    new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
                else if (choose_mat < 0.95f) // metal
                    list[i++] = new sphere(vec3(1,0,-1), 0.2f,
                                    new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())),  0.5*drand48()));
                else // glass
                    list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
            } 
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list,i);
}

vec3 color(const ray &r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001f, MAXFLOAT, rec)) { // t_min set to near 0 to avoid shadow acne
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_prt->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        } else {
            return vec3(0,0,0);
        }
    } else {
        // Background color: Lerp from blue to white
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    // Dimensions
    int nx = 600;
    int ny = 400;

    // Number of samples
    int ns = 6;

    // Output file
    ofstream imgRender(filename);

    // PPM Header
    imgRender << "P3\n" << nx << " " << ny << "\n255\n";
    
    // Object list
    hitable *world = random_scene();
    vec3 lookfrom(13, 2, 3), lookat(0, 0, 0);
    float aperture = 0.0f;
    float dist_to_focus = 10.0f;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
    int barWidth = 70;
    float progress = 0;
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
                col += color(r, world, 0);
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

        // Loading bar https://stackoverflow.com/a/14539953
        cout << "Rendering [";
        progress = float(ny - j) / float(ny) + 0.01f;
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        cout << "] " << int(progress * 100.0) << " %\r";
        cout.flush();
    }
    cout << "Finished \n";
    imgRender.close();
}
