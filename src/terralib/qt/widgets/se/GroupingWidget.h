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
  \file terralib/qt/widgets/se/GroupingWidget.h

  \brief A widget used to build a grouping.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_GROUPINGWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_GROUPINGWIDGET_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// QT
#include <QTableWidgetItem>
#include <QWidget>

// Forward declaraion
namespace Ui { class GroupingWidgetForm; }

namespace te
{
// Forward declarations
  namespace color { class ColorBar; }

  namespace da { class DataSetType; }

  namespace map 
  { 
    class Grouping;
    class GroupingItem;
  }

  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class ColorCatalogWidget;

      /*!
        \class GroupingWidget

        \brief A widget used to build a grouping.

        \sa 
      */
      class TEQTWIDGETSEXPORT GroupingWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a basic fill widget which is a child of parent, with widget flags set to f. */
          GroupingWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~GroupingWidget();

          //@}

        public:

          void setLayers(te::map::AbstractLayerPtr selectedLayer, std::vector<te::map::AbstractLayerPtr> allLayers);

          std::auto_ptr<te::map::Grouping> getGrouping();

        protected:

          /*! \brief Internal method to initialize the widget (e.g.: color, combos, icons, etc.) */
          void initialize();

          /*! \brief Updates the widget form based on internal fill element. */
          void updateUi(bool loadColorBar = false);

          void setDataSetType();

          void setGrouping();

          void setGrouping(te::map::Grouping* grouping);

          void getDataAsDouble(std::vector<double>& vec, const std::string& attrName, const int& dataType, int& nullValues);

          void getLinkedDataAsDouble(std::vector<double>& vec, const std::string& attrName, const int& dataType, int& nullValues);

          void getDataAsString(std::vector<std::string>& vec, const std::string& attrName, int& nullValues);

          void getLinkedDataAsString(std::vector<std::string>& vec, const std::string& attrName, int& nullValues);

          void createDoubleNullGroupingItem(int count);

          void createStringNullGroupingItem(int count);

          int getGeometryType();

          void buildSymbolizer(std::string meanTitle = "");

          void listAttributes();

        public slots:

          void onApplyPushButtonClicked();

          void onTypeComboBoxActivated(int idx);

          void onAttrComboBoxActivated(int idx);

          void onColorBarChanged();

          void onTableWidgetItemChanged(QTableWidgetItem* item);

          void onTableWidgetItemDoubleClicked(QTableWidgetItem* item);

          void onImportPushButtonClicked();

        signals:

          void applyPushButtonClicked();


        private:

          std::auto_ptr<Ui::GroupingWidgetForm> m_ui;               //!< Widget form.

          te::color::ColorBar* m_cb;                                //!< Terralib color bar objetc
          te::map::AbstractLayerPtr m_layer;                        //!< TerraLib layer auto ptr
          te::qt::widgets::ColorCatalogWidget* m_colorBar;         //!< Widget used to pick a color.
          std::vector<te::map::GroupingItem*> m_legend;             //!< Grouping items

          bool m_manual;
          
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_GROUPINGWIDGET_H
