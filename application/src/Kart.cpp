#include "Kart.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Kart::Kart()
    : position(0.f, 0.f, 0.f), orientation(glm::angleAxis(0.f, glm::vec3(0.f, 1.f, 0.f))),
      directionAngle(0.f), speed(0.f), angularSpeed(0.f)
{
}

Kart::Kart(glm::vec3 position, glm::quat orientation, float speed)
  : position(position), orientation(orientation), speed(speed)
{
}

Kart::~Kart()
{
}

void Kart::update()
{
  //Calcul de la nouvelle direction en fonction de l'angularSpeed
  directionAngle += angularSpeed; //ajouter gestion temps
  orientation = glm::angleAxis(directionAngle, glm::vec3(0.f, 1.f, 0.f));
  const glm::vec3 initialDirection = glm::vec3(0.f, 0.f, -1.f);
  glm::vec3 direction = glm::normalize(glm::toMat3(orientation) * initialDirection);

  //Calcul de la nouvelle position en fonction de la speed et de la direction
  position += direction * speed;//ajouter gestion temps
}

void Kart::moveForward(){
  speed = 0.1f;
}

void Kart::moveBackward(){
  speed = -0.1f;
}

void Kart::turnLeft(){
  angularSpeed = 10.f;
}

void Kart::turnRight(){
  angularSpeed = -10.f;
}

void Kart::stopMoving()
{
  speed = 0.f;
}

void Kart::stopTurning()
{
  angularSpeed = 0.f;
}

const glm::vec3& Kart::getPosition() const
  {return position;}

const glm::quat& Kart::getOrientation() const
{return orientation;}

