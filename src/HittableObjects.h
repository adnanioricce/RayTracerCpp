#ifndef RAYTRACER_HITTABLEOBJECTS_H
#define RAYTRACER_HITTABLEOBJECTS_H
#include <glm/vec3.hpp>
#include <tuple>
#include <limits>
#include "Ray.h"
#include "HitRecord.h"
#include "color.h"
#include "Utils.h"
const float infinity = std::numeric_limits<float>::infinity();
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
    //float discriminant = hit_sphere(ray,center,radius);
    float a  = glm::length2(ray.direction);
    float half_b = glm::dot(oc, ray.direction);
    float c = glm::length2(oc) - radius * radius;
    float discriminant = half_b * half_b - a * c;
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
void LogRayColorHit(glm::vec3 n,glm::vec3 c) {
    std::cout << "n:" << vec3_str(n) << std::endl;
    std::cout << "c:" << vec3_str(c) << std::endl;
}
glm::vec3 ray_color(const Ray r,glm::vec3 center){
    auto t = hit_sphere(r,center,0.5f);
    if(t > 0.0f){
        glm::vec3 n = glm::normalize(at(r,t) - glm::vec3(center.x,center.y,center.z));
        auto c = 0.5f * color((1.0f + n.x) * 255.0f,(1.0f + n.y) * 255.f,(1.0f + n.z) * 255.0f);
        LogRayColorHit(n,c);
        return c;
    }
    glm::vec3 unit_direction =
            glm::normalize(r.direction);
    t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * color (255.f,255.f,255.f) + t * color(255 * 0.5f,255 * 0.7f,255.f * 1.0f);
}
// renders an image resembling an eye
glm::vec3 eyeHit(const HitRecord rec,const glm::vec3 center) {
    glm::vec3 n =
            rec.normal - rec.position;
    auto correction = glm::sin(n);
    auto c = (n - center) * correction * 255.0f;
    LogRayColorHit(n,c);
    return n;
}
typedef glm::vec3 (*OnHitDelegate)(const HitRecord rec,const glm::vec3 center);
glm::vec3 defaultOnHit(const HitRecord rec,const glm::vec3 center){
    //auto n = rec.normal;
    glm::vec3 n =
            rec.normal - rec.position;
    //glm::normalize(rec.normal - rec.position);
    //glm::vec3(center.x,center.y,center.z));
    //n -= at(r,rec.t);
    //auto c = 0.5f * color((1.0f + n.x) * 255.0f,(1.0f + n.y) * 255.f,(1.0f + n.z) * 255.0f);
    //auto c = (255.0f * n) * 0.5f;
    auto correction = 0.4f;
    //auto correction = glm::clamp(glm::length(rec.position),0.2f,0.4f);
    auto c = (n - center) * correction * 255.0f;
    LogRayColorHit(n,c);
    //return n * -255.0f;
    return c;
}
glm::vec3 ray_color_with(const Ray& r,glm::vec3 center,const std::vector<HittableSphere> spheres,OnHitDelegate func) {
    auto result = has_hit_in_list(spheres,r,0.5f,infinity);
    if(std::get<1>(result)){
        auto rec = std::get<0>(result);
        return func(rec,center);
    }
    glm::vec3 unit_direction =
            glm::normalize(r.direction);
    auto t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f-t) * color(255.0f,255.0f,255.0f) + t * color(0.5f * 255.0f,0.7f * 255.0f,255.0f);
}
glm::vec3 ray_color(const Ray& r,glm::vec3 center,const std::vector<HittableSphere> spheres){
    return ray_color_with(r,center,spheres,defaultOnHit);
}
#endif //RAYTRACER_HITTABLEOBJECTS_H
