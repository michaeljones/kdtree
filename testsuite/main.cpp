
#include <kdtree/TreeFactory.h>
#include "Point.h"

#include <stdlib.h>
#include <memory>

#include <iostream>


struct PointDistance
{
  PointDistance( const Point2& p, float d )
    : distSq( d ), point( p ) {}
  
  float distSq;
  Point2 point;
};

typedef std::list< PointDistance > PointDistanceList;


#define POINT_COUNT 1000


int main( int argc, char** argv )
{
  std::vector< Point2 > points;

  srand48( 0 );

  for ( unsigned int i=0; i<POINT_COUNT; ++i )
  {
    float p[ 2 ] = { drand48(), drand48() };
    points.push_back( Point2( p ) );
  }

  kd::BoundsFactory boundsFactory;
  kd::Measurer measurer;
  kd::TreeFactory treeFactory( measurer, boundsFactory );

  std::auto_ptr< kd::Tree< Point2, 2 > > tree( treeFactory.create< Point2, 2 >( points ) );

  float min[2] = { -2.0f, -2.0f };
  float max[2] = { 2.0f, 2.0f };
  Point2 pmin( min );
  Point2 pmax( max );
  kd::Bounds< Point2, 2 > bounds( pmin, pmax );

  for ( unsigned int i=0; i<POINT_COUNT; ++i )
  {
    float p[ 2 ] = { drand48(), drand48() };
    Point2 point( p );

    // Find nearest neighbour
    kd::NeighbourData< Point2 > neighbourData = tree->nearestNeighbour( point, bounds );

    if ( ! neighbourData.found ) 
    {
      std::cerr << "Failed to find nearest point" << std::endl;
    }

    // Check result with a brute force search
    float distanceSq = measurer.distanceSq< Point2, 2 >( point, points[ 0 ] );
    Point2 nearest = points[ 0 ];
    for ( unsigned int j=1; j<POINT_COUNT; ++j )
    {
      float new_distanceSq = measurer.distanceSq< Point2, 2 >( point, points[ j ] );
      if ( new_distanceSq < distanceSq )
      {
        nearest = points[ j ];
        distanceSq = new_distanceSq;
      }
    }

    if ( nearest[ 0 ] != neighbourData.point[ 0 ] || nearest[ 1 ] != neighbourData.point[ 1 ] )
    {
      std::cerr << "Error - Found incorrect point for lookup." << std::endl;
    }

    // Find five nearest neighbours
    kd::MultiNeighbourData< Point2 > neighboursData = tree->nearestNeighbours( 5, point, bounds );

    if ( neighboursData.points().size() != 5 )
    {
      std::cerr << i << ") Failed to find 5 nearest neighbours" 
        " (count: " << neighboursData.points().size() << ")" << std::endl;
    }

    // Check actual values with a brute force search
    float nthDistanceSq = 200.0f;
    PointDistanceList pointList;
    for ( unsigned int j=0; j<POINT_COUNT; ++j )
    {
      float distSq = measurer.distanceSq< Point2, 2 >( point, points[ j ] );

      if ( distSq < nthDistanceSq )
      {
        PointDistanceList::iterator it = pointList.begin();
        PointDistanceList::iterator end = pointList.end();

        for( ; it != end; ++it )
        {
          if ( it->distSq > distSq )
            break;
        }

        // When it is nearest than one of our points insert it
        pointList.insert( it, PointDistance( points[ j ], distSq ) );

        // Crop our list at the desired size
        if ( pointList.size() > 5 )
        {
          pointList.pop_back();

          // Update our nthDistanceSq
          nthDistanceSq = pointList.back().distSq;
        }
      }
    }

    PointDistanceList::const_iterator bruteforce_it = pointList.begin();
    PointDistanceList::const_iterator bruteforce_end = pointList.end();

    kd::MultiNeighbourData< Point2 >::PointDistanceList::const_iterator kd_it = neighboursData.points().begin();
    kd::MultiNeighbourData< Point2 >::PointDistanceList::const_iterator kd_end = neighboursData.points().end();

    int index = 0;

    while ( kd_it != kd_end )
    {
      if ( kd_it->point[ 0 ] != bruteforce_it->point[ 0 ] || kd_it->point[ 1 ] != bruteforce_it->point[ 1 ] )
      {
        std::cerr << "Error - Found incorrect point set for lookup ( " << i << ":" << index << " ) " << std::endl;
        std::cerr << "x    " << kd_it->point[ 0 ] << " " <<  bruteforce_it->point[ 0 ] << std::endl;
        std::cerr << "y    " << kd_it->point[ 1 ] << " " <<  bruteforce_it->point[ 1 ] << std::endl;
        std::cerr << "dist " << kd_it->distSq << " " <<  bruteforce_it->distSq << std::endl;
      }

      ++kd_it;
      ++bruteforce_it;
      ++index;
    }
  }

  std::cerr << "Completed Testing" << std::endl;

  return 0;
}


