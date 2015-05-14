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
  \file terralib/myqt/widgets/mapdisplay/DataSetDisplay.h

  \brief A map display for a dataset.
*/

#ifndef __TERRALIB_QT_WIDGETS_MAPDISPLAY_INTERNAL_DATASETDISPLAY_H
#define __TERRALIB_QT_WIDGETS_MAPDISPLAY_INTERNAL_DATASETDISPLAY_H

// TerraLib
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QFrame>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declaration
      class Canvas;

      /*!
        \class DataSetDisplay

        \brief A map display for a dataset.
      */
      class TEQTWIDGETSEXPORT DataSetDisplay : public QFrame
      {
        Q_OBJECT

        public:

          DataSetDisplay(QWidget * parent = 0, Qt::WindowFlags f = 0);

          ~DataSetDisplay();

          void draw(const te::da::DataSetTypePtr& dataset, const te::da::DataSourcePtr& ds, te::da::DataSet* datasetData = 0);

          void draw(const te::da::DataSetTypePtr& dataset, const te::da::DataSourceInfoPtr& ds);

          void clear();

        protected:

          void paintEvent(QPaintEvent* e);

          void resizeEvent(QResizeEvent* e);

        private:

          std::auto_ptr<Canvas> m_canvas;
          //std::auto_ptr<te::gm::Envelope> m_extent;
          //te::da::DataSourcePtr m_ds;
          //te::da::DataSetTypePtr m_dataset;
          
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_MAPDISPLAY_INTERNAL_DATASETDISPLAY_H

