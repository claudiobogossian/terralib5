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
  \file terralib/qt/widgets/se/GroupingDialog.h

  \brief A dialog used to build a grouping.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_GROUPINGDIALOG_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_GROUPINGDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/Grouping.h"
#include "../Config.h"

// Qt
#include <QtGui/QDialog>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class GroupingDialogForm; }

namespace te
{
  namespace map { class Grouping; }

  namespace rst { class Raster; }

  namespace se { class RasterSymbolizer; }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class ColorMapWidget;
      class GroupingWidget;

      /*!
        \class GroupingDialog

        \brief A dialog used to build a grouping.
              
      */
      class TEQTWIDGETSEXPORT GroupingDialog : public QDialog
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a basic fill dialog which is a child of parent, with widget flags set to f. */
          GroupingDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~GroupingDialog();

          //@}

        public:

          void setLayers(te::map::AbstractLayerPtr selectedLayer, std::vector<te::map::AbstractLayerPtr> allLayers);

        protected:

          void buildVectorialGroupingInteface();

          void buildRasterGroupingInteface();

        protected slots:

          void onPushButtonClicked();

          void onApplyClicked();

        private:

          std::auto_ptr<Ui::GroupingDialogForm> m_ui;        //!< Dialog form.
          te::qt::widgets::GroupingWidget* m_groupingWidget; //!< Grouping Widget used to configure the grouping operation.
          te::qt::widgets::ColorMapWidget* m_colorMapWidget; //!< Color Map Widget used to configure the grouping operation over a raster.

          te::map::AbstractLayerPtr m_layer;                  //!< Layer ptr.
          std::auto_ptr<te::rst::Raster> m_raster;
          te::se::RasterSymbolizer* m_rasterSymb;

          std::vector<te::map::AbstractLayerPtr> m_allLayers; //!< All Project Layers with Grouping.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_GROUPINGDIALOG_H
