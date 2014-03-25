
#include "mainw.hh"
#include "ui_mainw.h"

#include "explorew.hh"
#include "searchdialog.hh"

#include <QFileDialog>
#include <QTextStream>
#include <stdio.h>

MainW::MainW(QWidget *parent)
    : QMainWindow(parent)
    , ui( new Ui::MainW )
    , search_dialog( new SearchDialog(this) )
{
  ui->setupUi(this);
  connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(new_file()));
  connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
  connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
  connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveas()));
  connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

  connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(search()));
  connect(ui->exploreButton, SIGNAL(clicked()), this, SLOT(explore()));
}

MainW::~MainW()
{
  delete ui;
  delete search_dialog;
}

/**
 * Open a new file.
 */
  bool
MainW::open_file(QString fname)
{
  QFile f(fname);
  if (!f.open(QFile::ReadOnly | QFile::Text))
    return false;

  filename = fname;
  QTextStream in(&f);
  ui->textEdit->setText(in.readAll());
  return true;
}

/**
 * Open a new file.
 */
  void
MainW::open()
{
  QString fname = QFileDialog::getOpenFileName
    (this, tr("Open File"), "", tr("Files (*.*)"));

  open_file(fname);
}

  void
MainW::new_file()
{
  this->filename = "";
  ui->textEdit->clear();
}

/**
 * Save the file.
 */
  bool
MainW::save()
{
  if (this->filename.isEmpty()) {
    return this->saveas();
  }

  QFile f(this->filename);
  if (!f.open(QFile::WriteOnly | QFile::Text))
    return false;

  QTextStream out(&f);
  out << ui->textEdit->toPlainText();
  return true;
}

/**
 * Save the file.
 */
  bool
MainW::saveas()
{
  QString fname = QFileDialog::getSaveFileName
    (this, tr("Save File"), this->filename, tr("Files (*.*)"));

  if (this->filename.isEmpty()) {
    this->filename = fname;
  }
  QFile f(fname);
  if (!f.open(QFile::WriteOnly | QFile::Text))
    return false;

  QTextStream out(&f);
  out << ui->textEdit->toPlainText();
  return true;
}

  void
MainW::search()
{
  if (!this->save())
    return;
  search_dialog->show();
  search_dialog->search(filename, ui->outputFileLineEdit->text());
}

  void
MainW::explore()
{
  if (!this->save())
    return;
  ExploreW explorew(this);
  explorew.explore(filename);
  explorew.exec();
}

