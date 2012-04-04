
#ifndef __TERRALIB_QT_QWT_INTERNAL_HISTOGRAM_DISPLAY_H_
#define __TERRALIB_QT_QWT_INTERNAL_HISTOGRAM_DISPLAY_H_

#include "Plot.h"
#include "Histogram.h"
#include "HistogramSelectionCursor.h"

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

namespace te
{
  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT HistogramDisplay : public Plot
      {
        Q_OBJECT

      public:

        HistogramDisplay(int col, te::map::DataGridOperation* op, QWidget * = NULL);
        ~HistogramDisplay();

        void closeEvent(QCloseEvent*);
        bool createHistograms();
        void updateHistograms();
        void attachHistograms();
        void updateVisibity();
        void drawItems(QPainter*, const QRectF&, const QwtScaleMap map[axisCnt] ) const;
        void setNumberOfBars(int numberOfBars);
        int getNumberOfBars();
        void setMaxNumberOfHorizontalLabels(int n);


      Q_SIGNALS:
        void selectionChanged(te::map::DataGridOperation*);
        void closed(QwtPlot*);

      protected:
        void select(double, double, std::vector<int>&);
        void selectString(double, double, std::vector<int>&);
        void selectDateTime(double, double, std::vector<int>&);

      protected Q_SLOTS:
        void showHistogramSlot(QwtPlotItem* , bool);
        void legendMenuSlot(QPoint&, QWidget*);
        void legendToFrontSlot();
        void legendToBackSlot();
        void legendColorSlot();
        void selectSlot(int, int);
        void buttonClickedSlot(QAbstractButton*);
        void selectionChangedSlot(te::map::DataGridOperation*);
        void wheelEventSlot(int);

      private:
        std::map<double, int> m_allValues;
        Histogram* m_allHistogram;
        Histogram* m_deselectedHistogram;
        Histogram* m_pointedHistogram;
        Histogram* m_queriedHistogram;
        Histogram* m_pointedAndQueriedHistogram;
        std::vector<Histogram*> m_histogramVec; // para controlar a ordem de desenho dos histogramas
        std::map<Histogram*, bool> m_histogramMap; // para controlar a visibilidade dos histogramas
        QButtonGroup* m_buttonGroup; // para selecionar a acao do mouse
        QRadioButton* m_radioButton;
        QWidget* m_legendWidget; // para informar qual histograma vai para toFront ou toBack
        HistogramSeletionCursor* m_selectionCursor;
        QMenu* m_legendMenu; // para colocar menu: toFront e toBack
      };
    }
  }
}
#endif
