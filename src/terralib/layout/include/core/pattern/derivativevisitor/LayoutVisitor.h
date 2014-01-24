#ifndef LAYOUTVISITOR_H
#define LAYOUTVISITOR_H

namespace te
{
  namespace layout
  {
    class LayoutVisitable;

    class LayoutVisitor
    {
      public:

        LayoutVisitor();
        ~LayoutVisitor(void);

        virtual void visit(LayoutVisitable* visitable);

        virtual void visitDependent() = 0;

      protected:
        LayoutVisitable* _visitable;
    };
  }
}

#endif