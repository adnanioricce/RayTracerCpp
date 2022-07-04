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
//OBS: Origin here is different from the tutorial.
//the origin in here is located at (width / 2,height / 2)
const auto aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
const int image_height = static_cast<int>(image_width / aspect_ratio);

// RayTracing structures...
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
struct HitRecord {
    glm::vec3 position;
    glm::vec3 normal;   
    float t;
    bool front_face;
    HitRecord(){
        this->position = glm::vec3(0);
        this->normal = glm::vec3(0);
        this->t = 0.0f;
        this->front_face = false;
    }
    HitRecord(glm::vec3 position,glm::vec3 normal,float t){
        this->position = position;
        this->normal = normal;
        this->t = t;
        this->front_face = false;
    }
};
struct HittableSphere{
    glm::vec3 center;
    float radius;
    HittableSphere(){
        this->center = glm::vec3(0);
        this->radius = 0.0f;
    }
    
    HittableSphere(glm::vec3 center,float radius){
        this->center = center; 
        this->radius = radius;
    }
};
const float infinity = std::numeric_limits<float>::infinity();
//has_hit_sphere = hit function of sphere class on tutorial
// Structures Functions
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
glm::vec3 at(Ray r,float t){
    return r.origin + (t * r.direction);
}
glm::vec3 color(float r,float g,float b){
    return glm::vec3(r,g,b);
}
HitRecord set_face_normal(const HitRecord record,const Ray& r,const glm::vec3 outward_normal){
    HitRecord newRecord;
    newRecord.front_face = glm::dot(r.direction,outward_normal) < 0.0;
    newRecord.normal = record.front_face ? outward_normal : -outward_normal;
    return newRecord;
}
float hit_sphere(const Ray& r,const glm::vec3 center,const float radius){
    glm::vec3 oc = r.origin - center;
    auto a = glm::length2(r.direction);
    auto half_b = glm::dot(oc,r.direction);
    auto c = glm::length2(oc) - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    return (discriminant < 0.0f)
        ? -1.0f
        : (-half_b - sqrt(discriminant)) / a;
}
void setPixel(EasyBMP::Image& img,glm::vec2 pos,glm::vec3 color){
    img.SetPixel(pos.x,pos.y,EasyBMP::RGBColor(color.x,color.y,color.z));
}
std::tuple<HitRecord,bool> has_hit_sphere(const HittableSphere& hittableObject,
        HitRecord& rec,
        const Ray& ray,
        float t_min,
        float t_max){
    glm::vec3 center = hittableObject.center;
    float radius = hittableObject.radius;
    glm::vec3 oc = ray.origin - center;
    float discriminant = hit_sphere(ray,center,radius);
    float a  = glm::length2(ray.direction);
    float half_b = glm::dot(oc, ray.direction);
    //float c = glm::length2(oc) - radius * radius;
    //float discriminant = half_b * half_b - a * c;
    if(discriminant < 0)
        return std::make_tuple(rec,false);
    float sqrtd = sqrt(discriminant);
    float root = (-half_b - sqrtd) / a;
    if(root < t_min || t_max < root){
        root = (-half_b + sqrtd) / a;
        if(root < t_min || t_max < root)
            return std::make_tuple(rec,false);
    }
    rec.t = root;
    rec.position = at(ray,rec.t);
    glm::vec3 outward_normal = (rec.position - center) / radius;    
    //TODO:is rec being updated as expected?Check it
    return std::make_tuple(set_face_normal(rec,ray,outward_normal),true);
    //return true;
    //return std::make_tuple(rec,true);
}
std::tuple<HitRecord,bool> has_hit_in_list(
        std::vector<HittableSphere> spheres,
        const Ray& r,
        float t_min,
        float t_max) {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    for(size_t i = 0;i < spheres.size();++i){
        auto result = has_hit_sphere(spheres[i],temp_rec,r,t_min,closest_so_far);
        if(std::get<1>(result)){
            auto resultRec = std::get<0>(result);
            hit_anything = true;
            closest_so_far = resultRec.t;
            temp_rec = resultRec;
        }
    }
    return std::make_tuple(temp_rec,hit_anything);
}
glm::vec3 ray_color(const Ray r){
    auto center = glm::vec3(image_width / 2.0f,(image_height / 2.0f) - .5f,-1.0f);
    auto t = hit_sphere(r,center,0.5f);
    if(t > 0.0f){
        glm::vec3 n = glm::normalize(at(r,t) - glm::vec3(center.x,center.y,center.z));
        std::cout << "n:" << vec3_str(n) << std::endl;
        auto c = 0.5f * color((1.0f + n.x) * 255.0f,(1.0f + n.y) * 255.f,(1.0f + n.z) * 255.0f);
        std::cout << "c:" << vec3_str(c) << std::endl;
        return c;
    }
     
    glm::vec3 unit_direction = 
        //r.direction;
        glm::normalize(r.direction);
        //r.direction / glm::length(r.direction);
    t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * color (255.f,255.f,255.f) + t * color(255 * 0.5f,255 * 0.7f,255.f * 1.0f);
}
glm::vec3 ray_color(const Ray& r,const std::vector<HittableSphere> spheres){
    //HitRecord rec;
    //if(has_hit_in_list(spheres,r,0,infinity)){
    auto result = has_hit_in_list(spheres,r,0.5f,infinity);
    if(std::get<1>(result)){
        auto rec = std::get<0>(result);
        
        return 0.5f * (rec.normal + color(255.0f,255.0f,255.0f));
    }
    glm::vec3 unit_direction = 
        //r.direction;
        glm::normalize(r.direction);
    auto t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f-t) * color(255.0f,255.0f,255.0f) + t * color(0.5f * 255.0f,0.7f * 255.0f,255.0f);
}
int main(){
  // Image

  EasyBMP::RGBColor black(64,88,127);
  EasyBMP::Image img(image_width,image_height,"output.bmp",black);

  // World
  std::vector<HittableSphere> spheres = {
    HittableSphere(glm::vec3(image_width / 2.0f,image_height / 2.0f,-1),0.5f),
    //HittableSphere(glm::vec3(image_width / 2.0f,(image_height / 2.0f) - 60.5,1.0f),100)
  };

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
          glm::vec3 pixel_color = ray_color(r,spheres);
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
