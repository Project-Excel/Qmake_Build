#include "search.h"
#include "ui_search.h"
#include "maintable.h"
#include "ui_maintable.h"
#include <QString>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <QApplication>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QClipboard>
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QWidget>
#include <QPoint>
#include <QAction>
#include <QApplication>
#include <QFontDialog>
#include <QFont>
#include <QColorDialog>
#include <QColor>
#include <QMessageBox>
#include <QUndoCommand>
#include <QUndoStack>
#include <QPen>
#include <QUndoGroup>
#include <cstring>
#include <QFile>
#include <QFileDialog>
Search::Search(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
    QStringList search_table_list;
    search_table_list << "Результаты поиска"<<"Индекс";
    ui->search_table->setHorizontalHeaderLabels(search_table_list);
}

Search::~Search()
{
    delete ui;
}

std::pair<QString,std::pair<int, int>> find_In_Table(QTableWidget* tableWidget, const QString& textToFind)
{
    std::pair<QString,std::pair<int, int>> res;
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int column = 0; column < tableWidget->columnCount(); ++column) {
            QTableWidgetItem* item = tableWidget->item(row, column);
            if (item && item->text().contains(textToFind, Qt::CaseInsensitive)) {
                QString value;
                value = tableWidget->item(row, column)->text();
                res = std::make_pair(value,std::make_pair(row,column));
            }
        }
    }
    return res;
}
extern QTableWidget *bee_cell_table;
void Search::on_pushButton_clicked()
{
    QString search_info = ui->lineEdit->text();
    std::pair<QString,std::pair<int,int>> res = find_In_Table(bee_cell_table,search_info);
    int count = 0;
    ui->search_table->insertRow(count);
    ui->search_table->setItem(count, 0, new QTableWidgetItem(res.first));
    ui->search_table->setItem(count,1,new QTableWidgetItem(QString::number(res.second.first) + " " + QString::number(res.second.second)));
}


