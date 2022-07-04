//
// Created by adnan on 7/4/2022.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <iostream>
#include "HittableObjects.h"
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
glm::vec3 at(Ray r,float t){
    return r.origin + (t * r.direction);
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
#endif //RAYTRACER_RAY_H
