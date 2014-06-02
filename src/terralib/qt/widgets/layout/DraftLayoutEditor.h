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

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DRAFT_MAP_EDITOR_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DRAFT_MAP_EDITOR_H

// TerraLib
#include "../Config.h"

// Qt
#include <QPixmap>
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DraftLayoutEditor

        \brief A DraftLayoutEditor ....

      */
      class TEQTWIDGETSEXPORT DraftLayoutEditor : public QWidget
      {
        public:
          DraftLayoutEditor(QWidget* parent = 0, Qt::WindowFlags f = Qt::Widget);
          virtual ~DraftLayoutEditor();

          QPixmap* getDraftPixmap();
          void resizeEvent(QResizeEvent*);
          void paintEvent(QPaintEvent*);
          void mouseMoveEvent(QMouseEvent*);

        private:
          QPixmap m_draftPixmap;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DRAFT_MAP_EDITOR_H
