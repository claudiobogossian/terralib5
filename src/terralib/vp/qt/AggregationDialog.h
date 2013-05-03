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
  \file terralib/vp/qt/AggregationDialog.h

  \brief A dialog intersection operation
*/

#ifndef __TERRALIB_VP_INTERNAL_AGGREGATIONDIALOG_H
#define __TERRALIB_VP_INTERNAL_AGGREGATIONDIALOG_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../core/Config.h"

// STL
#include <list>
#include <map>
#include <memory>

// Qt
#include <QtGui/QDialog>

namespace Ui { class AggregationDialogForm; }

// Forward declarations
class QModelIndex;
class QListWidgetItem;
class QTreeWidgetItem;

namespace te
{
  namespace vp
  {
// Forward declarations
    class LayerTreeModel;
    
    /*!
      \enum MemoryUse

      \brief Define possible states for memory use.
    */
    enum
    {
      WHOLE_MEM,      //!< Whole memory use.
      PARTIALLY_MEM,  //!< Partially memory use.
      LOW_MEM         //!< Low memory use.
    };

    /*!
      \enum Attributes

      \brief Define attributes for output layer.
    */
    enum Attributes
    {
      MIN_VALUE,          //!< Minimum value.
      MAX_VALUE,          //!< Maximum value.
      MEAN,               //!< Mean.
      SUM,                //!< Sum of values.
      COUNT,              //!< Total number of values.
      VALID_COUNT,        //!< Total not null values.
      STANDARD_DEVIATION, //!< Standard deviation.
      KERNEL,             //!< Kernel.
      VARIANCE,           //!< Variance.
      SKEWNESS,           //!< Skewness.
      KURTOSIS,           //!< Kurtosis.
      AMPLITUDE,          //!< Amplitude.
      MEDIAN,             //!< Median.
      VAR_COEFF,          //!< Coefficient variation.
      MODE                //!< Mode.
    };

    class TEVPEXPORT AggregationDialog : public QDialog
    {
      Q_OBJECT

      public:

        AggregationDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~AggregationDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        /*!
          \brief Set layer filtered

          \param layers Layers filtered.
        */
        void setFilteredLayers(std::list<te::map::AbstractLayerPtr> layers);

        /*!
          \brief Get the type of memory use based on a enum.

          \return MemoryUse enum.
        */
        int getMemoryUse();

        /*!
          \brief Set attributes for combobox 'm_selectAllComboBox' and 'm_rejectAllComboBox' based on a enum.
        */
        void setAttributes();

        /*!
          \brief Map Attributes enum for an intuitive name.
        */
        void setAttributesNameMap();

      private:

      protected slots:

        void onLayerTreeViewClicked(QTreeWidgetItem * item, int column);

        void onFilterLineEditTextChanged(const QString& text);
        
        void onTreeViewClicked(const QModelIndex& index);

        void onSelectAllComboBoxChanged(int index);

        void onRejectAllComboBoxChanged(int index);

        void onOutputListWidgetClicked(QListWidgetItem * item);

        void onCancelPushButtonClicked();

        void onHelpPushButtonClicked();

      private:

        std::auto_ptr<Ui::AggregationDialogForm> m_ui;
        std::list<te::map::AbstractLayerPtr> m_layers;          //!< List of layers.
        LayerTreeModel* m_model;                                //!< Layer Tree Model.
        std::map<Attributes, std::string> m_attributeNameMap;   //!< Maping of Attributes enum
    };
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_AGGREGATIONDIALOG_H