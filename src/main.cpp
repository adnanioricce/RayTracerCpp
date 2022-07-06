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
//OBS: Origin here is different from the tutorial.
//the origin in here is located at (width / 2,height / 2)
const auto aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
const int image_height = static_cast<int>(image_width / aspect_ratio);


//has_hit_sphere = hit function of sphere class on tutorial
// Structures Functions

void setPixel(EasyBMP::Image& img,glm::vec2 pos,glm::vec3 color){
    img.SetPixel(pos.x,pos.y,EasyBMP::RGBColor(color.x,color.y,color.z));
}

int main(){
  // Image

  EasyBMP::RGBColor black(64,88,127);
  EasyBMP::Image img(image_width,image_height,"output.bmp",black);
    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = glm::vec3(image_width / 2,image_height / 2,0);
    //auto origin = glm::vec3(0,0,0);
    auto horizontal = glm::vec3(viewport_width,0,0);
    auto vertical = glm::vec3(0,viewport_height,0);
    auto lower_left_corner = origin -
                             (horizontal / 2.0f)
                             //glm::vec3(1,0,0)
                             - (vertical / 2.0f) - glm::vec3(0,0, focal_length);
  // World
  std::vector<HittableSphere> spheres = {
    HittableSphere(glm::vec3(origin.x,origin.y,-1),0.5f),
    //HittableSphere(glm::vec3(image_width / 2.0f,(image_height / 2.0f) - .15f,1.0f),100)
  };


  std::cout << "lower_left_corner: (" << lower_left_corner.x << ","
      << lower_left_corner.y << ","
      << lower_left_corner.z << ")"
      << std::endl;
  // Render
  //std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  std::stringstream logCsv;
  logCsv << "Position,Origin,Direction,Color" << std::endl;

  for(int j = image_height-1;j >= 0; --j){
      for(int i = 0;i < image_width;++i){
          glm::vec2 pos = 
                  //glm::vec2(i,j);
                  glm::vec2(
                  (float(i) / (image_width - 1)),
                  (float(j) / (image_height - 1)));
          Ray r(origin,lower_left_corner + (pos.x * horizontal) + (pos.y * vertical) - origin);
          //glm::vec3 color = ray_color(r);
          //setPixel(img,pos,color);
          glm::vec3 pixel_color = ray_color(r,origin,spheres);
          logCsv << "r Pos = " + vec2_str(pos) + ", Origin = " << vec3_str(r.origin) 
              << ", Direction = " << vec3_str(r.direction)
              << ", Color = " << vec3_str(pixel_color)
              << std::endl;
          setPixel(img,glm::vec2(i,j),pixel_color);
      }
  }
  std::ofstream csvOut("output.csv");
  csvOut << logCsv.str();
  csvOut.close();
  img.Write();
  return 0;
}
