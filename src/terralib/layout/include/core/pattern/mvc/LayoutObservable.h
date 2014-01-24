#ifndef LAYOUTOBSERVABLE_H
#define LAYOUTOBSERVABLE_H

#include "ContextLayoutItem.h"
#include "LayoutProperties.h"

namespace te
{
  namespace layout
  {
    class LayoutObserver; 

    class LayoutObservable 
    {
      public:

        virtual ~LayoutObservable(void) {}
        virtual bool addObserver(LayoutObserver* o) = 0;
        virtual bool removeObserver(LayoutObserver* o) = 0;
        virtual LayoutProperties toString() = 0;

      protected:
        virtual void notifyAll(ContextLayoutItem context) = 0;
      };
  }
}

#endif