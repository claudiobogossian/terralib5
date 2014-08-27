/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/processing/qt/RasterToVectorDialog.h

  \brief Raster to vector processing dialog.
*/

#ifndef __TERRALIB_PROCESSING_INTERNAL_RASTERTOVECTORDIALOG_H
#define __TERRALIB_PROCESSING_INTERNAL_RASTERTOVECTORDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../statistics/core/Enums.h"
#include "../Config.h"

// STL
#include <list>
#include <map>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class RasterToVectorDialogForm; }

// Forward declarations
class QListWidgetItem;
class QModelIndex;
class QTreeWidgetItem;

namespace te
{
  namespace processing
  {
    class TEPROCESSINGEXPORT RasterToVectorDialog : public QDialog
    {
      Q_OBJECT

      public:

        RasterToVectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~RasterToVectorDialog();


        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        /*!
          \brief Get the generated layer.

          \return the generated layer.
        */
        te::map::AbstractLayerPtr getLayer();

      private:

        std::auto_ptr<Ui::RasterToVectorDialogForm> m_ui;
        std::list<te::map::AbstractLayerPtr> m_layers;                //!< List of layers.
        te::map::AbstractLayerPtr m_layer;                            //!< Generated Layer.
    };
  }   // end namespace processing
}     // end namespace te

#endif  // __TERRALIB_PROCESSING_INTERNAL_RASTERTOVECTORDIALOG_H
