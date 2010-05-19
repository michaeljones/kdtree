#ifndef POINT2
#define POINT2

class Point2 
{
public:

  Point2( float coords[2] ) : x( coords[ 0 ] ), y( coords[ 1 ] ) {}
  Point2( float _x, float _y ) : x( _x ), y( _y ) {}
  Point2() : x( 0.0f ), y( 0.0f ) {}

  float operator[]( int index ) const
  {
    return data[ index ];
  }

public:

  union {
    struct {
      float x;
      float y;
    };
    float data[2];
  };
};

#endif // POINT2

