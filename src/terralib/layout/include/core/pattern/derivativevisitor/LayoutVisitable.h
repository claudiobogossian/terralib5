#ifndef LAYOUTVISITABLE_H
#define LAYOUTVISITABLE_H

#include <set>

namespace te
{
  namespace layout
  {
    class LayoutVisitor;

    class LayoutVisitable
    {
      public:

        LayoutVisitable();
        ~LayoutVisitable(void);

        virtual void acceptVisitor(LayoutVisitor* visitor);
        virtual void unacceptVisitor(LayoutVisitor* visitor);

        bool operator==(const LayoutVisitable* &other) const;
        bool operator!=(const LayoutVisitable* &other) const;

      protected:
        virtual void updateVisitors();

      protected:
        std::set<LayoutVisitor*>	_visitors;
    };
  }
}
#endif