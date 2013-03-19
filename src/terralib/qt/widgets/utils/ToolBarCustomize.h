#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TOOLBARCUSTOMIZE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TOOLBARCUSTOMIZE_H

#include "../Config.h"

//Qt
#include <QtGui/QWidget>

// Forward declaration
class QToolBar;
class QMenuBar;
class QListWidgetItem;

namespace Ui
{
  class ToolBarCustomizeForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT ToolBarCustomize : public QWidget
      {
        Q_OBJECT

        public:

          ToolBarCustomize(QMenuBar* mbar, std::vector<QToolBar*> tbars, QWidget* parent=0);

          ~ToolBarCustomize();

        public slots:

          void toolbarChanged(QListWidgetItem * current, QListWidgetItem * previous);

        protected:

          std::vector<QToolBar*> m_bars;

        private:

          Ui::ToolBarCustomizeForm* m_ui;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_TOOLBARCUSTOMIZE_H
