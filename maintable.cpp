#include "maintable.h"
#include "./ui_maintable.h"
#include "search.h"
#include <string>
#include <vector>
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
#include <QFile>
#include <QFileDialog>
#include <QtTest/QTest>
#include <stack>
std::stack <std::pair<std::pair<std::pair<int,int>,std::pair<int,int>> , std::pair<QString,QString>>> undo;
std::stack <std::pair<std::pair<std::pair<int,int>,std::pair<int,int>> , std::pair<QString,QString>>> redo;

QTableWidget * global;
void MainTable::Global_pointer()
{
    global = ui->bee_cell_table;
}
MainTable::MainTable(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainTable)
{
            ui->setupUi(this);
            ui->bee_cell_table->setRowCount(4096);
            ui->bee_cell_table->setColumnCount(4096);
            ui->bee_cell_table->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(ui->bee_cell_table, SIGNAL(customContextMenuRequested(QPoint)), SLOT(showContextMenu(QPoint)));
            QStringList bee_cell_table_list;
            bee_cell_table_list << "A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"<<"K"<<"L"<<"M"<<"N"<<"O"<<"P"<<"Q"<<"R"<<"S"<<"T"<<"U"<<"V"<<"W"<<"X"<<"Y"<<"Z";
            for (int i =1 ; i <= 163 ; i++)
            {
                bee_cell_table_list << "A"+ QString::number(i) <<"B" + QString::number(i)<<"C" + QString::number(i) <<"D" + QString::number(i) <<"E" +QString::number(i)<<"F" + QString::number(i)<<"G" + QString::number(i)<<"H" + QString::number(i)<<"I" + QString::number(i)<<"J" + QString::number(i)<<"K" + QString::number(i)<<"L" + QString::number(i)<<"M" + QString::number(i)<<"N" + QString::number(i)<<"O" + QString::number(i)<<"P" + QString::number(i)<<"Q"+ QString::number(i)<<"R"+ QString::number(i)<<"S" + QString::number(i)<<"T" + QString::number(i)<<"U" + QString::number(i)<<"V" + QString::number(i)<<"W" + QString::number(i)<<"X" + QString::number(i)<<"Y" + QString::number(i)<<"Z" + QString::number(i);
            }
            ui->bee_cell_table->blockSignals(true);
            for (int i = 0 ; i <=4096;i++)
            {
                for (int j =0 ; j<=4096 ; j++)
                {
                  //Заполнение ячеек таблицы пустыми значениями
                  ui->bee_cell_table->setItem(i, j, new QTableWidgetItem(""));
                }
            }
        //ui->bee_cell_table->item(2,2)->setSelected(true);
        ui->bee_cell_table->setSortingEnabled(true);
        ui->bee_cell_table->setHorizontalHeaderLabels( bee_cell_table_list);
        //Подключение слотов клавиш
        keyCtrlC = new QShortcut(this);
        keyCtrlC->setKey(Qt::CTRL | Qt::Key_C);
        connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onItemChanged(QTableWidgetItem*)));
        connect (keyCtrlC,SIGNAL(activated()),this,SLOT(slotShortcutCtrlC()));
        keyCtrlV = new QShortcut(this);
        keyCtrlV->setKey(Qt::CTRL | Qt::Key_V);
        connect (keyCtrlV,SIGNAL(activated()),this,SLOT(slotShortcutCtrlV()));
        keyCtrlZ = new QShortcut(this);
        keyCtrlZ->setKey(Qt::CTRL | Qt::Key_Z);
        connect (keyCtrlZ,SIGNAL(activated()),this,SLOT(slotShortcutCtrlZ()));
        keyCtrlF = new QShortcut(this);
        keyCtrlF->setKey(Qt::CTRL | Qt::Key_F);
        connect (keyCtrlF,SIGNAL(activated()),this,SLOT(slotShortcutCtrlF()));
        keyCtrlY = new QShortcut(this);
        keyCtrlY->setKey(Qt::CTRL | Qt::Key_Y);
        connect (keyCtrlY,SIGNAL(activated()),this,SLOT(slotShortcutCtrlY()));
        Delete = new QShortcut(this);
        Delete->setKey(Qt::Key_Delete );
        connect (Delete,SIGNAL(activated()),this,SLOT(slotDelete()));
        ui->bee_cell_table->blockSignals(false);
}
MainTable::~MainTable()
{
    delete ui;
}

