#ifndef DATA
#define DATA

#include <list>

namespace kd 
{

/*! \brief Basic data for a single nearest neighbour query
 */
template< typename P >
struct NeighbourData
{
  NeighbourData( bool f, P p, typename P::base_type d )
    : found( f ), point( p ), distanceSq( d ) {}

  bool found;
  P point;
  typename P::base_type distanceSq;
};


/*! \brief Data for a group nearest neighbours query
 */
template< typename P >
class MultiNeighbourData
{
public:

  /*! \brief Location and distance squared to a neighbour point
   */
  struct PointDistance
  {
    PointDistance( const P& p, typename P::base_type d )
      : distSq( d ), point( p ) {}
    
    typename P::base_type distSq;
    P point;
  };

  typedef std::list< PointDistance > PointDistanceList;

  MultiNeighbourData( unsigned int num, typename P::base_type dist )
    : m_nthDistanceSq( dist ), m_maxNeighbours( num ) { }


  /*! \brief Update neighbour data to include provide point if desired
   */
  void update( const P& point, typename P::base_type distSq )
  {
    if ( distSq < m_nthDistanceSq )
    {
      typename PointDistanceList::iterator it = m_points.begin();
      typename PointDistanceList::iterator end = m_points.end();

      for( ; it != end; ++it )
      {
        if ( it->distSq > distSq )
          break;
      }

      // When it is nearest than one of our points insert it
      m_points.insert( it, PointDistance( point, distSq ) );

      // Crop our list at the desired size
      if ( m_points.size() > m_maxNeighbours )
      {
        m_points.pop_back();

        // Update our nthDistanceSq
        m_nthDistanceSq = m_points.back().distSq;
      }
    }
  }

  //! Return the distance squared to the furtherest neighbour found
  typename P::base_type maxDistanceSq() const
  {
    return m_nthDistanceSq;
  }

  //! Returns true if we have not found as many neighbours as we would like
  bool incomplete() const
  {
    return m_points.size() < m_maxNeighbours;
  }

  //! Returns a list of the points (and their distances) found
  const PointDistanceList& points() const
  {
    return m_points;
  }

private:

  PointDistanceList m_points;
  typename P::base_type m_nthDistanceSq;

  unsigned int m_maxNeighbours;

};



}; // namespace kd

#endif // DATA
