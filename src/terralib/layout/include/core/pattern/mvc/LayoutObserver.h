#ifndef LAYOUTOBSERVER_H
#define LAYOUTOBSERVER_H

#include "ContextLayoutItem.h"

namespace te
{
  namespace layout
  {
    class LayoutObserver
    {
      public:
        virtual ~LayoutObserver(void) {}
        virtual void updateObserver(ContextLayoutItem context) = 0;
    };
  }
}

#endif