//Реализация CTRL+C
void MainTable::slotShortcutCtrlC()
{
    QClipboard * clipboard = QApplication::clipboard();
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    QString Buffer;
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    //Нахождение выбранной области
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
       int Row_1 = 0;
       int Row_2;
       int Column_2;
       for (int i =Row_mn ;i <=Row_mx ; i++)
       {
           for (int j = Column_mn ; j<=Column_mx;j++)
           {
               if (ui->bee_cell_table->item(i,j)->text()!= "")
               {
               if (j != Column_mx){
               Buffer +=ui->bee_cell_table->item(i,j)->text() + "\t";
               }
               else {
                   Buffer +=ui->bee_cell_table->item(i,j)->text();
               }
               }
               else
               {
                   if (j != Column_mx){
                   Buffer +="\t";
                   }
               }
           }
           if (i != Row_mx)
           {
           Buffer+="\n";
           }
       }
       clipboard->setText(Buffer);
}
// РЕАЛИЗАЦИЯ CTRl+V
void MainTable::slotShortcutCtrlV()
{
    QClipboard* c_board=QApplication::clipboard();
    QString Buffer = c_board->text();
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    //НАХОЖДЕНИЕ ВЫБРАННОЙ ЯЧЕЙКИ КУДА НЕОБХОДИМО ВСТАВИТЬ
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    Row = Row_mn;
    Column = Column_mn;
    std::string Buffer_str = Buffer.toStdString();
    for (int i =0 ; i <Buffer_str.size(); i++)
    {
        if (Buffer_str[i]== '\t')
        {
            Column++;
        }
        if (Buffer_str[i]== '\n')
        {
            Row++;
            Column = Column_mn;
        }
        if (Buffer_str[i] != '\t' && Buffer_str[i] != '\n')
        {
            std::string reserve;
            while ( i < Buffer_str.size() && Buffer_str[i] != '\t' && Buffer_str[i] !='\n')
            {
                reserve+=Buffer_str[i];
                i++;
            }
            i--;
            ui->bee_cell_table->setItem(Row, Column, new QTableWidgetItem(QString::fromStdString(reserve)));
        }
    }
}
void MainTable::slotShortcutCtrlZ()
{
if (!undo.empty())
{
    ui->bee_cell_table->blockSignals(true);
    if (undo.top().first.first.first ==undo.top().first.second.first && undo.top().first.first.second ==undo.top().first.second.second){
    ui->bee_cell_table->item(undo.top().first.first.first,undo.top().first.second.second)->setText(undo.top().second.first);
    redo.push(std::make_pair(std::make_pair(undo.top().first.first,undo.top().first.second), std::make_pair(undo.top().second.first,undo.top().second.second)));
    }
    else
    {
        int Row_mn = undo.top().first.first.first;
        int Column_mn = undo.top().first.first.second;
        int Row_mx = undo.top().first.second.first;
        int Column_mx = undo.top().first.second.second;
        for (int i = Row_mn; i <=Row_mx;i++)
        {
            for (int j = Column_mn; j <=Column_mx;j++)
            {
                ui->bee_cell_table->setSpan(i,j,1,1);
            }
        }
    redo.push(std::make_pair(std::make_pair(std::make_pair(Row_mn,Column_mn),std::make_pair(Row_mx,Column_mx)), std::make_pair("","")));
    }
    undo.pop();
    ui->bee_cell_table->blockSignals(false);
}
}

void MainTable::slotShortcutCtrlF()
{
    Global_pointer();
    Search w;
    w.show();
    w.exec();
}

