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
  \file terralib/sa/qt/BayesGlobalDialog.h

  \brief A dialog to calculate the global empirical bayes of a dataset.
*/

#ifndef __TERRALIB_MNT_INTERNAL_PROFILEDIALOG_H
#define __TERRALIB_MNT_INTERNAL_PROFILEDIALOG_H

// TerraLib
#include "../../color/RGBAColor.h"
#ifndef Q_MOC_RUN
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#endif
#include "../core/Config.h"
#include "../core/Enums.h"

#include "../../qt/af/BaseApplication.h"

#include "ProfileTools.h"

// Qt
#include <QDialog>

namespace Ui { class ProfileDialogForm; }

// Forward declarations

namespace te
{
  namespace mnt
  {
// Forward declarations

    class TEMNTEXPORT ProfileDialog : public QDialog
    {
      Q_OBJECT

      public:

        ProfileDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~ProfileDialog();

      //Q_SIGNALS:
      //  void geometriesEdited();

      protected slots:
      
      void onInputComboBoxChanged(int index);

      void oneditionEnabled(bool);
      void onselectionEnabled(bool);

      void onVectorInputComboBoxChanged(int index);

      void onselectLineToggled(bool checked);
      void onaddPointMouseToggled(bool checked);
      void onaddPointKeyToggled(bool checked);
      void onchangePointToggled(bool checked);
      void onaddPointToggled(bool checked);
      void ondeletePointToggled(bool checked);
      void ondeletePathToggled(bool checked);
      void oninvertToggled(bool checked);

      void onGeometriesChanged();

      void onOkPushButtonClicked();   

      void DrawSelected(const std::vector<te::gm::LineString*> visadas, const std::vector<te::color::RGBAColor>color);

 /*   protected:
      void setCurrentTool(te::edit::GeometriesUpdateTool* tool, te::qt::af::MapDisplay* display);
*/
    public:
      te::map::AbstractLayerPtr getLayer();
      void setLayers(std::list<te::map::AbstractLayerPtr> layers);

    private:
      std::auto_ptr<Ui::ProfileDialogForm> m_ui;
      
      te::map::AbstractLayerPtr m_outputLayer;

      te::map::AbstractLayerPtr m_inputLayer;
      std::list<te::map::AbstractLayerPtr> m_layers;
      mntType m_inputType;  //!< Input type (TIN, GRID)
      int m_srid;
      double m_dummy;
      std::auto_ptr<te::da::DataSetType> m_dsType;
      te::map::AbstractLayerPtr m_rasterinputLayer;                                
      te::map::AbstractLayerPtr m_vectorinputLayer;

      //ProfileTools* m_tool;
      te::qt::widgets::AbstractTool *m_tool;
      te::qt::af::BaseApplication* m_app;

      //QAction* m_selectLineToolAction;
      //QAction* m_addPointMouseToolAction;
      //QAction* m_addPointKeyToolAction;
      //QAction* m_changePointToolAction;
      //QAction* m_addPointToolAction;
      //QAction* m_deletePointToolAction;
      //QAction* m_deletePathToolAction;
      //QAction* m_invertToolAction;

      //te::edit::GeometriesUpdateTool* m_currentTool;

      std::vector<te::gm::LineString*> m_visadas;

    };
  }   // end namespace mnt
}     // end namespace te

#endif  // __TERRALIB_MNT_INTERNAL_PROFILEDIALOG_H
