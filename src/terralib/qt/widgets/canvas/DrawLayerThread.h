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
  \file DrawLayerThread.h

  \brief This class represents a thread responsible to draw a given layer. Basically, this class receives draw layer requests and generates a QImage as result.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DRAWLAYERTHREAD_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DRAWLAYERTHREAD_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../Config.h"

// Qt
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QImage>

namespace te
{
// Forward declarations
  namespace map { class AbstractLayer; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DrawLayerThread

        \brief This class represents a thread responsible to draw a given layer.
               Basically, this class receives draw layer requests and generates a QImage as result.
      */
      class TEQTWIDGETSEXPORT DrawLayerThread : public QThread
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a new thread responsible to draw a given layer.

            \param parent The thread's parent.
          */
          DrawLayerThread(QObject* parent = 0);

          /*! \brief Destructor. */
          ~DrawLayerThread();

          //@}

          /*!
            \brief This method is used to request the draw of the given layer.

            \param layer The layer that will be draw.
            \param box The interest area to draw the layer.
            \param srid The SRS to be used to draw the layer objects.
            param scale The current scale of display.
            \param size The result size, in pixels; e.g. (800 x 600).
            \param index An optional index that can be provided by the caller to keep the draw order.
          */
          void draw(te::map::AbstractLayer* layer, const te::gm::Envelope& box, int srid, const double& scale, const QSize& size, const int& index);

           /*! \brief This method tells if the thread finished with success. */
          bool finishedWithSuccess() const;

          /*! \brief This method returns an error message if the thread has not finished with success. Otherwise, the error message is empty. */
          QString getErrorMessage() const;

          /*! \brief This method returns the layer handled by this thread. */
          te::map::AbstractLayer* getLayer() const;

        protected:

          /* \brief Starts the thread. i.e. performs the layer draw. */
          void run();

        protected slots:

          /* \brief Called right before thread start execution. */
          void onStarted();

          /* \brief Used to send a draw feedback. */
          void sendFeedback();

          /* \brief Called right after thread stop execution. */
          void onFinished();

        signals:

          /*!
            \brief This signal is emitted during the draw process.

            \param image The partial result of draw process.
          */
          void feedback(const QImage& image);

          /*!
            \brief This signal is emitted when the draw process ends.

            \param index The thread index.
            \param image The final result of draw process.
          */
          void drawLayerFinished(const int& index, const QImage& image);

        protected:

          te::map::AbstractLayer* m_layer; //!< The layer that will be drawn.
          te::gm::Envelope m_env;          //!< The interest area to draw the layer.
          int m_srid;                      //!< The SRS to be used to draw the layer objects.
          int m_index;                     //!< An optional index that can be provided by the caller to keep the draw order.
          double m_scale;                  //!< Scale value used to draw the layer.
          QImage m_image;                  //!< The image that will be generated during the draw process.
          QMutex m_mutex;                  //!< Controls the serialization between threads.
          QTimer m_feedback;               //!< Timer used to send feedback. The feedback will be sent right after timeout() QTimer's signal.
          int m_interval;                  //!< Interval used to send feedbacks.
          bool m_finishedWithSuccess;      //!< A flag that indicates if the layer could be drawn.
          QString m_errorMessage;          //!< A string that contains an error message.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DRAWLAYERTHREAD_H