void MainTable::slotShortcutCtrlY()
{
    if (!redo.empty())
    {
        ui->bee_cell_table->blockSignals(true);
        if (redo.top().first.first.first ==redo.top().first.second.first && redo.top().first.first.second ==redo.top().first.second.second)
        {
        ui->bee_cell_table->item(redo.top().first.first.first,redo.top().first.first.second)->setText(redo.top().second.second);
        undo.push(std::make_pair(std::make_pair(redo.top().first.first,redo.top().first.second), std::make_pair(redo.top().second.first,redo.top().second.second)));
        }
        else
        {
            int Row_mn = redo.top().first.first.first;
            int Column_mn = redo.top().first.first.second;
            int Row_mx = redo.top().first.second.first;
            int Column_mx = redo.top().first.second.second;
            undo.push(std::make_pair(std::make_pair(std::make_pair(Row_mn,Column_mn),std::make_pair(Row_mx,Column_mx)), std::make_pair("","")));
            ui->bee_cell_table->setSpan(Row_mn,Column_mn,Row_mx-Row_mn+1,Column_mx-Column_mn+1);
        }
        redo.pop();
        ui->bee_cell_table->blockSignals(false);
    }
}
//УДаление значений из выбранной области
void MainTable::slotDelete()
{
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    QString Buffer;
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    for (int i =Row_mn ;i <=Row_mx ; i++)
    {
        for (int j = Column_mn ; j<=Column_mx;j++)
        {
             ui->bee_cell_table->setItem(i, j, new QTableWidgetItem(""));
        }
    }
}
void MainTable::showContextMenu(QPoint pos)
{
    QMenu* menu = new QMenu(this);
    QAction* editBoard = new QAction(tr("Изменить границы"), this);
    QAction* Span = new QAction(tr("Объденить ячейки"), this);
    connect(Span, SIGNAL(triggered()), this, SLOT(Spans()));
    QAction* deleteCell = new QAction(tr("Очистить ячейки"), this);
    connect(deleteCell, SIGNAL(triggered()), this, SLOT(Delete_cell()));
    QAction* editFont = new QAction(tr("Изменить шрифт"), this);
    connect(editFont, SIGNAL(triggered()), this, SLOT(Change_font()));
    QAction* editColor = new QAction(tr("Изменить цвет заливки ячейки"), this);
    connect(editColor, SIGNAL(triggered()), this, SLOT(Change_Color()));
    QAction* editColor_text = new QAction(tr("Изменить цвет текста ячейки"), this);
    connect(editColor_text, SIGNAL(triggered()), this, SLOT(Change_Color_text()));
    menu->addAction(editBoard);
    menu->addAction(Span);
    menu->addAction(deleteCell);
    menu->addAction(editFont);
    menu->addAction(editColor);
    menu->addAction(editColor_text);
    menu->popup(ui->bee_cell_table->viewport()->mapToGlobal(pos));
}
void MainTable::ChangeBoard()
{
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    for (int i =Row_mn ;i <=Row_mx ; i++)
    {
        for (int j = Column_mn ; j<=Column_mx;j++)
        {
             //ui->bee_cell_table->item(i,j)->setTextAlignment()
        }
    }
}
void MainTable::Spans()
{
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    QString Buffer;
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    QMessageBox::warning(this,"Внимание!","Будет сохранено только верхнее левое значение!");
    for (int i =Row_mn ;i <=Row_mx ; i++)
    {
        for (int j = Column_mn ; j<=Column_mx;j++)
        {
            if (ui->bee_cell_table->item(i,j)->text()!= "")
            {
            if (j != Column_mx){
            Buffer +=ui->bee_cell_table->item(i,j)->text() + "\t";
            }
            else {
                Buffer +=ui->bee_cell_table->item(i,j)->text();
            }
            }
            else
            {
                if (j != Column_mx){
                Buffer +="\t";
                }
            }
        }
        if (i != Row_mx)
        {
        Buffer+="\n";
        }
    }
    undo.push(std::make_pair(std::make_pair(std::make_pair(Row_mn,Column_mn),std::make_pair(Row_mx,Column_mx)), std::make_pair(Buffer,ui->bee_cell_table->item(Row_mn,Column_mn)->text())));
    ui->bee_cell_table->setSpan(Row_mn,Column_mn,Row_mx-Row_mn+1,Column_mx-Column_mn+1);
}
void MainTable::Delete_cell()
{
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    for (int i =Row_mn ;i <=Row_mx ; i++)
    {
        for (int j = Column_mn ; j<=Column_mx;j++)
        {
             ui->bee_cell_table->setItem(i, j, new QTableWidgetItem(""));
        }
    }
}
void MainTable::Change_font()
{
    bool ok ;
    QFont font = QFontDialog::getFont(&ok,QFont ("Segoe UI", 9));
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    for (int i =Row_mn ;i <=Row_mx ; i++)
    {
        for (int j = Column_mn ; j<=Column_mx;j++)
        {
             ui->bee_cell_table->item(i, j)->setFont(font);
        }
    }
}
void MainTable::Change_Color()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    for (int i =Row_mn ;i <=Row_mx ; i++)
    {
        for (int j = Column_mn ; j<=Column_mx;j++)
        {
             ui->bee_cell_table->item(i, j)->setBackground(QBrush(color));
        }
    }
}

