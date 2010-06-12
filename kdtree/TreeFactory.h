#ifndef KDTREEFACTORY
#define KDTREEFACTORY

#include "Tree.h"

#include <vector>
#include <algorithm>


namespace kd {

class TreeFactory
{
public:

  TreeFactory( const Measurer& measurer, const BoundsFactory& boundsFactory )
  : m_measurer( measurer ), m_boundsFactory( boundsFactory ) {};

  template< typename P, unsigned int DIM >
  Tree< P, DIM >* create( const std::vector< P >& points );

private:

  template< typename P, unsigned int DIM >
  Node< P, DIM >* createSubTree( std::vector< P >* points, Bounds< P, DIM >& bounds );

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
Node< P, DIM >* TreeFactory::createSubTree( std::vector< P >* subset, Bounds< P, DIM >& bounds )
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

  unsigned int dim = bounds.longestDimension();
  PointCompare< P > cmp( dim );
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

  BoundsPair< P, DIM > boundsPair = m_boundsFactory.split( bounds, medianPoint, dim );

  return new SplitNode< P, DIM >(
      medianPoint,
      dim,
      createSubTree< P, DIM >( left, boundsPair.left ),
      createSubTree< P, DIM >( right, boundsPair.right ),
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
  Bounds< P, DIM > bounds = m_boundsFactory.createBounds< P, DIM >( *p );

  // Create the tree!
  return new Tree< P, DIM >( createSubTree< P, DIM >( p, bounds ), m_measurer, m_boundsFactory );
}


}; // namespace kd

#endif // KDTREEFACTORY

