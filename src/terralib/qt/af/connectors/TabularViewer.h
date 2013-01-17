/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

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
  \file connectors/TabularViewer.h

  \brief A connector of the te::qt::widgets::TabularViewer for the application framework.
 */

#ifndef __TERRALIB_QT_AF_INTERNAL_TABULARVIEWER_H
#define __TERRALIB_QT_AF_INTERNAL_TABULARVIEWER_H

//! Terralib include files
#include <terralib/qt/af/Config.h>

//! Qt include files
#include <QObject>
#include <QColor>

//! STL include files
#include <map>
#include <vector>

namespace te
{
  //! Forward declarations
  namespace da
  {
    class DataSet;
    class DataSourceTransactor;
  }

  namespace gm
  {
    class Geometry;
  }

  namespace map
  {
    class Layer;
  }

  namespace qt
  {
    //! Forward declarations
    namespace widgets
    {
      class TabularViewer;
    }

    namespace af
    {
      //! Forward declarations
      class Event;

      /*!
        \class TabularViewer
        
        \brief A connector of the te::qt::widgets::TabularViewer for the application framework.
        
        This is used to listen events sent by application framework and acts on a te::qt::widgets::TabularViewer object.
        
        \ingroup afconnector
      */
      class TEQTAFEXPORT TabularViewer : public QObject
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.
          */
          TabularViewer(te::qt::widgets::TabularViewer* viewer);

          /*!
            \brief destructor.
          */
          ~TabularViewer();

        protected slots:

          /*!
            \brief Listener to the application framewrork events.
          */
          void onApplicationTriggered(te::qt::af::Event* evt);

          void pointedObjects(const std::map<std::string, te::gm::Geometry*>& geoms);

          void colorChanged(const int& g, const QColor& c);

        protected:

          void updateColors(te::map::Layer* layer);

          std::vector<QColor>* getColorVector(te::map::Layer* layer);

          te::qt::widgets::TabularViewer* m_viewer; //!< Pointer to a component TabularViewer.
          te::da::DataSet* m_dset;
          te::da::DataSourceTransactor* m_trans;
          te::map::Layer* m_layer;                  //!< Layer being presented.
          std::map< te::map::Layer*, std::vector<QColor> > m_colors_map;
      };
    }
  }
}

#endif //! __TERRALIB_QT_AF_INTERNAL_TABULARVIEWER_H
