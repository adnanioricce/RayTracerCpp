#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "EasyBMP.hpp"
#include <string>
#include <sstream>
#include <ostream>
#include <cmath>
#include <limits>
#include <vector>
#include <tuple>
#include "Ray.h"
#include "HitRecord.h"
#include "HittableObjects.h"
#include "color.h"
#include "Utils.h"
struct Camera {
    float aspect_ratio;
    float viewport_height;
    float viewport_width;
    float focal_length;

    glm::vec3 origin;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 lower_left_corner;
    Camera(){
        this->aspect_ratio = 16.0 / 9.0;
        this->viewport_height = 2.0;
        this->viewport_width = aspect_ratio * viewport_height;
        this->focal_length = 1.0;

        this->origin = glm::vec3(0, 0, 0);
        this->horizontal = glm::vec3(viewport_width, 0.0, 0.0);
        this->vertical = glm::vec3(0.0, viewport_height, 0.0);
        this->lower_left_corner = origin - horizontal/2.0f - vertical/2.0f - glm::vec3(0, 0, focal_length);
    }
    Camera(glm::vec3 origin){
        this->aspect_ratio = 16.0 / 9.0;
        this->viewport_height = 2.0;
        this->viewport_width = aspect_ratio * viewport_height;
        this->focal_length = 1.0;

        this->origin = origin;
        this->horizontal = glm::vec3(viewport_width, 0.0, 0.0);
        this->vertical = glm::vec3(0.0, viewport_height, 0.0);
        this->lower_left_corner = origin - horizontal/2.0f - vertical/2.0f - glm::vec3(0, 0, focal_length);
    }
};
Ray get_ray(Camera cam,glm::vec2 uv){
    return Ray(cam.origin,cam.lower_left_corner + uv.x * cam.horizontal + uv.y * cam.vertical - cam.origin);
}
//OBS: Origin here is different from the tutorial.
//the origin in here is located at (width / 2,height / 2)
const auto aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
const int image_height = static_cast<int>(image_width / aspect_ratio);


//has_hit_sphere = hit function of sphere class on tutorial
// Structures Functions

void setPixel(EasyBMP::Image& img,glm::vec2 pos,glm::vec3 color,const int samples_per_pixel){
    auto scale = 1.0f / samples_per_pixel;
    color.r *= scale;
    color.g *= scale;
    color.b *= scale;
    auto c = glm::clamp(color,glm::vec3(0.0),glm::vec3(255.9f));
    img.SetPixel(pos.x,pos.y,EasyBMP::RGBColor(color.x,color.y,color.z));
}
//TODO: Sections in RayTracingInOneWeekend to be implemented:
    //TODO: Section 8 - Diffuse Materials
        //TODO: 8.1 -> A Simple Diffuse Material
            //TODO: Create a perlin noise like function to avoid using a random function for sampling
        //TODO: 8.2 -> Limit the Number of Child Rays
        //TODO: ...

//TODO: Create a class to define scenes
//TODO(OPTIONAL): Create a struct to define different object structures to be rendered
int main(){
  // Image

    EasyBMP::RGBColor black(64,88,127);
    EasyBMP::Image img(image_width,image_height,"output.bmp",black);
    std::cout << "image width:" << image_width << ", height:" << image_height << std::endl;
    const int samples_per_pixel = 2;
    // Camera
    auto origin = glm::vec3(image_width / 2,image_height / 2,0);
    Camera cam(origin);
    // World
    std::vector<HittableSphere> spheres = {
      HittableSphere(glm::vec3(cam.origin.x,cam.origin.y,-1),0.5f),
      HittableSphere(glm::vec3(cam.origin.x,cam.origin.y + 100.0f,-1.0f),100.0f)
    };

    std::cout << "lower_left_corner: (" << cam.lower_left_corner.x << ","
          << cam.lower_left_corner.y << ","
          << cam.lower_left_corner.z << ")"
          << std::endl;
      // Render
      //std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    std::stringstream logCsv;
    logCsv << "Position,Origin,Direction,Color" << std::endl;

    for(int j = image_height-1;j >= 0; --j){
       for(int i = 0;i < image_width;++i){
           glm::vec3 color(0,0,0);
           glm::vec2 pos;
           Ray r;
           for(int s = 0;s < samples_per_pixel;++s) {
               pos = glm::vec2(
                               (float(i + random_float()) / (image_width - 1)),
                               (float(j + random_float()) / (image_height - 1)));
               r = get_ray(cam,pos);
               //glm::vec3 color = ray_color(r);
               //setPixel(img,pos,color);
               color += ray_color(r, cam.origin, spheres);

           }
           logCsv << "r Pos = " + vec2_str(pos) + ", Origin = " << vec3_str(r.origin)
                  << ", Direction = " << vec3_str(r.direction)
                  << ", Color = " << vec3_str(color)
                  << std::endl;
           setPixel(img,glm::vec2(i,j),color,samples_per_pixel);
       }
    }
   std::ofstream csvOut("output.csv");
   csvOut << logCsv.str();
   csvOut.close();
   img.Write();
   return 0;
}
