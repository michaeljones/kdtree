#ifndef BOUNDS
#define BOUNDS

#include <vector>
#include <limits>

namespace kd 
{

template< typename P, unsigned int DIM >
class Bounds
{
public:
  Bounds( const P& min, const P& max )
  : m_min( min ), m_max( max ) {}

  const P& max() const { return m_max; } 
  const P& min() const { return m_min; } 

  bool contains( const P& point ) const
  {
    for ( unsigned int i=0; i<DIM; ++i )
    {
      if ( m_min[ i ] > point[ i ] ) return false;
      if ( m_max[ i ] < point[ i ] ) return false;
    }

    return true;
  }

  P farthestPoint( P point ) const
  {
    for ( unsigned int i=0; i<DIM; ++i )
    {
        point[ i ] = ( ( point[ i ] - m_min[ i ] ) > ( point[ i ] - m_max[ i ] ) ) ? m_min[ i ] : m_max[ i ];
    }

    return point;
  }

  P nearestPoint( P point ) const 
  {
    for ( unsigned int i=0; i<DIM; ++i )
    {
      if ( point[ i ] < m_min[ i ] ) point[ i ] = m_min[ i ];
      if ( point[ i ] > m_max[ i ] ) point[ i ] = m_max[ i ];
    }

    return point;
  }

  unsigned int longestDimension() const 
  {
    unsigned int longestDim = 0;
    typename P::base_type length = m_max[ 0 ] - m_min[ 0 ];

    for ( unsigned int i=1; i<DIM; ++i )
    {
      if ( length < m_max[ i ] - m_min[ i ] )
      {
        longestDim = i;
      }
    }

    return longestDim;
  }

private:

  const P m_min;
  const P m_max;
};



template< typename P, unsigned int DIM >
class BoundsPair
{
public:

  BoundsPair( Bounds< P, DIM > l, Bounds< P, DIM > r )
   : left( l ), right( r ) {}

  Bounds< P, DIM > left;
  Bounds< P, DIM > right;

};

class BoundsFactory
{
public:
  BoundsFactory() {};

  template< typename P, unsigned int DIM >
  Bounds< P, DIM > createBounds( const std::vector< P >& points ) const;

  template< typename P, unsigned int DIM >
  BoundsPair< P, DIM > split( const Bounds< P, DIM >& bounds, const P& point, const int dimension ) const;

};


template< typename P, unsigned int DIM >
Bounds< P, DIM > BoundsFactory::createBounds( const std::vector< P >& points ) const
{
  typename P::base_type min[ DIM ];
  typename P::base_type max[ DIM ];

  for ( unsigned int i=0; i<DIM; ++i )
  {
    min[ i ] = std::numeric_limits< typename P::base_type >::max();
    max[ i ] = - std::numeric_limits< typename P::base_type >::max();
  }

  typename std::vector< P >::const_iterator it = points.begin();
  typename std::vector< P >::const_iterator end = points.end();

  for ( ; it != end; ++it )
  {
    for ( unsigned int i=0; i<DIM; ++i )
    {
      min[ i ] = (*it)[ i ] < min[ i ] ? (*it)[ i ] : min[ i ];
      max[ i ] = (*it)[ i ] > max[ i ] ? (*it)[ i ] : max[ i ];
    }
  }

  return Bounds< P, DIM >( P( min ), P( max ) );
}

template< typename P, unsigned int DIM >
BoundsPair< P, DIM > BoundsFactory::split(
    const Bounds< P, DIM >& bounds,
    const P& point,
    const int dimension
    ) const
{
  typename P::base_type maxcoords[ DIM ];
  typename P::base_type mincoords[ DIM ];

  for ( unsigned int i=0; i<2; ++i )
  {
    if ( i == dimension )
    {
      maxcoords[ i ] = point[ i ];
      mincoords[ i ] = point[ i ];
    }
    else
    {
      maxcoords[ i ] = bounds.max()[ i ];
      mincoords[ i ] = bounds.min()[ i ];
    }
  }

  Bounds< P, DIM > lower( bounds.min(), P( maxcoords ) );
  Bounds< P, DIM > upper( P( mincoords ), bounds.max() );

  return BoundsPair< P, DIM >( lower, upper );
}

};


#endif // BOUNDS

