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
\file ProjectInfoDialog.h

\brief A dialog for edition of Project informations.
*/

#ifndef __TERRAVIEW_INTERNAL_PROJECTINFODIALOG_H
#define __TERRAVIEW_INTERNAL_PROJECTINFODIALOG_H

#include <QDialog>

// Forward declaration
struct ProjectMetadata;
class ProjectInfoWidget;

/*!
	\class

	\brief
*/
class ProjectInfoDialog : public QDialog
{
	Q_OBJECT

	public:

		/*!
			\brief Constructor.

			\param parent QWidget parent.
		*/
		ProjectInfoDialog(QWidget* parent=0);

		/*!
			\brief Destructor.
		*/
		~ProjectInfoDialog();

		/*!
			\brief Sets the project to be inspected.

			\param proj The te::qt::af::Project to be presented.
		*/
    void setProject(ProjectMetadata* proj);

	protected slots:

		/*!
			\brief Called on ok button pressed.
		*/
		void onOkPushButtonPressed();

		/*!
			\brief Called on help button pressed.
		*/
		void onHelpPushButtonPressed();

	protected:

		ProjectInfoWidget* m_info;    //!< Pointer to the widget that contains information about te::qt::af::Project.
};


#endif //__TERRALIB_QT_AF_INTERNAL_PROJECTINFODIALOG_H
