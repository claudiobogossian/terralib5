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
  \file terralib/edit/qt/FeatureAttributesDialog.h

  \brief A widget used to show and setup feature attributes.
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_FEATUREATTRIBUTESDIALOG_H
#define __TERRALIB_EDIT_QT_INTERNAL_FEATUREATTRIBUTESDIALOG_H

// TerraLib
#include "Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>

// Forward declarations
class QTreeWidgetItem;
namespace Ui { class FeatureAttributesDialogForm; }

namespace te
{
// Forward declaration
  namespace da
  {
    class DataSetType;
  }

  namespace edit
  {
// Forward declaration
    class Feature;

    /*!
      \class FeatureAttributesDialog

      \brief A widget used to show and setup feature attributes.
    */
    class TEEDITQTEXPORT FeatureAttributesDialog : public QDialog
    {
      Q_OBJECT

      public:

        /** @name Initializer Methods
          *  Methods related to instantiation and destruction.
          */
        //@{

        /*! \brief Constructs the feature attributes dialog which is a child of parent, with widget flags set to f. */
        FeatureAttributesDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        /*! \brief Destructor. */
        ~FeatureAttributesDialog();

        //@}

        void set(te::da::DataSetType* type, Feature* f);

      private:

        /*! \brief Internal method to initialize the dialog. */
        void initialize();

      private slots:

        void onOkPushButtonPressed();

        void onAttributesTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column);

      private:

        std::auto_ptr<Ui::FeatureAttributesDialogForm> m_ui;  //!< Dialog form.
        te::da::DataSetType* m_type;                          //!< The attributes dataset type.
        Feature* m_feature;                                   //!< The owner feature of the attributes.

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_FEATUREATTRIBUTESDIALOG_H
