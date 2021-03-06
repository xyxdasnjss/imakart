#ifndef KARTSTATE_HPP
#define KARTSTATE_HPP

#include "Kart.hpp"

//Classe amie de Kart pour pas se faire chier (acces aux attributs prives)
class KartState
{
public:
  KartState(Kart& kart)
    : kart(kart), eventStack(nullptr) {}
  virtual ~KartState() {}

  void setEventStack(std::stack<GameEvent>* eventStack)
    {this->eventStack = eventStack;}

  virtual void start() = 0;
  virtual void update(float elapsedTimeInSecond) = 0;
  virtual void moveForward() = 0;
  virtual void moveBackward() = 0;
  virtual void turnLeft() = 0;
  virtual void turnRight() = 0;
  virtual void stopMove() = 0;
  virtual void brake() = 0;

  //Implem par defaut
  virtual void stopTurning()
  {
    kart.currentAngularSpeed = 0.f;
  }

  //Implem par defaut
  virtual void bounce()
  {
    kart.setState(kart.bounceState);
  }

  //Implem par defaut
  virtual void boost()
  {
    kart.setState(kart.boostState);
  }
  virtual void drift() = 0;

protected:
  //C'est souvent ce code la qui revient
  inline glm::vec3 setKartOrientationAndComputeDirection(float elapsedTimeInSecond)
  {
    kart.directionAngle += kart.currentAngularSpeed * (elapsedTimeInSecond); //en degres/secondes

    kart.orientation = glm::angleAxis(kart.directionAngle, glm::vec3(0.f, 1.f, 0.f));

    const glm::vec3 initialDirection = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 direction = glm::normalize(glm::toMat3(kart.orientation) * initialDirection);
    return direction;
  }

  Kart& kart;
  std::stack<GameEvent>* eventStack;

};

//Classe intermediaire pour unir le code d'acceleration qui est commun a toutes les formes d'acceleration
class Acceleration : public KartState
{
public:
  Acceleration(Kart& kart)
    : KartState(kart) {}
  virtual ~Acceleration() {}

  virtual void update(float elapsedTimeInSecond)
  {
    glm::vec3 direction = setKartOrientationAndComputeDirection(elapsedTimeInSecond);

    float travelledDistance = kart.speed * elapsedTimeInSecond + kart.currentAcceleration * (elapsedTimeInSecond * elapsedTimeInSecond) / 2.f;
    kart.position += direction * travelledDistance;

    kart.speed = travelledDistance / elapsedTimeInSecond;

    if (abs(kart.speed) >= kart.specifications.maxSpeed)
    {
      setMaxSpeedState();//définie par les classes filles
    }
  }

  virtual void setMaxSpeedState() = 0;

};

class ForwardAcceleration : public Acceleration
{
public:
  ForwardAcceleration(Kart& kart)
    : Acceleration(kart) {}

  //Pas besoin de redéfinir update !
  virtual void setMaxSpeedState()
  {
    kart.setState(kart.forwardMaxSpeedReached);
  }

  virtual void start()
  {
    kart.currentAcceleration = kart.specifications.acceleration;
  }

  virtual void moveForward()
  {
    //J'y suis deja et pour l'instant est considere constante donc je fais rien !
  }

  virtual void moveBackward()
  {
    brake();
  }

  virtual void turnLeft()
  {
    if(kart.speed >= 15)
      kart.speed -= 0.25;
    kart.currentAngularSpeed = kart.specifications.angularSpeed;
  }

  virtual void turnRight()
  {
    if(kart.speed >= 15)
      kart.speed -= 0.25;
    kart.currentAngularSpeed = - kart.specifications.angularSpeed;
  }

  virtual void stopMove()
  {
    kart.setState(kart.forwardDecelerationState);
  }

  virtual void brake()
  {
    kart.setState(kart.forwardBrakeState);
  }

  void drift()
  {
    //TODO
  }
};

///Le seul truc qui change par rapport a la classe mere, c'est update !
class ForwardMaxSpeedReached : public ForwardAcceleration //Tada !
{
public:
  ForwardMaxSpeedReached(Kart& kart)
    : ForwardAcceleration(kart) {}

