#ifndef MAP3D_H
#define MAP3D_H

#include "Object3D.hpp"
#include "Vertex3D.hpp"
#include "GL/glew.h"
#include <vector>
#include "VBO.hpp"
#include "VAO.hpp"

class Map3D : public Object3D
{
public:
  Map3D();

  /**
   * @see Object3D
   */
  virtual void draw(const glimac::ShaderProgram& shaderProgram) const;


  /**
   * @see Object3D
   */
  virtual void update();

private:
  const GLsizeiptr verticesSize() const
    {return 8 * sizeof(glimac::Vertex3D);}

  const GLsizei indicesSize() const
    {return GLsizei(14);}

  glimac::Vertex3D vertices[8];
  GLushort indices[14];

  glimac::LowLevelVBO vbo;
  glimac::VAO vao;

  glm::mat4 modelMatrix;
  glm::vec4 color;
};

#endif // MAP3D_H
