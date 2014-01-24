#include "LayoutVisitor.h"
#include "LayoutVisitable.h"


te::layout::LayoutVisitor::LayoutVisitor() :
  _visitable(0)
{

}

te::layout::LayoutVisitor::~LayoutVisitor( void )
{

}

void te::layout::LayoutVisitor::visit( LayoutVisitable* visitable )
{
  if(_visitable)
  {
    if(visitable)
    {
      if(_visitable != visitable)
      {
        _visitable->unacceptVisitor(this);
        _visitable = visitable;
      }
    }
    visitDependent();
  }
  else
  {
    if(visitable)
    {
      _visitable = visitable;
      visitDependent();
    }
  }
}
