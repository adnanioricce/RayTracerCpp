//
// Created by adnan on 7/4/2022.
//

#ifndef RAYTRACER_UTILS_H
#define RAYTRACER_UTILS_H
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <sstream>
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
#endif //RAYTRACER_UTILS_H
