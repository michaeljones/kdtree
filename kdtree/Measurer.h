#ifndef MEASURER
#define MEASURER

namespace kd 
{

class Measurer
{
public:
  Measurer() {}

  template< typename P, unsigned int DIM >
  typename P::base_type distanceSq( const P& a, const P& b ) const
  {
    typename P::base_type length( 0 );

    for ( unsigned int i=0; i<DIM; ++i )
    {
      typename P::base_type sep = a[ i ] - b[ i ];
      length += sep*sep;
    }

    return length;
  }
};


}; // namespace kd

#endif // MEASURER
