#ifndef BOUNDS
#define BOUNDS

#include "Point2.h"

#include <vector>
#include <limits>

namespace kd 
{

class Bounds
{
public:
  Bounds( const Point2& min, const Point2& max )
  : m_min( min ), m_max( max ) {}

  const Point2& max() const { return m_max; } 
  const Point2& min() const { return m_min; } 

private:

  const Point2 m_min;
  const Point2 m_max;
};


typedef std::pair< Bounds, Bounds > BoundsPair;

class BoundsFactory
{
public:
  BoundsFactory() {};

  Bounds createBounds( const std::vector< Point2 >& points ) const;

  BoundsPair split( const Bounds& bounts, const Point2& point, const int dimension ) const;

};


Bounds BoundsFactory::createBounds( const std::vector< Point2 >& points ) const
{
  std::vector< Point2 >::const_iterator it = points.begin();
  std::vector< Point2 >::const_iterator end = points.end();

  Point2 min( std::numeric_limits< float >::max(), std::numeric_limits< float >::max() );
  Point2 max( - std::numeric_limits< float >::max(), - std::numeric_limits< float >::max() );

  for ( ; it != end; ++it )
  {
    max = Point2( it->x > max.x ? it->x : max.x, it->y > max.y ? it->y : max.y );
    min = Point2( it->x < min.x ? it->x : min.x, it->y < min.y ? it->y : min.y );
  }

  return Bounds( min, max );
}

BoundsPair BoundsFactory::split(
    const Bounds& bounds,
    const Point2& point,
    const int dimension
    ) const
{
  float maxcoords[2] = { 0.0, 0.0 };
  float mincoords[2] = { 0.0, 0.0 };

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

  Bounds lower( bounds.min(), Point2( maxcoords ) );
  Bounds upper( Point2( mincoords ), bounds.max() );

  return BoundsPair( lower, upper );
}

};


#endif // BOUNDS

