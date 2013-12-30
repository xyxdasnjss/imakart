#include "World3D.hpp"
#include "Object3D.hpp"
#include <stdexcept>
#include "Camera.hpp"


World3D::World3D()
    : camera(new Camera(1024,768)), sun(new DirectionalLight()) //Un peu degueu, a voir, c'est pour simplifier
{
  //Pour le dessin du monde 3D
  raceProgram.addShader(GL_VERTEX_SHADER, "shaders/Simple3DVS.glsl");
  raceProgram.addShader(GL_FRAGMENT_SHADER, "shaders/SimpleFS.glsl");

  //Pour la skybox
  skyboxProgram.addShader(GL_VERTEX_SHADER, "shaders/Skybox.vs.glsl");
  skyboxProgram.addShader(GL_FRAGMENT_SHADER, "shaders/Skybox.fs.glsl");
}

World3D::~World3D()
{
  //Utiliser un iterator declenche un bug hyper chelou (tente de detruire un deuxieme
  //pointeur vers la classe mere...)
  for (size_t i = 0; i < objects3D.size(); ++i)
  {
    delete objects3D[i];
  }

  objects3D.erase(objects3D.begin(), objects3D.end());
  delete camera;
}

void World3D::init()
{
  std::string logInfo;
  if (!raceProgram.compileAndLinkShaders(logInfo))
  {
    throw std::runtime_error(logInfo);
  }
  logInfo.clear();

  if (!skyboxProgram.compileAndLinkShaders(logInfo))
  {
    throw std::runtime_error(logInfo);
  }

  if(!skybox.init("textures/new_skybox","_deserted_back.tga","_deserted_front.tga","_deserted_up.tga","_deserted_bottom.tga","_deserted_right.tga","_deserted_left.tga"))
  {
    throw std::runtime_error("Impossible d'initialiser la SkyBox");
  }
  skybox.setCamera(camera);
}

void World3D::draw() const
{
  //Mise a jour matrice ViewProjection
  //Attention : le vertex shader doit contenir les bonnes uniforms
  camera->updateViewProjectionMatrix();
  skyboxProgram.use();
  skybox.render(skyboxProgram);

  raceProgram.use();
  const glm::mat4& viewMatrix = camera->getViewMatrix();
  const glm::mat4& viewProjection = camera->getViewProjectionMatrix();
  GLint viewId = raceProgram.getUniformIndex("uView");
  GLint viewProjectionId = raceProgram.getUniformIndex("viewProjection");
  raceProgram.setUniform(viewId, viewMatrix);
  raceProgram.setUniform(viewProjectionId, viewProjection);

  //Ranger aussi la gestion des lumieres
/*  for (auto oneLight = lights.begin(); oneLight != lights.end(); ++oneLight)
  {
    (*oneLight)->updateLightDirection();
    (*oneLight)->updateLight(viewMatrix);
    const glm::vec3& direction = (*oneLight)->getLighDirection();
    const glm::vec3& position = (*oneLight)->getLightPosition();
    const glm::vec3& intensity = (*oneLight)->getLightIntensity();
    GLint lightDirId = raceProgram.getUniformIndex("uLightDir");
    GLint lightPosId = raceProgram.getUniformIndex("uLightPos");
    GLint lightIntensityId = raceProgram.getUniformIndex("uLi");
    raceProgram.setUniform(lightDirId,1,direction);
    raceProgram.setUniform(lightPosId,1, position);
    raceProgram.setUniform(lightIntensityId, intensity);
  }*/
  sun->updateLight(viewMatrix);
  const glm::vec3& direction = sun->getLightDirection();
  const glm::vec3& intensity = sun->getLightIntensity();
  GLint lightDirId = raceProgram.getUniformIndex("uLightDir");
  GLint lightIntensityId = raceProgram.getUniformIndex("uLi");
  raceProgram.setUniform(lightDirId,1,direction);
  raceProgram.setUniform(lightIntensityId,1,intensity);

  for (auto object3D = objects3D.begin(); object3D != objects3D.end(); ++object3D)
  {
    (*object3D)->setViewMatrix(camera->getViewMatrix());
    (*object3D)->update();
    (*object3D)->draw(raceProgram);
  }
}

void World3D::setCamera(Camera* newCamera)
{
  delete camera;
  camera = newCamera;
  skybox.setCamera(camera);
}
