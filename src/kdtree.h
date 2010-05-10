
#include <vector>

class Point2 
{
public:

  Point2( float _x, float _y ) : x( _x ), y( _y ) {}

public:

  float x, y;
};


namespace kd 
{

class Node
{
public:
  Node() {};
  virtual ~Node() {};

};

class Tree
{
public:

  Tree() {};

};


class TreeFactory
{
public:

  TreeFactory() {};

  Tree create( const std::vector< Point2 >& points );

};


}


