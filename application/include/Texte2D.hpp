#ifndef Texte2D_HPP
#define Texte2D_HPP

#include <vector>
#include <string>
#include "VBO.hpp"
#include "VAO.hpp"
#include "Texture.hpp"
#include "Vertex2DRGB.hpp"

class Texte2D
{
public:
  Texte2D(std::string text, int x, int y, int size);
  ~Texte2D();

private:
  std::vector<glimac::Vertex2DUV> vertices;
  glimac::LowLevelVBO vbo;
  glimac::VAO vao;
  glimac::Texture* texture;

  void setVBO(glimac::Vertex2DUV vertices[]); //called in the constructor
  void setVAO(); //called in the constructor
};

#endif // Texte2D_HPP