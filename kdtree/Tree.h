#ifndef KDTREE
#define KDTREE

#include "Bounds.h"
#include "Node.h"
#include "Measurer.h"

#include <vector>

namespace kd 
{

//! \brief Tree which provides search interface
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

  //! Find nearest neighbour to given point
  NeighbourData< P > nearestNeighbour( const P& target, const Bounds< P, DIM >& bounds ) const;

  //! Find "num" nearest neighbours to the given point
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
  NeighbourData< P > data( maxDistanceSq );

  m_node->search( target, data, bounds );

  return data;
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

  m_node->search( target, data, bounds );

  return data;
}



};

#endif // KDTREE

