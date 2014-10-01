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

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_EDITOR_INFO_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_EDITOR_INFO_H

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
      class TEQTWIDGETSEXPORT EditorInfo : public LayoutObject
      {
        public:
          EditorInfo(te::qt::widgets::LayoutEditor* me, Qt::WindowFlags f = Qt::Widget);
          EditorInfo(const EditorInfo& rhs);

          virtual ~EditorInfo();

          EditorInfo& operator=(const EditorInfo& rhs);
          void setEditorChanged(bool);
          bool isEditorChanged();

        public:
          QRect m_rect; // rect of editor Widget 
          QRectF m_paperViewRect; // editor paper view rect
          QSize m_paperSize; // paper size
          bool m_showRulerGrid; // true = show
          int m_rulerGridLines; // grid options: 1 = all, 2 = only medium and large, 3 = only large
          bool m_resize;
          bool m_move;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_EDITOR_INFO_H
