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
    int nx = 200;
    int ny = 100;

    // Number of samples
    int ns = 50;

    // Output file
    ofstream imgRender(filename);

    // PPM Header
    imgRender << "P3\n" << nx << " " << ny << "\n255\n";
    
    // Object list
    hitable *list[4];
    list[0] = new sphere(vec3(0,0,-1), 0.5f,      new lambertian(vec3(0.1f, 0.2f, 0.5f)));
    list[1] = new sphere(vec3(0,-100.5f,-1), 100, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
    list[2] = new sphere(vec3(1,0,-1), 0.5f,  new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
    list[3] = new sphere(vec3(-1,0,-1), 0.5f, new dialetric(1.5f));
    hitable *world = new hitable_list(list, 4);
    camera cam;
    int barWidth = 70;
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

        // Loading bar
        cout << "Rendering [";
        float progress = float(ny - j) / float(ny);
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
