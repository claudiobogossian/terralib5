#ifndef __TERRALIB_QT_QWT_INTERNAL_COLORBAR_H_
#define __TERRALIB_QT_QWT_INTERNAL_COLORBAR_H_

//QWT

#include <qwt/qwt_scale_widget.h>

//STL
#include <map>

//TerraLib
#include "../widgets/Config.h"


namespace te
{
  namespace color
  {
    class ColorBar;
  }

  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT ColorBar: public QwtScaleWidget
      {
        public:

          ColorBar(QWidget* parent = 0);

          ~ColorBar();

        public:

          void setHeight(int value);

          void setInterval(double min, double max);

          void setColorBar(te::color::ColorBar* cb);

          te::color::ColorBar* getColorBar();


        protected:

          void setScaleEngine();

          void buildColorBar();

          virtual void paintEvent(QPaintEvent* e);

          virtual void mouseDoubleClickEvent(QMouseEvent* e);


        protected:

          te::color::ColorBar* m_colorBar;       //!< Color map information
          
          QwtInterval m_interval;

          int m_height;
      };
    }
  }
}
#endif // __TERRALIB_QT_QWT_INTERNAL_COLORBAR_H_