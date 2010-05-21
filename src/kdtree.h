#ifndef KDTREE
#define KDTREE

#include "Point2.h"
#include "bounds.h"

#include <vector>

namespace kd 
{

struct NeighbourData
{
  NeighbourData( bool f, Point2 p, float d )
    : found( f ), point( p ), distanceSq( d ) {}

  bool found;
  Point2 point;
  float distanceSq;
};

class Node;

class Tree
{
public:

  Tree( Node* node, const BoundsFactory& boundsFactory )
   : m_node( node ), m_boundsFactory( boundsFactory ) {}

  NeighbourData nearestNeighbour( Point2 point, Bounds bounds ) const;

private:

  Node* m_node;
  const BoundsFactory m_boundsFactory;

};


class TreeFactory
{
public:

  TreeFactory( const BoundsFactory& boundsFactory )
  : m_boundsFactory( boundsFactory ) {};

  Tree* create( const std::vector< Point2 >& points );

private:

  Node* createSubTree( std::vector< Point2 >& points );

private:

  const BoundsFactory m_boundsFactory;

};


};

#endif // KDTREE

