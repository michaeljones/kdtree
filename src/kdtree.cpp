
#include "kdtree.h"

#include <stack>

namespace kd
{


class NonTerminalNode : public Node 
{
public:
  NonTerminalNode() {}
  virtual ~NonTerminalNode() {}

  virtual void connect( Node* child ) = 0;

};


class RootNode : public NonTerminalNode
{
public:
  RootNode()
   : m_child( 0 ) {}

  void connect( Node* child ) { m_child = child; }

private:

  Node* m_child;

};

class SplitNode : public NonTerminalNode
{
public:
  SplitNode()
   : m_left( 0 ), m_right( 0 ) {}

  void connect( Node* child )
  {
    if ( ! m_left ) 
      m_left = child;
    else
      m_right = child;
  }

private:

  Node* m_left;
  Node* m_right;
};

class EmptyNode : public Node
{
public:
  EmptyNode() {}

};

class SingleNode : public Node
{
public:

  SingleNode( Point2& p ) : m_point( p ) {}

private:

  Point2 m_point;
};


typedef std::vector< Point2 > Subset;
typedef std::pair< NonTerminalNode*, const Subset* > Item;


bool cmp( Point2 a, Point2 b )
{
  return a.x > b.x;
}





Tree TreeFactory::create( const Subset& points )
{
  
  std::stack< Item > stack;

  NonTerminalNode* rootNode = new RootNode();
  stack.push( Item( rootNode, &points ) ); 

  while ( stack.size() )
  {
    Item item = stack.top();
    stack.pop();

    Node& node = *(item.first);
    const Subset& subset = *(item.second);

    if ( subset.size() == 0 )
    {
      Node* childNode = new EmptyNode;
      node.connect( childNode );
      continue;
    }

    if ( subset.size() == 1 )
    {
      Node* childNode = new SingleNode( subset[ 0 ] );
      node.connect( childNode );
      continue;
    }

    std::sort( subset.begin(), subset.end(), cmp );

    int medianIdx = int( subset.size() / 2.0f );

    Point2 medianPoint = subset[ medianIdx ];

    NonTerminalNode* childNode = new SplitNode( medianPoint );

    Subset* left = new Subset( median );
    Subset* right = new Subset( subset.size() - ( median + 1 ) );

    // TODO: Check for overrunning indicies
    std::copy( subset.begin(), subset.begin() + medianIdx, left->begin() );
    std::copy( subset.begin() + medianIdx + 1, subset.end(), right->begin() );

    stack.push( Item( *childNode, *left ) );
    stack.push( Item( *childNode, *right ) );
  }
}


}

