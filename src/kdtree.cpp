#include "kdtree.h"

#include <algorithm>
#include <vector>

namespace kd
{

class Node
{
public:
  Node() {};
  virtual ~Node() {};

  virtual NeighbourData nearest( Point2 point, NeighbourData data, Bounds bounds ) const = 0;

};



class SplitNode : public Node
{
public:
  SplitNode( const Point2& point, Node* left, Node* right, const BoundsFactory& boundsFactory )
   : m_left( left ),
     m_right( right ),
     m_point( point ),
     m_boundsFactory( boundsFactory )
     {}

  NeighbourData nearest( Point2 point, NeighbourData data, Bounds bounds ) const
  {
    BoundsPair boundsPair = m_boundsFactory.split( bounds, m_point, 0 );

    Point2 sep = m_point - point;
    float distanceSq = sep.lengthSquared();

    if ( distanceSq < data.distanceSq )
      data = NeighbourData( true, m_point, distanceSq );

    if ( boundsPair.first.contains( point ) )
      return m_left->nearest( point, data, boundsPair.first );
    else
      return m_right->nearest( point, data, boundsPair.second );
  }

private:

  const Node* m_left;
  const Node* m_right;

  const Point2 m_point;
  const BoundsFactory m_boundsFactory;
};


class EmptyNode : public Node
{
public:
  EmptyNode() {}

  NeighbourData nearest( Point2 point, NeighbourData data, Bounds bounds ) const
  {
    return data;
  }


};

class SingleNode : public Node
{
public:

  SingleNode( const Point2& p )
   : m_point( p ) {}

  NeighbourData nearest( Point2 point, NeighbourData data, Bounds bounds ) const
  {
    Point2 sep = m_point - point;
    float distanceSq = sep.lengthSquared();

    if ( distanceSq < data.distanceSq )
      data = NeighbourData( true, m_point, distanceSq );

    return data;
  }



private:

  const Point2 m_point;
};



bool cmp( const Point2 a, const Point2 b )
{
  return a.x > b.x;
}

NeighbourData Tree::nearestNeighbour( Point2 point, Bounds bounds ) const
{
  Point2 furthest = bounds.furthestPoint( point );
  float maxDistanceSq = furthest.x*furthest.x + furthest.y*furthest.y;
  NeighbourData data( false, Point2( 0.0, 0.0 ), maxDistanceSq );

  return m_node->nearest( point, data, bounds );
}


typedef std::vector< Point2 > Subset;
typedef std::pair< Node*, Subset* > Item;

Node* TreeFactory::createSubTree( Subset& subset )
{
  if ( subset.size() == 0 )
  {
    return new EmptyNode();
  }

  if ( subset.size() == 1 )
  {
    return new SingleNode( subset[ 0 ] );
  }

  std::sort( subset.begin(), subset.end(), cmp );

  int medianIdx = int( subset.size() / 2.0f );

  Point2 medianPoint = subset[ medianIdx ];

  // TODO: Compress creation and copy into creation from iterators
  Subset* left = new Subset( medianIdx );
  Subset* right = new Subset( subset.size() - ( medianIdx + 1 ) );

  // TODO: Check for overrunning indicies
  std::copy( subset.begin(), subset.begin() + medianIdx, left->begin() );
  std::copy( subset.begin() + medianIdx + 1, subset.end(), right->begin() );

  // Recursively create subtrees
  return new SplitNode(
      medianPoint,
      createSubTree( *left ),
      createSubTree( *right ),
      m_boundsFactory
      );
}

Tree* TreeFactory::create( const Subset& points )
{
  Subset p( points );

  // Create the tree!
  return new Tree( createSubTree( p ), m_boundsFactory );
}


};

