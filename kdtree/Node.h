#ifndef NODE
#define NODE

#include "Measurer.h"
#include "Bounds.h"
#include "Data.h" 

namespace kd
{

template< typename P, unsigned int DIM >
class Node
{
public:
  Node() {};
  virtual ~Node() {};

  virtual NeighbourData< P > nearest(
      const P& point,
      NeighbourData< P > data,
      const Bounds< P, DIM >& bounds
      ) const = 0;

  virtual MultiNeighbourData< P > nearestNeighbours(
      const P& target,
      MultiNeighbourData< P > data,
      const Bounds< P, DIM >& bounds
      ) const = 0;
};



template< typename P, unsigned int DIM >
class SplitNode : public Node< P, DIM >
{
public:
  SplitNode(
      const P& pivot,
      unsigned int dim,
      Node< P, DIM >* left,
      Node< P, DIM >* right,
      const Measurer& measurer,
      const BoundsFactory& boundsFactory
      )
   : m_dim( dim ),
     m_left( left ),
     m_right( right ),
     m_pivot( pivot ),
     m_measurer( measurer ),
     m_boundsFactory( boundsFactory )
     {}

  ~SplitNode()
  {
    delete m_left;
    delete m_right;
  }

  NeighbourData< P > nearest( const P& target, NeighbourData< P > data, const Bounds< P, DIM >& bounds ) const
  {
    // Check if the node's pivot is our new nearest neighbour
    typename P::base_type distanceSq = m_measurer.distanceSq< P, DIM >( m_pivot, target );

    if ( distanceSq < data.distanceSq )
      data = NeighbourData< P >( true, m_pivot, distanceSq );

    // Split the bounds based on the node's pivot 
    BoundsPair< P, DIM > boundsPair = m_boundsFactory.split( bounds, m_pivot, m_dim );

    // Find out which bound half the target is in
    // and so decide our first bound and node to check
    bool inLeft = boundsPair.left.contains( target );
    const Bounds< P, DIM >& leftBounds( inLeft ? boundsPair.left : boundsPair.right );
    const Bounds< P, DIM >& rightBounds( inLeft ? boundsPair.right : boundsPair.left );

    const Node< P, DIM >* leftNode( inLeft ? m_left : m_right );
    const Node< P, DIM >* rightNode( inLeft ? m_right : m_left );

    // Find the nearest point in the left bound
    NeighbourData< P > leftData = leftNode->nearest( target, data, leftBounds );

    // Check if it worth looking in the right bounds
    // It is worth it if some of the right bounds line within
    // our current distanceSq radius
    const P nearestPointInBound = rightBounds.nearestPoint( target );

    distanceSq = m_measurer.distanceSq< P, DIM >( nearestPointInBound, target );

    if ( distanceSq < leftData.distanceSq )
    {
      // Worth checking right bounds
      NeighbourData< P > rightData = rightNode->nearest( target, leftData, rightBounds );

      if ( rightData.distanceSq < leftData.distanceSq )
      {
        // Point in right bounds is closest
        return rightData;
      }
    }

    // Closest point in left bounds is closest
    return leftData;
  }


  MultiNeighbourData< P > nearestNeighbours(
      const P& target,
      MultiNeighbourData< P > data,
      const Bounds< P, DIM >& bounds
      ) const
  {
    // Add the node's pivot if necessary
    typename P::base_type distanceSq = m_measurer.distanceSq< P, DIM >( m_pivot, target );
    data.update( m_pivot, distanceSq );

    // Split the bounds based on the node's pivot 
    BoundsPair< P, DIM > boundsPair = m_boundsFactory.split( bounds, m_pivot, m_dim );

    // Find out which bound half the target is in
    // and so decide our first bound and node to check
    bool inLeft = boundsPair.left.contains( target );
    const Bounds< P, DIM >& leftBounds( inLeft ? boundsPair.left : boundsPair.right );
    const Bounds< P, DIM >& rightBounds( inLeft ? boundsPair.right : boundsPair.left );

    const Node< P, DIM >* leftNode( inLeft ? m_left : m_right );
    const Node< P, DIM >* rightNode( inLeft ? m_right : m_left );

    // Find the nearest point in the left bound
    MultiNeighbourData< P > leftData = leftNode->nearestNeighbours( target, data, leftBounds );

    // Check if it worth looking in the right bounds
    // It is worth it if some of the right bounds line within
    // our current distanceSq radius
    const P nearestPointInBound = rightBounds.nearestPoint( target );

    distanceSq = m_measurer.distanceSq< P, DIM >( nearestPointInBound, target );

    if ( distanceSq < leftData.maxDistanceSq() || leftData.incomplete() )
    {
      // Worth checking right bounds
      MultiNeighbourData< P > rightData = rightNode->nearestNeighbours( target, leftData, rightBounds );
      return rightData;
    }

    // Closest point in left bounds is closest
    return leftData;
  }



private:

  const unsigned int m_dim;

  const Node< P, DIM >* m_left;
  const Node< P, DIM >* m_right;

  const P m_pivot;
  const Measurer m_measurer;
  const BoundsFactory m_boundsFactory;
};


template< typename P, unsigned int DIM >
class EmptyNode : public Node< P, DIM >
{
public:
  EmptyNode() {}

  NeighbourData< P > nearest( const P& target, NeighbourData< P > data, const Bounds< P, DIM >& bounds ) const
  {
    return data;
  }

  MultiNeighbourData< P > nearestNeighbours(
      const P& target,
      MultiNeighbourData< P > data,
      const Bounds< P, DIM >& bounds
      ) const
  {
    return data;
  }
};


template< typename P, unsigned int DIM >
class SingleNode : public Node< P, DIM >
{
public:

  SingleNode( const P& p, const Measurer& measurer )
   : m_point( p ), m_measurer( measurer ) {}

  NeighbourData< P > nearest( const P& target, NeighbourData< P > data, const Bounds< P, DIM >& bounds ) const
  {
    typename P::base_type distanceSq = m_measurer.distanceSq< P, DIM >( m_point, target );

    if ( distanceSq < data.distanceSq )
      data = NeighbourData< P >( true, m_point, distanceSq );

    return data;
  }

  MultiNeighbourData< P > nearestNeighbours(
      const P& target,
      MultiNeighbourData< P > data,
      const Bounds< P, DIM >& bounds 
      ) const
  {
    typename P::base_type distanceSq = m_measurer.distanceSq< P, DIM >( m_point, target );
    data.update( m_point, distanceSq );
    return data;
  }


private:

  const P m_point;
  const Measurer m_measurer;
};


}; // namespace kd

#endif // NODE
