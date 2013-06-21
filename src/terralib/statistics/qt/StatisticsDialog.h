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
  \file terralib/statistics/qt/StatisticsDialog.h

  \brief A dialog statistics
*/

#ifndef __TERRALIB_STATISTICAL_INTERNAL_STATISTICSDIALOG_H
#define __TERRALIB_STATISTICAL_INTERNAL_STATISTICSDIALOG_H

// TerraLib
#include "../../dataaccess/dataset/DataSet.h"
#include "../../maptools/AbstractLayer.h"
#include "../core/Config.h"
#include "../core/Enums.h"

// STL
#include <list>
#include <map>
#include <memory>

// Qt
#include <QtGui/QDialog>

namespace Ui { class StatisticsDialogForm; }

// Forward declarations
class QTableView;

namespace te
{
  namespace stat
  {
    class TESTATEXPORT StatisticsDialog : public QDialog
    {
      Q_OBJECT

      public:

        StatisticsDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~StatisticsDialog();

        void setStatistics(te::da::DataSet* dataSet, const std::string prop);

        std::size_t getPropertyIndex();

        std::vector<std::string> getStringData(const int propType);

        std::vector<double> getNumericData(const int proType);

      private:

      protected slots:

        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        typedef std::map<te::stat::StatisticalSummary, std::string> StaticalSummaryMap;

        std::auto_ptr<Ui::StatisticsDialogForm> m_ui;
        te::da::DataSet* m_dset;
        std::string m_prop;
    };
  }   // end namespace stat
}     // end namespace te

#endif  // __TERRALIB_STATISTICAL_INTERNAL_STATISTICSDIALOG_H