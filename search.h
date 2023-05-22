#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <QTabWidget>
#include "maintable.h"
#include "ui_maintable.h"
namespace Ui {
class Search;
}
class Search : public QDialog
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();
private slots:
    void on_pushButton_clicked();


private:
    Ui::Search *ui;
};

#endif // SEARCH_H
