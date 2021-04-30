#include <iostream>
#include <glm/glm.hpp>
#include <fstream>
#include "Ray.h"

inline
float deg2rad(const float &deg)
{ return deg * M_PI / 180; }

bool hit_sphere(const glm::vec3& center, double radius, const ray& r) {
    //std::cout<<r.direction().x<<" "<<r.direction().y<<" "<<r.direction().z<<std::endl;
    glm::vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

glm::vec3 rayColor(const ray& r){
    if(hit_sphere(glm::vec3(1.5,0.5,0), 1, r)){
        //std::cout<<"HIT";
        return glm::vec3(1,0,0);
    }else{
        //std::cout<<"MISS";
        return glm::vec3(0,0,0);
    }
}


int main() {
    
    // Image
    //const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 640;
    //const int image_height = 480;
    const auto aspect_ratio = 4.0/3.0;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    int fovy = 90;
    auto viewport_height = 2*tan(deg2rad(fovy*0.5));
    std::cout<<viewport_height<<std::endl;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = glm::vec3(0, 0, 8);
    auto horizontal = glm::vec3(viewport_width, 0, 0);
    auto vertical = glm::vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal*0.5f - vertical*0.5f - glm::vec3(0, 0, focal_length);
    
    
    // Render
    
    glm::vec3 * frameBuffer = new glm::vec3[image_width*image_height];
    glm::vec3* pix = frameBuffer;
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    
    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            
            auto u = float(i) / (image_width-1);
            auto v = float(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            
            
            *(pix++) = rayColor(r);
            
            //std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ofs("/Users/adityasinha/Desktop/out1.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << image_width << " " << image_height << "\n255\n";
    for (uint32_t i = 0; i < image_width * image_height; ++i) {
        //std::cout<<frameBuffer[i].x<<""<<frameBuffer[i].y<<""<<frameBuffer[i].z<<std::endl;
        char r = 255*frameBuffer[i].x;
        char g = 255*frameBuffer[i].y;
        char b = 255*frameBuffer[i].z;
        ofs << r << g << b;
    }
    
    ofs.close();
    //delete [] frameBuffer;
    std::cerr<<"\nDone.\n";
    
    
}
