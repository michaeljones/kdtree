
#include <kdtree.h>

#include <stdlib.h>

#include <iostream>


int main( int argc, char** argv )
{
  std::vector< Point2 > points;

  srand48( 0 );

  for ( unsigned int i=0; i<10; ++i )
  {
    points.push_back( Point2( drand48(), drand48() ) );
  }

  kd::BoundsFactory boundsFactory;
  kd::TreeFactory treeFactory( boundsFactory );

  kd::Tree* tree = treeFactory.create( points );

  std::cerr << "Completed Testing" << std::endl;

  return 0;
}


