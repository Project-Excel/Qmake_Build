#ifndef MAINTABLE_H
#define MAINTABLE_H
#include <QMainWindow>
#include <QShortcut>
#include <QMenu>
#include <string>
#include <iomanip>
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
#include <iostream>
#include <windows.h>
#include <vector>
#include <sstream>
#include <stack>
QT_BEGIN_NAMESPACE
namespace Ui { class MainTable; }
QT_END_NAMESPACE
class MainTable : public QMainWindow
{
    Q_OBJECT
public:
    MainTable(QWidget *parent = nullptr);
    void Global_pointer();
    ~MainTable();
private slots :
void slotShortcutCtrlC();
void slotShortcutCtrlV();
void slotShortcutCtrlZ();
void slotShortcutCtrlF();
void slotShortcutCtrlY();
void slotDelete();
void showContextMenu(QPoint);
void ChangeBoard();
void Spans();
void Delete_cell();
void Change_font();
void Change_Color();
void Change_Color_text();
void on_pushButton_12_clicked();
void on_pushButton_10_clicked();
void on_pushButton_11_clicked();
void on_bee_cell_table_itemChanged(QTableWidgetItem *item);
void on_tabWidget_2_tabBarClicked(int index);
void on_bee_cell_table_2_itemChanged(QTableWidgetItem *item);
void itemChanged(QTableWidgetItem *item);
private:
    Ui::MainTable *ui;
    QShortcut *keyCtrlC;
    QShortcut *Delete;
    QShortcut *keyCtrlV;
    QShortcut *keyCtrl;
    QShortcut *keyCtrlZ;
    QShortcut *keyCtrlF;
    QShortcut *keyCtrlY;
};

#endif
