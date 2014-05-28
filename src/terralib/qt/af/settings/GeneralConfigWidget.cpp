#include "ui_GeneralConfigWidgetForm.h"
#include "GeneralConfigWidget.h"

// TerraLib
#include "terralib/common/SystemApplicationSettings.h"
#include "terralib/common/UserApplicationSettings.h"
#include "../ApplicationController.h"
#include "../Utils.h"

// Qt
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtGui/QInputDialog>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QPushButton>

// Boost
#include <boost/foreach.hpp>

void UpdateImage(const QString& imgFile, QPixmap*& pix, const int& size)
{
  pix = new QPixmap(imgFile);

  if(pix->isNull())
    throw;

  if(size != 0)
    *pix = pix->scaled(QSize(size, size));
}

void GetIconsNames(QString& sysIcon, QString& aboutIcon, QString& terraIcon)
{
  te::common::SystemApplicationSettings& appConf = te::common::SystemApplicationSettings::getInstance();

  sysIcon = appConf.getValue("Application.IconName").c_str();
  aboutIcon = appConf.getValue("Application.AboutDialogLogo.<xmlattr>.xlink:href").c_str();
  terraIcon = appConf.getValue("Application.TerraLibLogo.<xmlattr>.xlink:href").c_str();
}

void DrawPixmap(QFrame* frame, QPixmap* pix)
{
  QPainter p(frame);
  int ww = frame->width(),
      wh = frame->height(),
      pw = pix->width(),
      ph = pix->height();
        
  p.drawPixmap(ww/2 - pw/2, wh/2 - ph/2, *pix);
}

QPixmap* GetResizedImage(const QString& name, const int& size)
{
  QPixmap* res = new QPixmap;

  *res = QIcon::fromTheme(name).pixmap(size, size);

  return res;
}

void GetPluginsCategories(QComboBox* cbb)
{
  boost::property_tree::ptree& p = te::common::SystemApplicationSettings::getInstance().getAllSettings().get_child("Application");

  if(!p.empty())
    BOOST_FOREACH(boost::property_tree::ptree::value_type& v, p.get_child("DefaultPluginsCategory"))
    {
      if(v.second.data().empty())
        continue;

      cbb->addItem(v.second.data().c_str());
    }
}

void FillSRSList(QComboBox* list)
{
  //std::pair<te::srs::SpatialReferenceSystemManager::iterator,
  //          te::srs::SpatialReferenceSystemManager::iterator> its = te::srs::SpatialReferenceSystemManager::getInstance().getIterators();
  
  list->clear();
  list->addItem("0");

  //while (its.first != its.second) 
  //{
  //  list->addItem(QString::number(its.first->m_auth_id));
    //if (its.first->m_auth_name == "EPSG")
    //{
    //  if (its.first->m_auth_id < 5000)
    //  {
    //    QTreeWidgetItem *geog = new QTreeWidgetItem(items[0]);
    //    geog->setText(0, its.first->m_name.c_str());
    //    geog->setText(1, QString("%1").arg(its.first->m_auth_id));
    //    geog->setText(2, "EPSG");
    //  }
    //  else
    //  {
    //    QTreeWidgetItem *proj = new QTreeWidgetItem(items[1]);
    //    proj->setText(0, its.first->m_name.c_str());
    //    proj->setText(1, QString("%1").arg(its.first->m_auth_id));
    //    proj->setText(2, "EPSG");
    //  }
    //}
    //else
    //{
    //  QTreeWidgetItem *userd = new QTreeWidgetItem(items[2]);
    //  userd->setText(0, its.first->m_name.c_str());
    //  userd->setText(1, QString("%1").arg(its.first->m_auth_id));
    //  userd->setText(2, its.first->m_auth_name.c_str());
    //}
  //  ++its.first;
  //}
}


