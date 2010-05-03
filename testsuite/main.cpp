
#include <kdtree.h>

#include <stdlib.h>


int main( int argc, char** argv )
{
  std::vector< Point3 > points;

  for ( unsigned int i=0; i<10; ++i )
  {
    points.push_back( Point3( drand48(), drand48(), drand48() ) );
  }

  kd::TreeFactory treeFactory;

  kd::Tree tree = treeFactory.create( points );

  return 0;
}


