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
\file src/terraMobilePlugin/qt/GeoPackagePublisherDialog.cpp

\brief This interface is used to get the input parameters for GeoPackage Publisher operation.
*/

// TerraLib
#include "../../../../common/progress/ProgressManager.h"
#include "../../../../common/progress/TaskProgress.h"
#include "../../../../common/STLUtils.h"
#include "GeoPackagePublisherDialog.h"
#include "ui_GeoPackagePublisherDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// libcurl
#include <curl/curl.h>

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

void getProjectList();

te::qt::plugins::terramobile::GeoPackagePublisherDialog::GeoPackagePublisherDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::GeoPackagePublisherDialogForm)
{
  // add controls
  m_ui->setupUi(this);

  //connects
  connect(m_ui->m_importSearchPushButton, SIGNAL(clicked()), this, SLOT(onImportSearchPushButtonClicked()));
  connect(m_ui->m_importPushButton, SIGNAL(clicked()), this, SLOT(onImportPushButtonClicked()));
  connect(m_ui->m_exportFilePushButton, SIGNAL(clicked()), this, SLOT(onExportFilePushButtonClicked()));
  connect(m_ui->m_exportPushButton, SIGNAL(clicked()), this, SLOT(onExportPushButtonClicked()));
}

te::qt::plugins::terramobile::GeoPackagePublisherDialog::~GeoPackagePublisherDialog()
{

}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onImportSearchPushButtonClicked()
{
  getProjectList();

  if (m_ui->m_importURLLineEdit->text().isEmpty())
  {
    m_ui->m_importTableWidget->setRowCount(0);

    QMessageBox::warning(this, tr("Warning"), tr("Server not defined."));
    return;
  }

  //check server

  //fill list
}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onImportPushButtonClicked()
{
  if (m_ui->m_importTableWidget->selectedItems().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Select at least one geopackage file to import."));
    return;
  }

  //import
}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onExportFilePushButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open GeoPackage File"), "", tr("GeoPackage File (*.gpkg *.GPKG)"));

  if (fileName.isEmpty())
    m_ui->m_exportFileLineEdit->clear();
  else
    m_ui->m_exportFileLineEdit->setText(fileName);
}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onExportPushButtonClicked()
{
  if (m_ui->m_exportFileLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Geopackage File not selected."));
    return;
  }

  if (m_ui->m_exportServerLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Server not defined."));
    return;
  }

  //check server

  //export gpkg
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

void getProjectList()
{
  CURL *curl;
  CURLcode res;

  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  struct curl_slist *headerlist = NULL;
  static const char buf[] = "Expect:";

  curl_global_init(CURL_GLOBAL_ALL);

  /* Fill in the file upload field */
  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "user",
    CURLFORM_COPYCONTENTS, "userName",
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "password",
    CURLFORM_COPYCONTENTS, "password",
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "projectStatus",
    CURLFORM_COPYCONTENTS, "1",
    CURLFORM_END);

  /* Fill in the submit field too, even if this is rarely needed */
  /*    curl_formadd(&formpost,
  &lastptr,
  CURLFORM_COPYNAME, "submit",
  CURLFORM_COPYCONTENTS, "send",
  CURLFORM_END);*/

  std::string readBuffer;

  curl = curl_easy_init();
  /* initialize custom header list (stating that Expect: 100-continue is not
  wanted */
  headerlist = curl_slist_append(headerlist, buf);
  if (curl) {
    /* what URL that receives this POST */
    curl_easy_setopt(curl, CURLOPT_URL, "http://terrabrasilis.info/TerraMobileServer/projectservices/listprojects");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
      curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* then cleanup the formpost chain */
    curl_formfree(formpost);
    /* free slist */
    curl_slist_free_all(headerlist);
  }

  std::stringstream ss(readBuffer);

  try
  {
    boost::property_tree::ptree pt;
    boost::property_tree::json_parser::read_json(ss, pt);

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("projects"))
    {
      std::string projectName = v.second.get<std::string>("project_name");
      std::string projectStatus = v.second.get<std::string>("project_status");
      std::string projectId = v.second.get<std::string>("project_id");
      std::string projectDesc = v.second.get<std::string>("project_description");

      int a = 0;
    }
  }
  catch (boost::property_tree::json_parser::json_parser_error &je)
  {
    return;
  }
  catch (std::exception const& e)
  {
    return;
  }

  return;
}
