#include "PointLight.hpp"

PointLight::PointLight()
  :position(0.f,10.f,0.f,1.f),info(0.f,10.f,0.f,1.f),intensity(0.4f,0.4f,0.4f)
{
}

PointLight::PointLight(glm::vec4 position)
    :position(position),info(position),intensity(0.4f,0.4f,0.4f)
{
}

PointLight::PointLight(glm::vec4 position, glm::vec3 intensity)
    :position(position),info(position),intensity(intensity)
{
}

const glm::vec4 PointLight::getLightPosition() const{
  return position;
}

const glm::vec3 PointLight::getLightIntensity() const {
  return intensity;
}

void PointLight::updateLight(const glm::mat4 viewMatrix) {
    position = viewMatrix * info;
}