void MainTable::Change_Color_text()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    for (int i =Row_mn ;i <=Row_mx ; i++)
    {
        for (int j = Column_mn ; j<=Column_mx;j++)
        {
             ui->bee_cell_table->item(i, j)->setForeground(color);
        }
    }
}
void MainTable::on_pushButton_12_clicked()
{
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    for (int i =Row_mn ;i <=Row_mx ; i++)
    {
        for (int j = Column_mn ; j<=Column_mx;j++)
        {
             ui->bee_cell_table->item(i, j)->setTextAlignment(Qt::AlignRight);
        }
    }
}
void MainTable::on_pushButton_10_clicked()
{
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    for (int i =Row_mn ;i <=Row_mx ; i++)
    {
        for (int j = Column_mn ; j<=Column_mx;j++)
        {
             ui->bee_cell_table->item(i, j)->setTextAlignment(Qt::AlignLeft);
        }
    }
}
void MainTable::on_pushButton_11_clicked()
{
    QList<QTableWidgetItem*> items = ui->bee_cell_table->selectedItems();
    int Row_mn =0 , Row_mx =0;
    int Column_mn =0 , Column_mx = 0;
    int count = items.count();
    int Row =0 ;
    int Column =0;
    for (int i =0 ; i < count ; i++)
    {
       if (i == 0)
       {
       Row_mn = ui->bee_cell_table->row(items.at(i));
       Column_mn = ui->bee_cell_table->column(items.at(i));
       Column_mx = Column_mn;
       Row_mx = Row_mn;
       }
       Row = ui->bee_cell_table->row(items.at(i));
       Column = ui->bee_cell_table->column(items.at(i));
       if (Column < Column_mn )
       {
           Column_mn = Column;
       }
       if (Column > Column_mx)
       {
           Column_mx =Column;
       }
       if (Row < Row_mn )
       {
           Row_mn = Row;
       }
       if (Row > Row_mx)
       {
           Row_mx =Row;
       }
    }
    for (int i =Row_mn ;i <=Row_mx ; i++)
    {
        for (int j = Column_mn ; j<=Column_mx;j++)
        {
             ui->bee_cell_table->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}
void MainTable::on_bee_cell_table_itemChanged(QTableWidgetItem *item)
{
    int row = item->row();
    int column = item->column();
    QString text = item->text();
    QString currentText = item->text();
    QString previousText = item->data(Qt::UserRole).toString();
    item->setData(Qt::UserRole, currentText);
    if (currentText != "" && currentText !=previousText )
    {
    undo.push(std::make_pair(std::make_pair(std::make_pair(row,column),std::make_pair(row,column)), std::make_pair(previousText,currentText)));
    }
}

