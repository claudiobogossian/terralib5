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
  \file Legend.h

  \brief It provides an implementation for generating a legend for a layer.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_LEGEND_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_LEGEND_H

// TerraLib
#include "../Config.h"
#include "terralib/qt/widgets/colorbar/ColorBar.h"
#include "terralib/color/ColorBar.h"
#include "../../../color/RGBAColor.h"

// Qt
#include <QtGui/QDialog>
#include "ui_Legend.h"


namespace te
{
  namespace da
  {
    class DataSource;
    class DataSourceTransactor;
    class DataSetType;
  }

  namespace color { class ColorBar; }
  namespace map { class LegendItem; }

  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class LayerItem;

      class TEQTWIDGETSEXPORT Legend : public QDialog, public Ui_LegendDialog
      {
        Q_OBJECT

        public:
          Legend(te::qt::widgets::LayerItem* layerItem, QWidget* parent = 0);

          const std::vector<te::map::LegendItem*>& getLegend() const;

          size_t getNumberOfValuesNotGrouped() const;

        protected:
          void closeEvent(QCloseEvent* e);

        private slots:
          void typeComboBoxActivated(int index);
          void applyPushButtonClicked();
          void legendTableItemChanged(QTableWidgetItem*);
          void okPushButtonClicked();
          void cancelPushButtonClicked();
          void helpPushButtonClicked();

        private:
          void setTableContents();
         
        private:
          te::qt::widgets::LayerItem* m_layerItem;

          te::da::DataSourceTransactor* m_t;
          te::da::DataSetType* m_dataSetType;

          std::vector<te::map::LegendItem*> m_legend;
          te::color::ColorBar* m_colorBar;
          std::vector<te::color::RGBAColor> m_legendColors;
          size_t m_numValuesNotGrouped;

          std::map<int, std::string> m_changedItemLabel;
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // endnamespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_LEGEND_H
