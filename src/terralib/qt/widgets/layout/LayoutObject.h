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

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_LAYOUT_OBJECT_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_LAYOUT_OBJECT_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class LayoutEditor;
      class TEQTWIDGETSEXPORT LayoutObject : public QWidget
      {
        public:
          LayoutObject(te::qt::widgets::LayoutEditor* me, Qt::WindowFlags f = Qt::Widget); // LayoutEditor não é o pai
          LayoutObject(const LayoutObject& rhs);

          virtual ~LayoutObject();

          LayoutObject& operator=(const LayoutObject& rhs);

          unsigned int getId();
          void setId(unsigned int);
          te::qt::widgets::LayoutEditor* getLayoutEditor();
          void setLayoutEditorChanged(bool);
          bool getLayoutEditorChanged();
          virtual void hide();
          virtual void show();
          virtual void adjust();
          virtual void sendEventToChildren(bool);
          virtual void showSelectionPoints();
          virtual void hideSelectionPoints();
          virtual double getScale();
          virtual void setScale(double v);
          virtual bool isNew();
          virtual void setNew(bool);
          virtual QRectF getFrameRect();
          virtual QPixmap* getPixmap();


        protected:
          unsigned int m_id; // identificador do frame (utilizado para fazer undo).
                             // Serve tambem para identificar mudancas no map editor (id = 0). Tais como resize, zoom, formato, etc.
          te::qt::widgets::LayoutEditor* m_layoutEditor;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_LAYOUT_OBJECT_H
