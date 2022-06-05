#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include  "EasyBMP.hpp"
#include <string>
#include <sstream>
struct Ray{
    glm::vec3 origin;
    glm::vec3 direction;
    Ray(){
        this->origin = glm::vec3(0);
        this->direction = glm::vec3(0);
    }
    Ray(glm::vec3 origin,glm::vec3 direction){
        this->origin = origin;
        this->direction = direction;
    }
};
glm::vec3 at(Ray r,float t){
    return r.origin + (t * r.direction);
}
glm::vec3 color(float r,float g,float b){
    return glm::vec3(r,g,b);
}
glm::vec3 ray_color(Ray r){
    glm::vec3 unit_direction = 
        //r.direction;
        glm::normalize(r.direction);
        //r.direction / glm::length(r.direction);
    auto t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * color (255.f,255.f,255.f) + t * color(255 * 0.5f,255 * 0.7f,255.f * 1.0f);
}
std::string vec3_str(glm::vec3 v){
    std::stringstream ss;
    ss << "(" << v.x << "," << v.y << "," << v.z << ")";
    return ss.str();
}
std::string vec2_str(glm::vec2 v){
    std::stringstream ss;
    ss << "(" << v.x << "," << v.y << ")";
    return ss.str();
}
void setPixel(EasyBMP::Image& img,glm::vec2 pos,glm::vec3 color){
    img.SetPixel(pos.x,pos.y,EasyBMP::RGBColor(color.x,color.y,color.z));
}
int main(){
  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

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
  std::cout << "lower_left_corner: (" << lower_left_corner.x << ","
      << lower_left_corner.y << ","
      << lower_left_corner.z << ")"
      << std::endl;
  // Render
  //std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  for(int j = image_height-1;j >= 0; --j){
      for(int i = 0;i < image_width;++i){

          glm::vec2 pos = 
                  //glm::vec2(i,j);
                  glm::vec2(
                  (float(i) / (image_width - 1)),
                  (float(j) / (image_height - 1)));
          Ray r(origin,lower_left_corner + (pos.x * horizontal) + (pos.y * vertical) - origin);
          glm::vec3 color = ray_color(r);
          //setPixel(img,pos,color);
          std::cout << "r Pos = " + vec2_str(pos) + ", Origin = " << vec3_str(r.origin) 
              << ", Direction = " << vec3_str(r.direction)
              << ", Color = " << vec3_str(color)
              << std::endl;
          setPixel(img,glm::vec2(i,j),color);
      }
  } 
  img.Write();
  return 0;
}
