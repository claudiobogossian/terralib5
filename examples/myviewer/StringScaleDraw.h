#ifndef __TERRALIB_QT_QWT_INTERNAL_STRING_SCALE_DRAW_H_
#define __TERRALIB_QT_QWT_INTERNAL_STRING_SCALE_DRAW_H_

//QWT
#include <qwt_scale_draw.h>

//STL
#include <set>

//TerraLib
#include "../widgets/Config.h"

namespace te
{
  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT StringScaleDraw: public QwtScaleDraw
      {
      public:
        StringScaleDraw();
        StringScaleDraw(const std::set<QString>&);
        StringScaleDraw(int);
        ~StringScaleDraw();

        virtual QwtText label( double value ) const;
        void setTimeType(int);
        void setStringSet(const std::set<QString>&);

      private:
        std::set<QString> m_values;
        int m_timeType;
      };
    }
  }
}
#endif