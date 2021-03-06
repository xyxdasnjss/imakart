#include "Map.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <dirent.h>
#ifndef WIN32
  #include <sys/types.h>
#endif

/**
 * @brief Map::Map
 * Attention truc degueu :
 * depuis Blender faut inverser y et z...
 */
Map::Map() :
  completed(false), position(0.f), orientation(glm::angleAxis(0.f, glm::vec3(0.f, 1.f, 0.f))), visible(true)
{
}

Map::~Map()
{
  for(unsigned int i = 0; i < itemsGenerator.size(); ++i){
    delete itemsGenerator[i];
  }
  itemsGenerator.erase(itemsGenerator.begin(),itemsGenerator.end());

  for (unsigned int i = 0; i < boundingBoxes.size(); ++i)
  {
    delete boundingBoxes[i];
  }
  boundingBoxes.erase(boundingBoxes.begin(), boundingBoxes.end());
}

void Map::loadFromFile(const std::string& filePath)
{
  std::ifstream mapStream(filePath);
  if (!mapStream.good())
  {
    throw std::runtime_error("Impossible de charger le fichier de description de circuit " + filePath);
  }

  //Jouons avec string
  name = filePath.substr(filePath.find_last_of('/') + 1, filePath.find_last_of('.') - filePath.find_last_of('/') - 1);

  std::string currentWord;
  while (mapStream >> currentWord)
  {
    if (currentWord == "PlayerCheckpoint")
    {
      loadCheckpoint(mapStream, true);
    }
    else if (currentWord == "OpponentCheckpoint")
    {
      loadCheckpoint(mapStream, false);
    }
    else if (currentWord == "StartingPoint")
    {
      loadStartingPoint(mapStream);
    }
    else if (currentWord == "Item")
    {
      loadItem(mapStream);
    }
    else if (currentWord == "BoundingBox")
    {
      loadBoundingBox(mapStream);
    }
    else if (currentWord == "FrictionArea")
    {
      loadFrictionArea(mapStream);
    }
  }

  mapStream.close();
}

const glm::vec3& Map::getPosition() const
  {return position;}

const glm::quat& Map::getOrientation() const
{return orientation;}

void Map::loadCheckpoint(std::ifstream& mapStream, bool playerCheckpoint)
{
  assert(mapStream);

  Checkpoint checkpoint;

  std::string attribute;

  mapStream >> attribute;
  if (attribute == "Start")
    checkpoint.start = true;
  else
    checkpoint.start = false;

  attribute.clear();

  //Pour l'instant a la bourrin, je sais que j'ai 2 attributs par checkpoint...
  for (int i = 0; i < 2; ++i)
  {
    mapStream >> attribute;
    if (attribute == "location")
    {
      mapStream >> checkpoint.position.x;
      mapStream >> checkpoint.position.y;
      mapStream >> checkpoint.position.z;
    }
    else if (attribute == "radius")
    {
      mapStream >> checkpoint.radius;
    }
  }
  checkpoint.checked = false;
  if (playerCheckpoint)
    playerCheckpoints.push_back(checkpoint);
  else
    opponentCheckpoints.push_back(checkpoint);
}

void Map::loadBoundingBox(std::ifstream& mapStream)
{
  assert(mapStream);

  std::string attribute;

  //Pour l'instant on zappe le name;
  mapStream >> attribute;
  attribute.clear();

  glm::vec3 position, size;
  glm::quat orientation;

  //Pour l'instant a la bourrin, je sais que j'ai 3 attributs par boundingbox...
  for (int i = 0; i < 3; ++i)
  {
    mapStream >> attribute;
    if (attribute == "location")
    {
      mapStream >> position.x;
      mapStream >> position.y;
      mapStream >> position.z;
    }
    else if (attribute == "size")
    {
      mapStream >> size.x;
      mapStream >> size.y;
      mapStream >> size.z;
    }
    else if (attribute == "rotation")
    {
      mapStream >> orientation.w;
      mapStream >> orientation.x;
      mapStream >> orientation.y;
      mapStream >> orientation.z;
    }
  }

  BoundingBox* bb = new BoundingBox(position, size);
  bb->setOrientation(orientation);
  boundingBoxes.push_back(bb);
}

void Map::loadItem(std::ifstream& mapStream)
{
  assert(mapStream);

  glm::vec3 itemPosition;

  std::string attribute;
  //Pour l'instant on zappe le name;
  mapStream >> attribute;
  attribute.clear();
  mapStream >> attribute; //je sais que c'est forcement "location"

  mapStream >> itemPosition.x;
  mapStream >> itemPosition.y;
  mapStream >> itemPosition.z;

  ItemGenerator* itemGenerator = new ItemGenerator(itemPosition);
  itemsGenerator.push_back(itemGenerator);
}

void Map::loadFrictionArea(std::ifstream& mapStream)
{
  assert(mapStream);
  //Commente tant que FrictionArea est pas défini

  FrictionArea area;
  std::string attribute;

  //Pour l'instant on zappe le name;
  mapStream >> attribute;
  attribute.clear();

  //Pour l'instant a la bourrin, je sais que j'ai 2 attributs par frictionArea...
  for (int i = 0; i < 2; ++i)
  {
    mapStream >> attribute;
    if (attribute == "location")
    {
      mapStream >> area.position.x;
      mapStream >> area.position.y;
      mapStream >> area.position.z;
    }
    else if (attribute == "size")
    {
      mapStream >> area.size.x;
      mapStream >> area.size.y;
      mapStream >> area.size.z;
    }
  }

  frictionAreas.push_back(area);
}

void Map::loadStartingPoint(std::ifstream& mapStream)
{
  assert(mapStream);

  glm::vec3 startingPointPosition;

  std::string attribute;
  //Pour l'instant on zappe le name;
  mapStream >> attribute;
  attribute.clear();
  mapStream >> attribute; //je sais que c'est forcement "location"

  mapStream >> startingPointPosition.x;
  mapStream >> startingPointPosition.y;
  mapStream >> startingPointPosition.z;

  kartStartingPoints.push_back(startingPointPosition);
}

