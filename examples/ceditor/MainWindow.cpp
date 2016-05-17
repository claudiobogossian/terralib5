
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
  QString file = QFileDialog::getOpenFileName(this, tr("Open Script File"), "", tr("Lua files (*.lua *.LUA);; Pyhton files (*.py *.PY)"), 0);

  if(!file.isEmpty())
  {
    QFileInfo info(file);

    QString scName = info.fileName();

    te::ce::ScriptWidget* editor = new te::ce::ScriptWidget(this);

    m_ui->m_codeTab->insertTab(0, editor, te::ce::CreateLangIcon(file), scName);

    editor->open(file);
  }
}

void MainWindow::newScript()
{

}

void MainWindow::save()
{
  QString selFilter;

  std::map<QString, QString> ext;

  QString luaFilter = tr("Lua files (*.lua *.LUA)"),
  pyFilter = tr("Pyhton files (*.py *.PY)");

  ext[luaFilter] = ".lua";
  ext[pyFilter] = ".py";

  QString file = QFileDialog::getSaveFileName(this, tr("Save Script File"), "", luaFilter + ";;" +pyFilter, &selFilter);

  if(!file.isEmpty())
  {
    QFileInfo info(file);

    if(info.suffix().isEmpty())
      file.append(ext[selFilter]);

    te::ce::ScriptWidget* editor = (te::ce::ScriptWidget*)m_ui->m_codeTab->currentWidget();

    editor->save(file);

    m_ui->m_codeTab->setTabText(m_ui->m_codeTab->currentIndex(), info.fileName());
  }
}

void MainWindow::run()
{

}
