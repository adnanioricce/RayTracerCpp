//
// Created by adnan on 7/4/2022.
//

#ifndef RAYTRACER_UTILS_H
#define RAYTRACER_UTILS_H
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <sstream>
#include <random>
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
inline float random_float(float min,float max){
    static std::uniform_real_distribution<float> distribution(min,max);
    static std::mt19937 generator;
    return distribution(generator);
}
inline float random_float(){
    return random_float(0.0f,1.0f);
}
inline glm::vec3 random_vec3(){
    return glm::vec3(random_float(),random_float(),random_float());
}
inline glm::vec3 random(float min, float max) {
    return glm::vec3(random_float(min,max), random_float(min,max), random_float(min,max));
}
#endif //RAYTRACER_UTILS_H
