
#include <vector>

class Point2 
{
public:

  Point2( float _x, float _y ) : x( _x ), y( _y ) {}
  Point2() : x( 0.0f ), y( 0.0f ) {}

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

  Tree( Node* node )
   : m_node( node ) {}

private:

  Node* m_node;

};


class TreeFactory
{
public:

  TreeFactory() {};

  Tree* create( const std::vector< Point2 >& points );

private:

  Node* createSubTree( std::vector< Point2 >& points );

};


}


