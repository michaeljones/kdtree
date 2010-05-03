
#include <vector>

class Point3 
{
public:

  Point3( float _x, float _y, float _z ) : x( _x ), y( _y ), z( _z ) {}

public:

  float x, y, z;
};


namespace kd 
{

class Tree
{
public:

  Tree() {};

};


class TreeFactory
{
public:
  TreeFactory() {};

  Tree create( std::vector< Point3 >& points );

};


}

