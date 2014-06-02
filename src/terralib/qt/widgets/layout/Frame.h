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
  \file terralib/qt/widgets/canvas/FrameData.h

  \brief The main class of FrameData.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_FRAME_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_FRAME_H

// TerraLib
#include "LayoutObject.h"
#include "../Config.h"

// Qt
#include <QRect>
#include <QRectF>
#include <QPixmap>
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class LayoutEditor;
      /*!
        \class FrameData

        \brief A Framedata ....

      */
      class TEQTWIDGETSEXPORT Frame : public LayoutObject
      {
        public:
          Frame(te::qt::widgets::LayoutEditor* me, Qt::WindowFlags f = Qt::Widget);
          Frame(const Frame& rhs);

          virtual ~Frame();

          Frame& operator=(const Frame& rhs);

          bool isNew();
          void setNew(bool);
          void showToolTip(bool b);
          QRectF getFrameRect();

        protected:
          int m_selected; // ponto selecionado
          bool m_dragging; // para edicao
          bool m_showToolTip; // true = mostra tool tip para edição por mouse
          bool m_new; // se true, indica ao undo que deve ser deletado (pois o frame foi criado por edicao durante a secao).

          QRectF m_frameRect; // tamanho do frame em mm

          // selection points em mm
          QRect m_recSel1;
          QRect m_recSel2;
          QRect m_recSel3;
          QRect m_recSel4;
          QRect m_recSel5;
          QRect m_recSel6;
          QRect m_recSel7;
          QRect m_recSel8;
          QRect m_recSel9;
          QRect m_recSel10;


          QRectF m_auxFrameRect; // para edicao
          QRectF m_copyAuxFrameRect; // para edicao
          QPoint m_pressPoint; // para edicao
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DATA_FRAME_H
