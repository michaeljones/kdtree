#ifndef KDTREE
#define KDTREE

#include "Point2.h"
#include "bounds.h"

#include <vector>

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

  TreeFactory( const BoundsFactory& boundsFactory )
  : m_boundsFactory( boundsFactory ) {};

  Tree* create( const std::vector< Point2 >& points );

private:

  Node* createSubTree( std::vector< Point2 >& points, const Bounds& bounds );

private:

  const BoundsFactory m_boundsFactory;

};


};

#endif // KDTREE

