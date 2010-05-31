
#include <kdtree.h>
#include <Point2.h>

#include <stdlib.h>

#include <iostream>


int main( int argc, char** argv )
{
  std::vector< Point2 > points;

  srand48( 0 );

  for ( unsigned int i=0; i<1000; ++i )
  {
    float p[ 2 ] = { drand48(), drand48() };
    points.push_back( Point2( p ) );
  }

  kd::BoundsFactory boundsFactory;
  kd::Measurer measurer;
  kd::TreeFactory treeFactory( measurer, boundsFactory );

  kd::Tree< Point2, 2 >* tree = treeFactory.create< Point2, 2 >( points );

  float min[2] = { -2.0f, -2.0f };
  float max[2] = { 2.0f, 2.0f };
  Point2 pmin( min );
  Point2 pmax( max );
  kd::Bounds< Point2, 2 > bounds( pmin, pmax );

  for ( unsigned int i=0; i<1000; ++i )
  {
    float p[ 2 ] = { drand48(), drand48() };
    Point2 point( p );

    kd::NeighbourData< Point2 > neighbourData = tree->nearestNeighbour( point, bounds );

    if ( ! neighbourData.found ) 
    {
      std::cerr << "Failed to find nearest point" << std::endl;
    }

    float distanceSq = measurer.distanceSq< Point2, 2 >( point, points[ 0 ] );
    Point2 nearest = points[ 0 ];
    for ( unsigned int i=1; i<1000; ++i )
    {
      float new_distanceSq = measurer.distanceSq< Point2, 2 >( point, points[ i ] );
      if ( new_distanceSq < distanceSq )
      {
        nearest = points[ i ];
        distanceSq = new_distanceSq;
      }
    }

    if ( nearest[ 0 ] != neighbourData.point[ 0 ] || nearest[ 1 ] != neighbourData.point[ 1 ] )
    {
      std::cerr << "Error - Found incorrect point for lookup." << std::endl;
    }
  }

  std::cerr << "Completed Testing" << std::endl;

  return 0;
}


