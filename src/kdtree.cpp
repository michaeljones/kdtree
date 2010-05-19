#include "kdtree.h"

#include <algorithm>
#include <vector>

namespace kd
{


class BasicNode : public Node
{
public:

  BasicNode( const Bounds& bounds )
   : m_bounds( bounds ) {}

private:

  const Bounds m_bounds;

};

class SplitNode : public BasicNode
{
public:
  SplitNode( const Bounds& bounds, const Point2& point, Node* left, Node* right )
   : BasicNode( bounds ), m_left( left ), m_right( right ), m_point( point ) {}

private:

  const Node* m_left;
  const Node* m_right;

  const Point2 m_point;
};


class EmptyNode : public BasicNode
{
public:
  EmptyNode( const Bounds& bounds )
   : BasicNode( bounds ) {}

};

class SingleNode : public BasicNode
{
public:

  SingleNode( const Bounds& bounds, const Point2& p )
   : BasicNode( bounds ), m_point( p ) {}

private:

  const Point2 m_point;
};



bool cmp( const Point2 a, const Point2 b )
{
  return a.x > b.x;
}


typedef std::vector< Point2 > Subset;
typedef std::pair< Node*, Subset* > Item;

Node* TreeFactory::createSubTree( Subset& subset, const Bounds& bounds )
{
  if ( subset.size() == 0 )
  {
    return new EmptyNode( bounds );
  }

  if ( subset.size() == 1 )
  {
    return new SingleNode( bounds, subset[ 0 ] );
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

  BoundsPair boundsPair = m_boundsFactory.split( bounds, medianPoint, 0 );

  // Recursively create subtrees
  return new SplitNode(
      bounds,
      medianPoint,
      createSubTree( *left, boundsPair.first ),
      createSubTree( *right, boundsPair.second )
      );
}

Tree* TreeFactory::create( const Subset& points )
{
  Subset p( points );

  // Create the tree!
  return new Tree( createSubTree( p, m_boundsFactory.createBounds( points ) ) );
}


};

