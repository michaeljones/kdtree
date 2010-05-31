#ifndef DATA
#define DATA

namespace kd 
{

template< typename P >
struct NeighbourData
{
  NeighbourData( bool f, P p, float d )
    : found( f ), point( p ), distanceSq( d ) {}

  bool found;
  P point;
  typename P::base_type distanceSq;
};


}; // namespace kd

#endif // DATA
