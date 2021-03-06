#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <string>

class Positionable;

class Camera
{
public:
  Camera(size_t windowWidth, size_t windowHeight);

  const glm::vec3 getPosition() const;
  const glm::vec3 getWhereILook() const;
  const glm::mat4& getViewMatrix() const;
  const glm::mat4& getProjectionMatrix() const;
  const glm::mat4& getViewProjectionMatrix() const;
  void updateViewProjectionMatrix();

  void setSize(const unsigned int width, const unsigned int height);

  void linkToPositionable(const Positionable& objectToFollow)
    {this->objectToFollow = &objectToFollow;}

  void switchInBackwardView();
  void switchInForwardView();
  void switchInRightBendView();
  void switchInLeftBendView();

private:
  const Positionable* objectToFollow;
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
  glm::mat4 viewProjection;

  size_t windowWidth;
  size_t windowHeight;

  bool viewThirdPerson;
  bool viewBackward;
  bool viewRightBend;
  bool viewLeftBend;
};

#endif // CAMERA_HPP
