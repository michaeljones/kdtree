#ifndef POINT2
#define POINT2


class Point2 
{
public:

  typedef float base_type;

  Point2( float coords[2] ) { data[0] = coords[0]; data[1] = coords[1]; }
  Point2() { data[0] = 0.0f; data[1] = 0.0f; }

  float& operator[]( int index )
  {
    return data[ index ];
  }

  float operator[]( int index ) const
  {
    return data[ index ];
  }



public:

  float data[ 2 ];
};

#endif // POINT2

