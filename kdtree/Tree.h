#ifndef KDTREE
#define KDTREE

#include "Bounds.h"
#include "Node.h"
#include "Measurer.h"

#include <vector>
#include <algorithm>

namespace kd 
{

template< typename P, unsigned int DIM >
class Tree
{
public:

  Tree( Node< P, DIM >* node, const Measurer& measurer, const BoundsFactory& boundsFactory )
   : m_node( node ), m_measurer( measurer ), m_boundsFactory( boundsFactory ) {}

  ~Tree()
  {
    delete m_node;
  }

  NeighbourData< P > nearestNeighbour( const P& target, const Bounds< P, DIM >& bounds ) const;

  MultiNeighbourData< P > nearestNeighbours(
      unsigned int num,
      const P& target,
      const Bounds< P, DIM >& bounds
      ) const;

private:

  Node< P, DIM >* m_node;
  const Measurer m_measurer;
  const BoundsFactory m_boundsFactory;

};

template< typename P, unsigned int DIM >
NeighbourData< P > Tree< P, DIM >::nearestNeighbour(
    const P& target,
    const Bounds< P, DIM >& bounds
    ) const
{
  P farthest = bounds.farthestPoint( target );
  typename P::base_type maxDistanceSq = m_measurer.distanceSq< P, DIM >( farthest, target );
  NeighbourData< P > data( false, farthest, maxDistanceSq );

  return m_node->nearest( target, data, bounds );
}


template< typename P, unsigned int DIM >
MultiNeighbourData< P > Tree< P, DIM >::nearestNeighbours(
    unsigned int num,
    const P& target,
    const Bounds< P, DIM >& bounds
    ) const
{
  P farthest = bounds.farthestPoint( target );
  typename P::base_type maxDistanceSq = m_measurer.distanceSq< P, DIM >( farthest, target );
  MultiNeighbourData< P > data( num, maxDistanceSq );

  return m_node->nearestNeighbours( target, data, bounds );
}


class TreeFactory
{
public:

  TreeFactory( const Measurer& measurer, const BoundsFactory& boundsFactory )
  : m_measurer( measurer ), m_boundsFactory( boundsFactory ) {};

  template< typename P, unsigned int DIM >
  Tree< P, DIM >* create( const std::vector< P >& points );

private:

  template< typename P, unsigned int DIM >
  Node< P, DIM >* createSubTree( std::vector< P >* points );

private:

  const Measurer m_measurer;
  const BoundsFactory m_boundsFactory;
};


template< typename P >
struct PointCompare
{
  PointCompare( const unsigned int dimension )
    : dim( dimension ) {}

  bool operator()( const P& a, const P& b ) const
  {
    return a[ dim ] < b[ dim ];
  }

  unsigned int dim;
};



template< typename P, unsigned int DIM >
Node< P, DIM >* TreeFactory::createSubTree( std::vector< P >* subset )
{
  if ( subset->size() == 0 )
  {
    // Clean up subset
    delete subset;
    return new EmptyNode< P, DIM >();
  }

  if ( subset->size() == 1 )
  {
    // Clean up subset
    P point = (*subset)[ 0 ];
    delete subset;
    return new SingleNode< P, DIM >( point, m_measurer );
  }

  PointCompare< P > cmp( 0 );
  std::sort( subset->begin(), subset->end(), cmp );

  typename std::vector< P >::iterator it = subset->begin();
  typename std::vector< P >::iterator end = subset->end();

  int medianIdx = int( subset->size() / 2.0f );

  P medianPoint = (*subset)[ medianIdx ];

  // TODO: Compress creation and copy into creation from iterators
  std::vector< P >* left = new std::vector< P >( medianIdx );
  std::vector< P >* right = new std::vector< P >( subset->size() - ( medianIdx + 1 ) );

  // TODO: Check for overrunning indicies
  std::copy( subset->begin(), subset->begin() + medianIdx, left->begin() );
  std::copy( subset->begin() + medianIdx + 1, subset->end(), right->begin() );

  // Clean up previous subset
  delete subset;

  return new SplitNode< P, DIM >(
      medianPoint,
      createSubTree< P, DIM >( left ),
      createSubTree< P, DIM >( right ),
      m_measurer,
      m_boundsFactory
      );
}

template< typename P, unsigned int DIM >
Tree< P, DIM >* TreeFactory::create( const std::vector< P >& points )
{
  // Dynamically allocate it so we can clean up the memory 
  // faster than it might otherwise be cleaned
  std::vector< P >* p = new std::vector< P >( points );

  // Create the tree!
  return new Tree< P, DIM >( createSubTree< P, DIM >( p ), m_measurer, m_boundsFactory );
}



};

#endif // KDTREE