  virtual void update(float elapsedTimeInSecond)
  {
    if(kart.speed < kart.specifications.maxSpeed){
      //Si il perd de la vitesse parcequ'il tourne il faut le faire accelerer
      kart.setState(kart.forwardAccelerationState);
    }
    glm::vec3 direction = setKartOrientationAndComputeDirection(elapsedTimeInSecond);

    //Seul changement : cette ligne !
    float travelledDistance = kart.specifications.maxSpeed * elapsedTimeInSecond;
    kart.position += direction * travelledDistance;
  }
};

class BackwardAcceleration : public Acceleration
{
public:
  BackwardAcceleration(Kart& kart)
    : Acceleration(kart) {}

  virtual void update(float elapsedTimeInSecond)
  {
    glm::vec3 direction = setKartOrientationAndComputeDirection(elapsedTimeInSecond);

    float travelledDistance = kart.speed * elapsedTimeInSecond + kart.currentAcceleration * (elapsedTimeInSecond * elapsedTimeInSecond) / 2.f;
    kart.position += direction * travelledDistance;

    kart.speed = travelledDistance / elapsedTimeInSecond;

    if (abs(kart.speed) >= 5)
    {
      setMaxSpeedState();//définie par les classes filles
    }
  }

  virtual void setMaxSpeedState()
  {
    kart.setState(kart.backwardMaxSpeedReached);
  }

  virtual void start()
  {
    kart.currentAcceleration = -kart.specifications.acceleration;
  }

  virtual void moveForward()
  {
    brake();
  }

  virtual void moveBackward()
  {
    //cf ForwardAccelerationState
  }

  virtual void turnLeft()
  {
    kart.currentAngularSpeed = -kart.specifications.angularSpeed;
  }

  virtual void turnRight()
  {
    kart.currentAngularSpeed = kart.specifications.angularSpeed;
  }

  virtual void stopMove()
  {
    kart.setState(kart.backwardDecelerationState);
  }

  virtual void brake()
  {
    kart.setState(kart.backwardBrakeState);
  }

  void drift()
  {
    //TODO
  }
};

class BackwardMaxSpeedReached : public BackwardAcceleration
{
public:
  BackwardMaxSpeedReached(Kart& kart)
    : BackwardAcceleration(kart) {}

  virtual void update(float elapsedTimeInSecond)
  {
    glm::vec3 direction = setKartOrientationAndComputeDirection(elapsedTimeInSecond);

    float travelledDistance = kart.speed * elapsedTimeInSecond;
    kart.position += direction * travelledDistance;

  }
};


class Deceleration : public KartState
{
public:
  Deceleration(Kart& kart)
    : KartState(kart) {}
  virtual ~Deceleration() {}

  //On se permet de regrouper l'update car les state sont supposes s'appeler dans le bon ordre,
  //Donc la valeur de kart.currentAcceleration devrait toujours etre bonne quand on arrive ici
  virtual void update(float elapsedTimeInSecond)
  {
    glm::vec3 direction = setKartOrientationAndComputeDirection(elapsedTimeInSecond);
    //Attention ici au moins : nous sommes en phase de deceleration
    float travelledDistance = kart.speed * elapsedTimeInSecond + kart.currentAcceleration * (elapsedTimeInSecond * elapsedTimeInSecond) / 2.f;
    float supposedNewSpeed = travelledDistance / elapsedTimeInSecond;
    if (abs(kart.speed - 0.f) <= 0.000001f || supposedNewSpeed / kart.speed < 0.f)
    {
      kart.setState(kart.noMoveState);
    }
    else
    {
      kart.speed = supposedNewSpeed;
    }

    kart.position += direction * travelledDistance;
  }

};

class ForwardDeceleration : public Deceleration
{
public:
  ForwardDeceleration(Kart& kart)
    : Deceleration(kart) {}

  virtual void start()
  {
    kart.currentAcceleration = -kart.currentAcceleration;
  }

  virtual void moveForward()
  {
    kart.setState(kart.forwardAccelerationState);
  }

  virtual void moveBackward()
  {
    brake();
  }

