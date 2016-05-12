#include "MainWindow.h"

#include <ui_CodeEditorForm.h>

// TerraLib
#include <terralib/qsci/ceditor/ScriptWidget.h>
#include <terralib/qsci/ceditor/Utils.h>

// Qt
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
  m_ui = new Ui::MainWindow;

  m_ui->setupUi(this);
}

void MainWindow::open()
{
  QString file = QFileDialog::getOpenFileName(this, tr("Open Script File"), "", tr("Lua file (*.lua *.LUA);; Pyhton file (*.py *.PY)"), 0);

  if(!file.isEmpty())
  {
    QFileInfo info(file);

    QString scName = info.fileName();

    te::ce::ScriptWidget* editor = new te::ce::ScriptWidget(this);

    m_ui->m_codeTab->insertTab(0, editor, te::ce::getIcon(file), scName);

    editor->open(file);
  }
}

void MainWindow::newScript()
{

}

void MainWindow::save()
{

}

void MainWindow::run()
{

}
