
#include <kdtree.h>

#include <stdlib.h>

#include <iostream>


int main( int argc, char** argv )
{
  std::vector< Point2 > points;

  srand48( 0 );

  for ( unsigned int i=0; i<10; ++i )
  {
    double x = drand48();
    double y = drand48();
    std::cerr << "Adding " << x << " " << y << std::endl;
    points.push_back( Point2( x, y ) );
  }

  kd::BoundsFactory boundsFactory;
  kd::TreeFactory treeFactory( boundsFactory );

  kd::Tree* tree = treeFactory.create( points );

  for ( unsigned int i=0; i<10; ++i )
  {
    double x = drand48();
    double y = drand48();
    std::cerr << "Looking up " << x << " " << y << std::endl;
    Point2 point( x, y );

    kd::Bounds bounds( Point2( -2.0, -2.0 ), Point2( 2.0, 2.0 ) );
    kd::NeighbourData neighbourData = tree->nearestNeighbour( point, bounds );

    if ( neighbourData.found ) 
    {
      std::cerr << "Found point at " << neighbourData.point.x
        << " " << neighbourData.point.y
        << " ( distanceSq: " << neighbourData.distanceSq << " ) "  << std::endl;
    }
    else
    {
      std::cerr << "Failed to find nearest point" << std::endl;
    }

    
    Point2 sep = point - points[ 0 ];
    float distance = sep.x*sep.x + sep.y*sep.y;
    Point2 nearest = points[ 0 ];
    for ( unsigned int i=1; i<10; ++i )
    {
      sep = point - points[ i ];
      float new_distance = sep.x*sep.x + sep.y*sep.y;
      if ( new_distance < distance )
      {
        nearest = points[ i ];
        distance = new_distance;
      }
    }

    std::cerr << "Nearest is at  " << nearest.x << " " << nearest.y 
        << " ( distanceSq: " << distance << " ) "  << std::endl;
    
  }

  std::cerr << "Completed Testing" << std::endl;

  return 0;
}


