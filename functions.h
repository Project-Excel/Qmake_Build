#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QTableWidget>
#include <QString>
#include <QStringList>
#include <unordered_map>
#include <utility>
#include <regex>
#include "exprtk.hpp"

class Functions
{
private:
    QTableWidget* bee_cell_table;

    struct MyHash {
        std::size_t operator()(const std::pair<int, int>& p) const {

            std::size_t h1 = std::hash<int>()(p.first);
            std::size_t h2 = std::hash<int>()(p.second);

            return h1 ^ (h2 << 1);
        }
    };

    std::unordered_map<std::pair<int, int>, std::pair<std::pair<int, int>, QString>, MyHash> expressions;
    bool needToEvaluate;
    std::pair<int, int> previousClickedCell;
public:
    Functions(QTableWidget *);

    QString currentCellText;
    int isFunction                      (QString &);
    bool isNumber                       (const QChar &);
    QStringList functions;
    QString evaluation                  (QString &);
    bool correctSumFunction             (QString, std::pair<int, int> &, std::pair<int, int> &);
    bool correctMaxFunction             (QString, std::pair<int, int> &, std::pair<int, int> &);
    bool correctMinFunction             (QString, std::pair<int, int> &, std::pair<int, int> &);
    bool correctSaFunction              (QString, std::pair<int, int> &, std::pair<int, int> &);
    bool correctEsliFunction            (QString, QString &, QString &, QString &);
    bool correctChooseFunction          (QString, std::pair<int, int> &);
    bool correctLookFunction            (QString, QString&, std::pair<int, int> &, std::pair<int, int> &,
                                         std::pair<int, int> &, std::pair<int, int> &);
    bool correctSearchPositionFunction  (QString, std::pair<int, int> &, std::pair<int, int> &, QString &);
    bool correctDateFunction            (QString, std::pair<int, int> &, std::pair<int, int> &, std::pair<int, int> &);
    bool correctDaysFunction            (QString, std::pair<int, int> &, std::pair<int, int> &);
    bool correctTrigFunction            (QString, std::pair<int, double> &);
    bool correctExpression              (QString, QString &);
    bool isNumber                       (const QString &);
    QString sum                         (const QString &);
    QString max                         (const QString &);
    QString min                         (const QString &);
    QString sa                          (const QString &);
    QString esli                        (const QString &);
    QString choose                      (const QString &);
    QString look                        (const QString &);
    QString searchPosition              (const QString &);
    QString date                        (const QString &);
    QString days                        (const QString &);
    QString trig                        (const QString &, const int &);
    QString expression                  (const QString &);

    bool isString                       (const QString &);
    void removeLetters                  (QString &);
    bool checkCondition                 (const QString &);
    bool getString                      (QString &, QString &);
    bool getExpression                  (QString &, QString &);
    bool getDate                        (const QString &, int &, int &, int &);
    bool isCorrectDate                  (const int &, const int &, const int &);

    void cellChanged(const int&, const int&);
    void cellClicked(const int&, const int&);
};

class Date
{
private:
    short day;
    short month;
    short year;
    short NumDays();
public:
    Date(const int&, const int&, const int&);
    Date(const QString &);
    bool IsLeap();
    Date NextDay();
    Date PreviousDay();
    short WeekNumber();
    int Duration(Date, Date);
    QString DateToQString();
};

#endif // FUNCTIONS_H
