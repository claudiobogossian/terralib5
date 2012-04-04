
#ifndef __TERRALIB_QT_QWT_INTERNAL_SCATTER_DISPLAY_H_
#define __TERRALIB_QT_QWT_INTERNAL_SCATTER_DISPLAY_H_

#include "Plot.h"
#include "Scatter.h"
#include "ScatterSelectionCursor.h"

//QWT
#include <qwt_legend.h>

//QT
#include <QEvent>
#include <QMenu>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCursor>

//TerraLib
#include "../../dataaccess.h"
#include "../../maptools.h"
#include "../widgets/Config.h"

namespace te
{
  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT ScatterDisplay : public Plot
      {
        Q_OBJECT

      public:

        ScatterDisplay(int colx, int coly, te::map::DataGridOperation* op, QWidget * = NULL);
        ~ScatterDisplay();

        void updateVisibity();

        bool createScatters();
        double getXValue(const QString& s);
        double getYValue(const QString& s);
        void attachScatters();
        void updateScatters();

        void drawItems(QPainter*, const QRectF&, const QwtScaleMap map[axisCnt] ) const;

      Q_SIGNALS:
        void selectionChanged(te::map::DataGridOperation*);

      protected:
        void select(QRectF rec, std::vector<int>& visRows);

      protected Q_SLOTS:
        void showScatterSlot(QwtPlotItem* , bool);
        void legendMenuSlot(QPoint&, QWidget*);
        void legendToFrontSlot();
        void legendToBackSlot();
        void legendColorSlot();
        void selectSlot(QRect);
        void buttonClickedSlot(QAbstractButton*);
        void selectionChangedSlot(te::map::DataGridOperation*);

      private:
        Scatter* m_allScatter;
        Scatter* m_deselectedScatter;
        Scatter* m_pointedScatter;
        Scatter* m_queriedScatter;
        Scatter* m_pointedAndQueriedScatter;
        std::vector<Scatter*> m_scatterVec; // para controlar a ordem de desenho dos scatters
        std::map<Scatter*, bool> m_scatterMap; // para controlar a visibilidade dos scatters
        QButtonGroup* m_buttonGroup; // para selecionar a acao do mouse
        QRadioButton* m_radioButton;
        QWidget* m_legendWidget; // para informar qual scatter vai para toFront ou toBack
        ScatterSelectionCursor* m_selectionCursor;
        QMenu* m_legendMenu; // para colocar menu: toFront e toBack
      };
    }
  }
}
#endif