  virtual void turnLeft()
  {
    if(kart.speed >= 15)
      kart.speed -= 0.25;
    kart.currentAngularSpeed = kart.specifications.angularSpeed;
  }

  virtual void turnRight()
  {
    if(kart.speed >= 15)
      kart.speed -= 0.25;
    kart.currentAngularSpeed = -kart.specifications.angularSpeed;
  }

  virtual void stopMove()
  {
    //What the fuck am I doing here ?? :p
  }

  virtual void brake()
  {
    kart.setState(kart.forwardBrakeState);
  }

  void drift()
  {
    //TODO
  }

};

class BackwardDeceleration : public Deceleration
{
public:

  BackwardDeceleration(Kart& kart)
    : Deceleration(kart) {}

  virtual void start()
  {
    kart.currentAcceleration = -kart.currentAcceleration;
  }

  virtual void moveForward()
  {
    brake();
  }

  virtual void moveBackward()
  {
    kart.setState(kart.backwardAccelerationState);
  }

  virtual void turnLeft()
  {
    kart.currentAngularSpeed = -kart.specifications.angularSpeed;
  }

  virtual void turnRight()
  {
    kart.currentAngularSpeed = kart.specifications.angularSpeed;
  }

  virtual void stopMove()
  {
    //What the fuck am I doing here ?? :p
  }

  virtual void brake()
  {
    kart.setState(kart.backwardBrakeState);
  }

  virtual void drift()
  {
    //TODO
  }
};

//Etat qui fait rien a part changer d'etat
class NoMove : public KartState
{
public:
  NoMove(Kart& kart)
    : KartState(kart) {}

  virtual void start()
  {
    kart.speed = 0.f;
    kart.currentAcceleration = 0.f;
  }

  virtual void update(float elapsedTimeInSecond) {}

  virtual void moveForward()
  {
    kart.setState(kart.forwardAccelerationState);
  }

  virtual void moveBackward()
  {
    kart.setState(kart.backwardAccelerationState);
  }

  //On peut pas bouncer immobile
  virtual void bounce() {}
  virtual void turnLeft() {}
  virtual void turnRight() {}
  virtual void stopMove() {}
  virtual void brake() {}
  virtual void drift() {}
};

class Bumped : public NoMove
{
public:
  Bumped(Kart& kart)
    : NoMove(kart) {}

  virtual void start()
  {
    if(!effect){
      effect = true;
      time = 50.f;
      kart.speed = 0.f;
      kart.currentAcceleration = 0.f;
    }
  }

  virtual void update(float elapsedTimeInSecond)
  {
    if(time > 40.f){
      kart.position.y += 0.2;
    }else if(time <= 40.f && time > 0.f){
      kart.position.y -= 0.05;
    }else{
      kart.position.y = 0;
      effect = false;
      kart.setState(kart.forwardAccelerationState);
    }
    time -= 1;
  }
private:
  float time;
  bool effect;
};

class Bounce : public KartState
{
public:
  Bounce(Kart& kart)
    : KartState(kart), distanceToTravel(3.f), currentDistance(0.f), bouncing(false) {}

  virtual void start()
  {
    //Si on est deja en train de bouncer on reinitialise pas
    if (bouncing)
      return;

    //Au cas où (debug un peu sale mais rapide) : on s'assure qu'en cas de bounce pendant un boost
    //le motion blur est désactivé. En vrai faudrait chainer bounce et boost de la meme maniere que les autres etats
    //au lieu d'en dur dans Kart
    GameEventData data;//On s'en fout de la data ici
    GameEvent boostEvent(USE_BOOST_END, data);
    eventStack->push(boostEvent);

    currentDistance = 0.f;
    distanceToTravel = log(abs(kart.speed)) - 0.5f;//au hasard
    kart.speed = -kart.speed;
    bouncing = true;
  }

  virtual void update(float elapsedTimeInSecond)
  {
    if (abs(currentDistance) >= distanceToTravel)
    {
      kart.setState(kart.noMoveState);
      //Se remet a zero
      bouncing = false;
      return;
    }

    glm::vec3 direction = setKartOrientationAndComputeDirection(elapsedTimeInSecond);
    float travelledDistance = kart.speed * elapsedTimeInSecond + kart.currentAcceleration * (elapsedTimeInSecond * elapsedTimeInSecond) / 2.f;
    kart.speed = travelledDistance / elapsedTimeInSecond;
    kart.position += direction * travelledDistance;
    currentDistance = currentDistance + travelledDistance;
  }

