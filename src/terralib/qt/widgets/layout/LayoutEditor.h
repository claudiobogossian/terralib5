/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A Free and Open Source GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*!
  \file terraview/LayoutEditor.h

  \brief The main class of LayoutEditor.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_MAP_EDITOR_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_MAP_EDITOR_H


// TerraLib
#include <terralib/geometry.h>
#include <terralib/maptools.h>
#include <terralib/srs.h>
#include "../Config.h"

// Qt
#include <QAction>
#include <QMenu>

#include <QRect>
#include <QMatrix>
#include <QWidget>
#include <QPaintEvent>
#include <QResizeEvent>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class LayoutObject;
      class DataFrame;
      class EditorInfo;
      class DraftLayoutEditor;
      /*!
        \class LayoutEditor

        \brief A map editor ....

        \sa 
      */
      class TEQTWIDGETSEXPORT LayoutEditor : public QWidget
      //class TEQTWIDGETSEXPORT LayoutEditor : public QWidget, public te::common::Singleton<LayoutEditor>
      {
//        friend class te::common::Singleton<LayoutEditor>;

        public:
          LayoutEditor(QWidget* parent = 0, Qt::WindowFlags f = Qt::Window );
          LayoutEditor(const QSize& paperSize, QWidget* parent = 0, Qt::WindowFlags f = Qt::Window );
          virtual ~LayoutEditor();

          void createWorkingArea(bool undo = true);
          void insertRulers();
          void createRulerGrid();
          void showRulerGrid();
          void hideRulerGrid();
          void insert(te::qt::widgets::LayoutObject*);
          void remove(te::qt::widgets::LayoutObject*);
          void insertCopy2Undo(te::qt::widgets::LayoutObject*);
          void removeUndo(te::qt::widgets::LayoutObject*);
          te::qt::widgets::LayoutObject* find(unsigned int);
          void paintEvent(QPaintEvent* event);
          void resizeEvent(QResizeEvent* event);
          void moveEvent(QMoveEvent* event);
          void setPaperSize(const QSize& size);
          QSize getPaperSize();
          QRectF getPaperViewRect();
          int getHorizontalRulerWidth();
          int getVerticalRulerWidth();
          QWidget* getAuxWidget();
          void setUndoBufferSize(const int& size);
          int getUndoBufferSize();
          void appendToUndo();
          void copyState();
          void pan(const QPointF& p);
          void draw();
          void drawButtonClicked();
          void drawLayersSelection();
          void setSelectionColor(QColor selColor);
          void setDisplayBackgroundColor(QColor);
          void resetPaperView();
          QMatrix getMatrixPaperViewToVp();
          QMatrix getMatrix();
          QPixmap* getDraftPixmap();
          void raiseDraftLayoutEditor();
          void lowerDraftLayoutEditor();
          void drawRectArea(); 
          void adjustAspectRatio(QRectF& r, const QRectF& ref); // faz a relacao de aspecto de r igual a do ref
          void setMouseMode(int); // 0=none, 1=zoomin, 2=zoomout, 3=pan (sobre a area do papel/dado)
          void hideAllObjects();
          void showAllObjects();

          void mousePressEvent(QMouseEvent* e);
          void mouseMoveEvent(QMouseEvent* e);
          void mouseReleaseEvent(QMouseEvent* e);
          void wheelEvent(QWheelEvent* e);
          void keyPressEvent(QKeyEvent* e);
          void sendEventToChildren(bool b); // true para os layers objects receberem eventos.
          bool eventFilter(QObject*, QEvent*); // deixa passar apenas o event de paint quando o filtro é instalado.

          void setFrameSelected(te::qt::widgets::LayoutObject*);
          te::qt::widgets::LayoutObject* getFrameSelected();

        private:
          unsigned int m_idCount;
          QSize m_paperSize;
          int m_horizontalRulerWidth;
          int m_verticalRulerWidth;
          te::qt::widgets::LayoutObject* m_layoutObjectSelected;
          bool m_dataPan; // flag for datab pan execution
          bool m_showRulerGrid;
          int m_rulerGridLines; // grid options: 1 = all, 2 = only medium and large, 3 = only large
          int m_undoBufferSize;
          te::qt::widgets::EditorInfo* m_editorState;
          bool m_resize; // true indica resize operation
          bool m_move; // true indica move operation


          // IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE ////////////////////
          QWidget* m_auxWidget; // support to view port area. Area interna do LayoutEditor que exclui as reguas. Ele é necessario
                                // para que os widgets de frame nao escondam as reguas.
                                // Todos os frames sao inicializados com LayoutEditor*, mas, a classe base (LayoutObject) é iniciada com m_auxWidget
          ////////////////////////////////////////////////////////////////////////
          DraftLayoutEditor* m_draftLayoutEditor; // to edition. É composta da parte interna (sem as reguas).
          QMatrix m_matrixPaperViewToVp;
          QMatrix m_matrix;
          QRectF m_paperViewRect;
          QRect m_rect; // used for resize operation
          QPointF m_pos; // used for resize operation
          QPointF m_difTopLeft; // offset used to resize operation
          QPointF m_difBottomRight; // offset used to resize operation
          bool m_paperViewReseted;
          QPixmap m_totalPixmap;
          QPixmap m_viewPixmap;
          std::vector<te::qt::widgets::LayoutObject*> m_layoutObjects;
          std::vector<te::qt::widgets::LayoutObject*> m_undoLayoutObjects;
          bool m_putUndo;
          QPointF m_pressPoint;
          QPointF m_startPan; // for pan operation (viewport coordenate) - paper pan and data pan
          int m_rulerSmall;
          int m_rulerMedium;
          int m_rulerLarge;
          QPixmap m_rulerGridPixmap;

          int m_zmouseMode; // 0=none, 1=zoomin, 2=zoomout, 3=pan (sobre a area do papel/dado)
          QPoint m_zpressPoint;
          QPoint m_zpoint;
          QRect m_zrect;
          bool m_zpanEnd;
          QPixmap m_zpixmap;
          te::qt::widgets::DataFrame* m_zdataFrame;
          QPixmap* m_zoomInPixmap;
          QPixmap* m_zoomOutPixmap;
          QPixmap* m_panPixmap;

      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_MAP_EDITOR_H
