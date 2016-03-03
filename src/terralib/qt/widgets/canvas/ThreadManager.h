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
\file terralib/qt/widgets/canvas/ThreadManager.h

\brief Manager for threads of rendering.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_THREADMANAGER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_THREADMANAGER_H

//Qt
#include <QObject>

//STL
#include <vector>

class QRunnable;
class QThreadPool;
class QTimer;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class ThreadManager : public QObject
      {
        Q_OBJECT

      public:

        ThreadManager(const std::vector<QRunnable*>& threads, int interval=-1);

        ~ThreadManager();

        void run();

        void stopProccess();

      Q_SIGNALS:

        void showFeedback();

        void finished();

      protected slots:

        void onThreadFinished();

      protected:

        std::vector<QRunnable*> m_threads;

        QThreadPool* m_pool;

        QTimer* m_timer;

        int m_interval;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_THREADMANAGER_H