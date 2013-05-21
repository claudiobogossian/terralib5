/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/tools/Info.h

  \brief This class implements a concrete tool to get informations about a specified feature using pointing operation.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_INFO_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_INFO_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"
#include "AbstractTool.h"

// Qt
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

// STL
#include <list>

namespace te
{
  namespace da
  {
    class DataSet;
  }

  namespace gm
  {
    class Envelope;
    class Geometry;
  }

  namespace rst
  {
    class Raster;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
    class MapDisplay;

      /*!
        \class Info

        \brief This class implements a concrete tool to get informations about a specified feature using pointing operation.
      */
      class TEQTWIDGETSEXPORT Info : public AbstractTool
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a info tool associated with the given map display.

            \param display The map display associated with the tool.
            \param layers  The layer list that will be queried.
            \param parent  The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
          */
          Info(MapDisplay* display, const std::list<te::map::AbstractLayerPtr>& layers, QObject* parent = 0);

          /*! \brief Destructor. */
          ~Info();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool mouseReleaseEvent(QMouseEvent* e);

          //@}

        private:

          void getInfo(const te::map::AbstractLayerPtr& layer, const te::gm::Envelope& e);

          void getGeometryInfo(QTreeWidgetItem* layerItem, te::da::DataSet* dataset, const te::gm::Envelope& e, int srid, bool needRemap);

          void getRasterInfo(QTreeWidgetItem* layerItem, te::rst::Raster* raster, const te::gm::Envelope& e, int srid, bool needRemap);

          void drawGeometry(te::gm::Geometry* g);

        private:

          const std::list<te::map::AbstractLayerPtr>& m_layers; //!< A pointer to layer list that will be queried.
          QTreeWidget* m_infoWidget;                            //!< Widget used to show the informations.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_INFO_H