  //Redefinition, TODO y'a ptete un truc mieux a faire
  virtual void bounce() {}

  virtual void moveBackward() {}
  virtual void moveForward() {}
  virtual void turnLeft(){}
  virtual void turnRight() {}
  virtual void stopMove() {}
  virtual void brake() {}
  virtual void drift() {}

private:
  ///La distance parcourue pendant un bounce
  float distanceToTravel;
  double currentDistance;
  bool bouncing;

};

class Boost: public KartState
{

public:
Boost(Kart& kart)
  : KartState(kart), time(200), boost(false) {}

  virtual void start()
  {
    assert(eventStack);
    //Si on est deja en train de booster on remet l'autre bosst a 0
    if (boost){
      time = 0;
      kart.currentAcceleration = kart.specifications.acceleration;
    }

    GameEventData data;//On s'en fout de la data ici
    GameEvent boostEvent(USE_BOOST_BEGIN, data);
    eventStack->push(boostEvent);

    boost = true;
    time = 200;
    kart.currentAcceleration = kart.specifications.acceleration + 25;
  }

  virtual void update(float elapsedTimeInSecond)
  {
    assert(eventStack);
    time -= 1;
    if (time <= 0)
    {
      kart.currentAcceleration = kart.specifications.acceleration;
      if(kart.speed > kart.specifications.maxSpeed){
        kart.speed = kart.specifications.maxSpeed;
      }
      else
      {
        GameEventData data;//On s'en fout de la data ici
        GameEvent boostEvent(USE_BOOST_END, data);
        eventStack->push(boostEvent);
        kart.setState(kart.forwardAccelerationState);

      }
      //Se remet a zero
      time = 0;
      boost = false;
      return;
    }

    glm::vec3 direction = setKartOrientationAndComputeDirection(elapsedTimeInSecond);
    float travelledDistance = kart.speed * elapsedTimeInSecond + kart.currentAcceleration * (elapsedTimeInSecond * elapsedTimeInSecond) / 2.f;
    kart.speed = travelledDistance / elapsedTimeInSecond;
    kart.position += direction * travelledDistance;
  }

  //Redefinition, TODO y'a ptete un truc mieux a faire
  virtual void bounce() {}
  virtual void moveBackward() {}
  virtual void moveForward() {}
  virtual void turnLeft(){
    if(kart.speed >= 15)
      kart.speed -= 0.25;
     kart.currentAngularSpeed = kart.specifications.angularSpeed;
  }
  virtual void turnRight() {
    if(kart.speed >= 15)
      kart.speed -= 0.25;
     kart.currentAngularSpeed = -kart.specifications.angularSpeed;
  }
  virtual void stopMove() {}
  virtual void brake() {}
  virtual void drift() {}
private:
  int time;
  bool boost;
};

class ForwardBrake : public ForwardDeceleration
{
public:
  ForwardBrake(Kart& kart)
    : ForwardDeceleration(kart) {}

  virtual void start()
  {
    kart.currentAcceleration = - kart.specifications.breakingCoefficient * kart.specifications.acceleration;
  }

  virtual void stopMove() {
    kart.currentAcceleration = kart.specifications.acceleration;
    kart.setState(kart.forwardDecelerationState);
  }
  virtual void brake() {}
  virtual void drift() {}
};

class BackwardBrake : public BackwardDeceleration
{
public:
  BackwardBrake(Kart& kart)
    : BackwardDeceleration(kart) {}

  virtual void start()
  {
    kart.currentAcceleration = kart.specifications.breakingCoefficient/2 * kart.specifications.acceleration;
  }

  virtual void stopMove() {
    kart.currentAcceleration = -kart.specifications.acceleration;
    kart.setState(kart.backwardDecelerationState);
  }
  virtual void brake() {}
  virtual void drift() {}
};

#endif // KARTSTATE_HPP
