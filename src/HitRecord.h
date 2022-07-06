//
// Created by adnan on 7/4/2022.
//

#ifndef RAYTRACER_HITRECORD_H
#define RAYTRACER_HITRECORD_H
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "Ray.h"
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
HitRecord set_face_normal(const HitRecord record,const Ray& r,const glm::vec3 outward_normal){
    HitRecord newRecord;
    newRecord.front_face = glm::dot(r.direction,outward_normal) < 0.0;
    newRecord.normal = record.front_face ? outward_normal : -outward_normal;
    return newRecord;
}

#endif //RAYTRACER_HITRECORD_H
