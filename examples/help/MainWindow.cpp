#include "MainWindow.h"
#include <ui_mainwindow.h>

#include "HelpTest.h"
#include "HelpTest2.h"
#include "Plugin.h"

//TerraLib help files
#include <terralib/qt/widgets/help/HelpManager.h>
#include <terralib/qt/widgets/help/AssistantHelpManagerImpl.h>


MainWindow::MainWindow(QWidget* parent) :
QMainWindow(parent),
  m_ui(new Ui::MainWindow)
{
  m_impl = new te::qt::widgets::AssistantHelpManagerImpl("help.qhc", this);
  te::qt::widgets::HelpManager::getInstance().setImpl(m_impl);

  m_ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete m_ui;
}

void MainWindow::showTest1()
{
  HelpTest* dlg = new HelpTest(this);

  dlg->show();
}

void MainWindow::showTest2()
{
  HelpTest2* dlg = new HelpTest2(this);

  dlg->show();
}

void MainWindow::pluginTest()
{
  Plugin* dlg = new Plugin(this);

  dlg->show();
}
