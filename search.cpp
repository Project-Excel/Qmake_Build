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
    ui->search_table->setColumnCount(2);
    ui->search_table->setHorizontalHeaderLabels(search_table_list);
    ui->search_table->setSortingEnabled(true);
    ui->search_table->horizontalHeader()->setStretchLastSection(true);
    ui->search_table->setEditTriggers(QTableWidget::NoEditTriggers);
}
Search::~Search()
{
    delete ui;
}
void find_In_Table( std::vector<std::pair<QString,std::pair<int, int>>>& arr, QTableWidget* tableWidget, const QString& textToFind)
{

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int column = 0; column < tableWidget->columnCount(); ++column)
        {
            QTableWidgetItem* item = tableWidget->item(row, column);
            if (item && item->text().contains(textToFind, Qt::CaseInsensitive))
            {
                QString value;
                std::pair<QString,std::pair<int, int>> res;
                value = tableWidget->item(row, column)->text();
                res = std::make_pair(value,std::make_pair(row,column));
                arr.push_back(res);
            }
        }
    }
}
int count;
extern QTableWidget *global;
std::vector<std::pair<QString,std::pair<int, int>>> arr;
void Search::on_pushButton_clicked()
{
    ui->search_table->setRowCount(0);
    QString search_info = ui->lineEdit->text();
    find_In_Table(arr,global,search_info);
    count = 0;
    for (int i =0 ; i < arr.size() ;i++)
    {
    ui->search_table->insertRow(count);
    ui->search_table->setItem(count, 0, new QTableWidgetItem(arr[i].first));
    ui->search_table->setItem(count,1,new QTableWidgetItem("Строка : " + QString::number(arr[i].second.first) + " " + "Столбец : " + QString::number(arr[i].second.second)));
    global->item(arr[i].second.first,arr[i].second.second)->setSelected(true);
    count++;
    }
    arr.clear();

}
