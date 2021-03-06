#ifndef DIRLIGHT_HPP
#define DIRLIGHT_HPP

#include <glm/glm.hpp>


class DirectionalLight
{
public:
  DirectionalLight();
  DirectionalLight(glm::vec4 direction);
  DirectionalLight(glm::vec3 intensity);
  DirectionalLight(glm::vec4 direction, glm::vec3 intensity);

  const glm::vec4 getLightDirection() const;
  const glm::vec3 getLightIntensity() const;
  void setLightIntensity(glm::vec3 intensity_);
  void updateLight(const glm::mat4 viewMatrix);


private:
  glm::vec4 direction;
  glm::vec4 info;
  glm::vec3 intensity;
};

#endif // DIRLIGHT_HPP
