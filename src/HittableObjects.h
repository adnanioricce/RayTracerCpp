#ifndef RAYTRACER_HITTABLEOBJECTS_H
#define RAYTRACER_HITTABLEOBJECTS_H
#include <glm/vec3.hpp>
#include <tuple>
#include "Ray.h"
#include "HitRecord.h"
#include "color.h"
#include "Utils.h"
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
glm::vec3 ray_color(const Ray r,glm::vec3 center){
    //auto center = glm::vec3(image_width / 2.0f,(image_height / 2.0f) - .5f,-1.0f);
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
#endif //RAYTRACER_HITTABLEOBJECTS_H
