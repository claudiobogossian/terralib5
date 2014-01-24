#include "LayoutVisitable.h"
#include "LayoutVisitor.h"


te::layout::LayoutVisitable::LayoutVisitable()
{

}

te::layout::LayoutVisitable::~LayoutVisitable( void )
{

}

void te::layout::LayoutVisitable::acceptVisitor( LayoutVisitor* visitor )
{
  _visitors.insert(visitor);
	
  //allow the incoming visitor to start visiting
  visitor->visit(this);
}

void te::layout::LayoutVisitable::updateVisitors()
{
  std::set<LayoutVisitor*>::iterator it;
  for(it = _visitors.begin(); it != _visitors.end(); ++it)
  {
  (*it)->visit(this);
  }
}

void te::layout::LayoutVisitable::unacceptVisitor( LayoutVisitor* visitor )
{
  _visitors.erase(visitor);
}

bool te::layout::LayoutVisitable::operator==( const LayoutVisitable* &other ) const
{
  //void * vp = dynamic_cast<void *>(_visitable);

  /*if(*this == other)
  return true;*/
  return false;
}

bool te::layout::LayoutVisitable::operator!=( const LayoutVisitable* &other ) const
{
  /*if(!(*this == other))
  return true;*/
  return false;
}
