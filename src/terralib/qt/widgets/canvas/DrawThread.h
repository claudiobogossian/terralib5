/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

This file is part of the TerraLib - a Framework for building GIS enabled applications.

TerraLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

TerraLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with TerraLib. See COPYING. If not, write to
TerraLib Team at <terralib-team@terralib.org>.
*/

/*!
\file terralib/qt/widgets/canvas/DrawThread.h

\brief Thread to draw a Layer in a PaintDevice.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DRAWTHREAD_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DRAWTHREAD_H

// Qt
#include <QColor>
#include <QObject>
#include <QRunnable>

class QPaintDevice;
class QSize;

namespace te
{
  namespace gm
  {
    class Envelope;
  }

  namespace map
  {
    class AbstractLayer;
    enum AlignType;
  }

  namespace qt
  {
    namespace widgets
    {
      class DrawThread : public QObject, public QRunnable
      {
        Q_OBJECT

      public:

        DrawThread(QPaintDevice* dev, te::map::AbstractLayer* layer, te::gm::Envelope* env, const QColor& bckGround, int srid, double scale, 
                   te::map::AlignType hAlign, te::map::AlignType vAlign);

        ~DrawThread();

        void run();

        bool hasFinished() const;

        QString errorMessage() const;

        QString layerId() const;

       Q_SIGNALS:

        void finished();

      protected:

        QPaintDevice* m_device;
        
        te::map::AbstractLayer* m_layer;

        te::gm::Envelope* m_envelope;

        QColor m_bckGround;

        int m_srid;

        double m_scale;

        te::map::AlignType m_hAlign;                        //!< The display horizontal align.

        te::map::AlignType m_vAlign;                        //!< The display vertical align.

      public: 

        bool m_cancel;

      protected:

        bool m_finished;

        QString m_errorMessage;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_DRAWTHREAD_H