te::qt::af::GeneralConfigWidget::GeneralConfigWidget(QWidget* parent) :
AbstractSettingWidget(parent),
m_ui(new Ui::GeneralConfigWidgetForm),
m_needRestart(false),
m_sysIcon(0),
m_sizeIcon(0),
m_aboutIcon(0),
m_terraIcon(0)
{
  m_ui->setupUi(this);

  m_resumeText = tr("Changes the general configurations of the application.");

  m_ui->m_aboutLogoFileLineEdit->setLabel(tr("About logo file: "));
  m_ui->m_helpFileLineEdit->setLabel(tr("Help file: "));
  m_ui->m_iconThemeFolderLineEdit->setLabel(tr("Icon theme folder: "));
  m_ui->m_pluginsFileLineEdit->setLabel(tr("Plugins file: "));
  m_ui->m_sysIconNameLineEdit->setLabel(tr("System icon file: "));
  m_ui->m_terralibLogoFileLineEdit->setLabel(tr("Terralib logo file: "));
  m_ui->m_userSettingsFileLineEdit->setLabel(tr("User settings file: "));
  m_ui->m_dsLocationFileFileChooser->setLabel(tr("Data sources file: "));

  m_ui->m_addPluginCategoryToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removePluginCategoryToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_iconThemeFolderLineEdit->setResourceType(te::qt::widgets::FileChooser::FOLDER);

  resetState();

  // Setting image files filters
  QString imgFilters("Image files(*.png *.PNG *.bmp *.BMP *.svg *.SVG)");

  m_ui->m_sysIconNameLineEdit->setFilterPattern(imgFilters);
  m_ui->m_aboutLogoFileLineEdit->setFilterPattern(imgFilters);
  m_ui->m_terralibLogoFileLineEdit->setFilterPattern(imgFilters);

  // Setting xml files filters
  QString xmlFilters("XML files(*.xml *.XML)");

  m_ui->m_pluginsFileLineEdit->setFilterPattern(xmlFilters);
  m_ui->m_userSettingsFileLineEdit->setFilterPattern(xmlFilters);
  m_ui->m_dsLocationFileFileChooser->setFilterPattern(xmlFilters);

  // Setting help files filter
  m_ui->m_helpFileLineEdit->setFilterPattern(tr("Help files(*.qhc *.QHC)"));

  connect(m_ui->m_iconSizeSpinBox, SIGNAL(valueChanged(int)), SLOT(iconSizeChanged(int)));
  connect(m_ui->m_toolbarIconSizeSpinBox, SIGNAL(valueChanged(int)), SLOT(iconSizeChanged(int)));
  connect(m_ui->m_addPluginCategoryToolButton, SIGNAL(pressed()), SLOT(addPluginCategory()));
  connect(m_ui->m_removePluginCategoryToolButton, SIGNAL(pressed()), SLOT(removePluginCategory()));

  // Images that changed
  connect(m_ui->m_sysIconNameLineEdit, SIGNAL(resourceSelected(QString)), SLOT(valueChanged(QString)));
  connect(m_ui->m_aboutLogoFileLineEdit, SIGNAL(resourceSelected(QString)), SLOT(valueChanged(QString)));
  connect(m_ui->m_terralibLogoFileLineEdit, SIGNAL(resourceSelected(QString)), SLOT(valueChanged(QString)));
  connect(m_ui->m_iconThemeFolderLineEdit, SIGNAL(resourceSelected(QString)), SLOT(valueChanged(QString)));

  // Line edit texts changed
  connect(m_ui->m_organizationLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
  connect(m_ui->m_sysNameLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
  connect(m_ui->m_sysTitleLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
  connect(m_ui->m_iconThemeNameLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
  connect(m_ui->m_helpFileLineEdit, SIGNAL(resourceSelected(QString)), SLOT(valueChanged(QString)));
  connect(m_ui->m_userSettingsFileLineEdit, SIGNAL(resourceSelected(QString)), SLOT(valueChanged(QString)));
  connect(m_ui->m_pluginsFileLineEdit, SIGNAL(resourceSelected(QString)), SLOT(valueChanged(QString)));
  connect(m_ui->m_dsLocationFileFileChooser, SIGNAL(resourceSelected(QString)), SLOT(valueChanged(QString)));

  connect(m_ui->m_selectionColorButton, SIGNAL(colorChanged(const QColor&)), SLOT(colorChanged(const QColor&)));
  connect(m_ui->m_selectionColorToolButton, SIGNAL(colorChanged(const QColor&)), SLOT(colorChanged(const QColor&)));

  m_ui->m_sysIconFrame->installEventFilter(this);
  m_ui->m_iconSizeFrame->installEventFilter(this);
  m_ui->m_terralibLogoFrame->installEventFilter(this);
  m_ui->m_aboutLogoFrame->installEventFilter(this);
  m_ui->m_toolbarsIconSizeFrame->installEventFilter(this);

  m_ui->m_tabWidget->setCurrentIndex(0);
}

te::qt::af::GeneralConfigWidget::~GeneralConfigWidget()
{
  if(m_needRestart)
  {
    QMessageBox msgBox(this);

    msgBox.setText(tr("The system must be restarted for the changes to take effect."));
    msgBox.setInformativeText(tr("Do you want to do it now?"));
    msgBox.setWindowTitle(tr("Restart system"));

    std::auto_ptr<QPushButton> resButton;
    std::auto_ptr<QPushButton> laterResButton;

    laterResButton.reset(msgBox.addButton(tr("Restart later"), QMessageBox::DestructiveRole));
    msgBox.addButton(QMessageBox::NoButton);
    resButton.reset(msgBox.addButton(tr("Restart now"), QMessageBox::ActionRole));

    msgBox.setDefaultButton(resButton.get());

    msgBox.exec();

    if(msgBox.clickedButton() == resButton.get())
      qApp->exit(1000);
  }
}

void te::qt::af::GeneralConfigWidget::saveChanges()
{
  bool orgChanged = false,
    sysNameChanged = false;

  te::common::SystemApplicationSettings& appSet = te::common::SystemApplicationSettings::getInstance();

  std::string org = m_ui->m_organizationLineEdit->text().toStdString(),
    sysName = m_ui->m_sysNameLineEdit->text().toStdString(),
    userFile = m_ui->m_userSettingsFileLineEdit->getSelectedResource().toStdString(),
    plgFile = m_ui->m_pluginsFileLineEdit->getSelectedResource().toStdString(),
    currPlgFile = appSet.getValue("Application.PluginsFile.<xmlattr>.xlink:href"),
    currUserFile = appSet.getValue("Application.UserSettingsFile.<xmlattr>.xlink:href");
  
  bool plgFileChanged = plgFile != currPlgFile,
       usrFileChanged = userFile != currUserFile;

  if(usrFileChanged)
    UpdateUserSettingsFile(userFile.c_str());

  if(plgFileChanged)
    UpdateAppPluginsFile(plgFile.c_str());

  if(appSet.getValue("Application.Organization") != org)
    orgChanged = true;
  if(appSet.getValue("Application.Name") != sysName)
    sysNameChanged = true;

  appSet.setValue("Application.Organization", org);
  appSet.setValue("Application.Name", sysName);
  appSet.setValue("Application.Title", m_ui->m_sysTitleLineEdit->text().toStdString());
  appSet.setValue("Application.HelpFile.<xmlattr>.xlink:href", m_ui->m_helpFileLineEdit->getSelectedResource().toStdString());
  appSet.setValue("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href", m_ui->m_iconThemeFolderLineEdit->getSelectedResource().toStdString());
  appSet.setValue("Application.IconThemeInfo.DefaultTheme", m_ui->m_iconThemeNameLineEdit->text().toStdString());
  appSet.setValue("Application.PluginsFile.<xmlattr>.xlink:href", plgFile);
  appSet.setValue("Application.IconName", m_ui->m_sysIconNameLineEdit->getSelectedResource().toStdString());
  appSet.setValue("Application.AboutDialogLogo.<xmlattr>.xlink:href", m_ui->m_aboutLogoFileLineEdit->getSelectedResource().toStdString());
  appSet.setValue("Application.TerraLibLogo.<xmlattr>.xlink:href", m_ui->m_terralibLogoFileLineEdit->getSelectedResource().toStdString());
  appSet.setValue("Application.UserSettingsFile.<xmlattr>.xlink:href", userFile);
  appSet.setValue("Application.ToolBarDefaultIconSize", m_ui->m_iconSizeSpinBox->text().toStdString());
  appSet.setValue("Application.DefaultSelectionColor", m_ui->m_selectionColorButton->getColor().name().toStdString());
  appSet.setValue("Application.DefaultSRID", m_ui->m_srsComboBox->currentText().toStdString());

  appSet.getAllSettings().get_child("Application.DefaultPluginsCategory").clear();

  for(int i=0; i<m_ui->m_pluginsCategoriesComboBox->count(); i++)
    appSet.getAllSettings().add("Application.DefaultPluginsCategory.Category", m_ui->m_pluginsCategoriesComboBox->itemText(i).toStdString());

  //Updating user settings file
  te::common::UserApplicationSettings& usrSett = te::common::UserApplicationSettings::getInstance();

  usrSett.setValue("UserSettings.SelectedIconTheme", m_ui->m_selIconThemeLineEdit->text().toStdString());
  usrSett.setValue("UserSettings.ToolBarIconSize", m_ui->m_toolbarIconSizeSpinBox->text().toStdString());
  usrSett.setValue("UserSettings.DefaultSRID", m_ui->m_DefaultSridComboBox->currentText().toStdString());
  usrSett.setValue("UserSettings.SelectionColor", m_ui->m_selectionColorToolButton->getColor().name().toStdString());
  usrSett.setValue("UserSettings.DataSourcesFile", m_ui->m_dsLocationFileFileChooser->getSelectedResource().toStdString());

  m_needRestart = true;
}

void te::qt::af::GeneralConfigWidget::resetState()
{
  QString iconName,
          aboutLogo,
          terraLogo;
  GetIconsNames(iconName, aboutLogo, terraLogo);

  te::common::SystemApplicationSettings& appConf = te::common::SystemApplicationSettings::getInstance();
  te::common::UserApplicationSettings& usrConf = te::common::UserApplicationSettings::getInstance();

  m_ui->m_organizationLineEdit->setText(appConf.getValue("Application.Organization").c_str());
  m_ui->m_sysNameLineEdit->setText(appConf.getValue("Application.Name").c_str());
  m_ui->m_sysTitleLineEdit->setText(appConf.getValue("Application.Title").c_str());
  m_ui->m_helpFileLineEdit->setInitialPath(appConf.getValue("Application.HelpFile.<xmlattr>.xlink:href").c_str());
  m_ui->m_iconThemeFolderLineEdit->setInitialPath(appConf.getValue("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href").c_str());
  m_ui->m_iconThemeNameLineEdit->setText(appConf.getValue("Application.IconThemeInfo.DefaultTheme").c_str());
  m_ui->m_sysIconNameLineEdit->setInitialPath(iconName);
  m_ui->m_pluginsFileLineEdit->setInitialPath(appConf.getValue("Application.PluginsFile.<xmlattr>.xlink:href").c_str());
  m_ui->m_aboutLogoFileLineEdit->setInitialPath(aboutLogo);
  m_ui->m_terralibLogoFileLineEdit->setInitialPath(terraLogo);
  m_ui->m_userSettingsFileLineEdit->setInitialPath(appConf.getValue("Application.UserSettingsFile.<xmlattr>.xlink:href").c_str());
  m_ui->m_iconSizeSpinBox->setValue(QString(appConf.getValue("Application.ToolBarDefaultIconSize").c_str()).toInt());
  m_ui->m_selectionColorButton->setColor(QColor(appConf.getValue("Application.DefaultSelectionColor").c_str()));

  GetPluginsCategories(m_ui->m_pluginsCategoriesComboBox);

  FillSRSList(m_ui->m_srsComboBox);

  int curr = m_ui->m_srsComboBox->findText(appConf.getValue("Application.DefaultSRID").c_str());
  m_ui->m_srsComboBox->setCurrentIndex(curr);

  // Values for user settings tab
  m_ui->m_dsLocationFileFileChooser->setInitialPath(usrConf.getValue("UserSettings.DataSourcesFile").c_str());
  m_ui->m_selIconThemeLineEdit->setText(usrConf.getValue("UserSettings.SelectedIconTheme").c_str());
  m_ui->m_toolbarIconSizeSpinBox->setValue(QString(usrConf.getValue("UserSettings.ToolBarIconSize").c_str()).toInt());
  m_ui->m_selectionColorToolButton->setColor(QColor(usrConf.getValue("UserSettings.SelectionColor").c_str()));

  QPixmap* sysIcon = 0;
  QPixmap* aboutIcon = 0;
  QPixmap* terraIcon = 0;

  m_sizeIcon.reset(GetResizedImage("zoom-in", m_ui->m_iconSizeSpinBox->value()));
  m_toolBarSizeIcon.reset(GetResizedImage("zoom-in", m_ui->m_toolbarIconSizeSpinBox->value()));

  UpdateImage(iconName, sysIcon, 0);
  UpdateImage(aboutLogo, aboutIcon, 0);
  UpdateImage(terraLogo, terraIcon, 0);

  m_sysIcon.reset(sysIcon);
  m_aboutIcon.reset(aboutIcon);
  m_terraIcon.reset(terraIcon);
}
          
void te::qt::af::GeneralConfigWidget::getHelpInformations(QString& ns, QString& helpFile)
{
  ns = "dpi.inpe.br.apf";
  helpFile = "apf/settings/generalconfig/GeneralConfig.html";
}

bool te::qt::af::GeneralConfigWidget::eventFilter(QObject* watched, QEvent* e)
{
  switch (e->type())
  {
    case QEvent::Paint:
      if(watched == m_ui->m_sysIconFrame)
        DrawPixmap(m_ui->m_sysIconFrame, m_sysIcon.get());
      else if(watched == m_ui->m_aboutLogoFrame)
        DrawPixmap(m_ui->m_aboutLogoFrame, m_aboutIcon.get());
      else if(watched == m_ui->m_iconSizeFrame)
        DrawPixmap(m_ui->m_iconSizeFrame, m_sizeIcon.get());
      else if(watched == m_ui->m_terralibLogoFrame)
        DrawPixmap(m_ui->m_terralibLogoFrame, m_terraIcon.get());
      else if(watched == m_ui->m_toolbarsIconSizeFrame)
        DrawPixmap(m_ui->m_toolbarsIconSizeFrame, m_toolBarSizeIcon.get());

      return true;
    break;

    default:
      return watched->event(e);
    break;
  }
}

void te::qt::af::GeneralConfigWidget::valueChanged(QString s)
{
  QObject* send = sender();

  if(send == m_ui->m_sysIconNameLineEdit)
  {
    QPixmap* img = 0;
    UpdateImage(m_ui->m_sysIconNameLineEdit->getSelectedResource(), img, 0);
    m_sysIcon.reset(img);
    m_ui->m_sysIconFrame->repaint();
  }
  else if(send == m_ui->m_aboutLogoFileLineEdit)
  {
    QPixmap* img = 0;
    UpdateImage(m_ui->m_aboutLogoFileLineEdit->getSelectedResource(), img, 0);
    m_aboutIcon.reset(img);
    m_ui->m_aboutLogoFrame->repaint();
  }
  else if(send == m_ui->m_terralibLogoFileLineEdit)
  {
    QPixmap* img = 0;
    UpdateImage(m_ui->m_terralibLogoFileLineEdit->getSelectedResource(), img, 0);
    m_terraIcon.reset(img);
    m_ui->m_terralibLogoFrame->repaint();
  }

  changeApplyButtonState(true);
}

void te::qt::af::GeneralConfigWidget::iconSizeChanged(int size)
{
  QObject* send = sender();
  QPixmap* pix = GetResizedImage("zoom-in", size);

  if(send == m_ui->m_iconSizeSpinBox)
  {
    m_sizeIcon.reset(pix);
    m_ui->m_iconSizeFrame->repaint();
  }
  else
  {
    m_toolBarSizeIcon.reset(pix);
    m_ui->m_toolbarsIconSizeFrame->repaint();
  }

  changeApplyButtonState(true);
}

void te::qt::af::GeneralConfigWidget::addPluginCategory()
{
  bool ok;

  QString text = QInputDialog::getText(this, tr("Enter new plugin category"), tr("Plugin category:"), QLineEdit::Normal, "", &ok);

  if (ok && !text.isEmpty())
  {
    m_ui->m_pluginsCategoriesComboBox->addItem(text);
    m_ui->m_pluginsCategoriesComboBox->setCurrentIndex(m_ui->m_pluginsCategoriesComboBox->count()-1);

    changeApplyButtonState(true);
  }
}

void te::qt::af::GeneralConfigWidget::removePluginCategory()
{
  m_ui->m_pluginsCategoriesComboBox->removeItem(m_ui->m_pluginsCategoriesComboBox->currentIndex());

  changeApplyButtonState(true);
}

void te::qt::af::GeneralConfigWidget::textChanged(const QString&)
{
  changeApplyButtonState(true);
}

void te::qt::af::GeneralConfigWidget::colorChanged(const QColor& c)
{
  ApplicationController::getInstance().setSelectionColor(c);
  changeApplyButtonState(true);
}
