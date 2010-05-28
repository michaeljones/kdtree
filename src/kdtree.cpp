#include "kdtree.h"

#include <algorithm>
#include <vector>
#include <iostream>

namespace kd
{

class Node
{
public:
  Node() {};
  virtual ~Node() {};

  virtual NeighbourData nearest( const Point2& point, NeighbourData data, const Bounds& bounds ) const = 0;

};



class SplitNode : public Node
{
public:
  SplitNode( const Point2& pivot, Node* left, Node* right, const BoundsFactory& boundsFactory )
   : m_left( left ),
     m_right( right ),
     m_pivot( pivot ),
     m_boundsFactory( boundsFactory )
     {}

  NeighbourData nearest( const Point2& target, NeighbourData data, const Bounds& bounds ) const
  {
    // Check if the node's pivot is our new nearest neighbour
    Point2 sep = m_pivot - target;
    float distanceSq = sep.lengthSquared();

    if ( distanceSq < data.distanceSq )
      data = NeighbourData( true, m_pivot, distanceSq );

    // Split the bounds based on the node's pivot 
    BoundsPair boundsPair = m_boundsFactory.split( bounds, m_pivot, 0 );

    // Find out which bound half the target is in
    // and so decide our first bound and node to check
    bool inFirst = boundsPair.first.contains( target );
    const Bounds& firstBounds( inFirst ? boundsPair.first : boundsPair.second );
    const Bounds& secondBounds( inFirst ? boundsPair.second : boundsPair.first );

    const Node* firstNode( inFirst ? m_left : m_right );
    const Node* secondNode( inFirst ? m_right : m_left );

    // Find the nearest point in the first bound
    NeighbourData firstData = firstNode->nearest( target, data, firstBounds );

    // Check if it worth looking in the second bounds
    // It is worth it if some of the second bounds line within
    // our current distanceSq radius
    const Point2 nearestPointInBound = secondBounds.nearestPoint( target );

    sep = nearestPointInBound - target;
    distanceSq = sep.lengthSquared();

    if ( distanceSq < firstData.distanceSq )
    {
      // Worth checking second bounds
      NeighbourData secondData = secondNode->nearest( target, firstData, secondBounds );

      if ( secondData.distanceSq < firstData.distanceSq )
      {
        // Point in second bounds is closest
        return secondData;
      }
    }

    // Closest point in first bounds is closest
    return firstData;
  }

private:

  const Node* m_left;
  const Node* m_right;

  const Point2 m_pivot;
  const BoundsFactory m_boundsFactory;
};


class EmptyNode : public Node
{
public:
  EmptyNode() {}

  NeighbourData nearest( const Point2& target, NeighbourData data, const Bounds& bounds ) const
  {
    return data;
  }

};

class SingleNode : public Node
{
public:

  SingleNode( const Point2& p )
   : m_point( p ) {}

  NeighbourData nearest( const Point2& target, NeighbourData data, const Bounds& bounds ) const
  {
    Point2 sep = m_point - target;
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
  return a.x < b.x;
}

NeighbourData Tree::nearestNeighbour( const Point2& target, const Bounds& bounds ) const
{
  Point2 furthest = bounds.furthestPoint( target );
  Point2 sep = furthest - target;
  float maxDistanceSq = sep.x*sep.x + sep.y*sep.y;
  NeighbourData data( false, furthest, maxDistanceSq );

  return m_node->nearest( target, data, bounds );
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

  Subset::iterator it = subset.begin();
  Subset::iterator end = subset.end();

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

