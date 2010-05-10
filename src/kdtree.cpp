
#include "kdtree.h"

#include <algorithm>

namespace kd
{


class SplitNode : public Node
{
public:
  SplitNode( Point2 point, Node* left, Node* right )
   : m_left( left ), m_right( right ), m_point( point ) {}

private:

  const Node* m_left;
  const Node* m_right;

  const Point2 m_point;
};


class EmptyNode : public Node
{
public:
  EmptyNode() {}

};

class SingleNode : public Node
{
public:

  SingleNode( const Point2& p ) : m_point( p ) {}

private:

  const Point2 m_point;
};


typedef std::vector< Point2 > Subset;
typedef std::pair< Node*, const Subset* > Item;


bool cmp( const Point2 a, const Point2 b )
{
  return a.x > b.x;
}


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
      createSubTree( *right )
      );
}

Tree* TreeFactory::create( const Subset& points )
{
  Subset p( points );

  // Create the tree!
  return new Tree( createSubTree( p ) );
}


}

