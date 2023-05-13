#include "functions.h"

#include <QDebug>

Functions::Functions(QTableWidget *t): bee_cell_table(t)
{
    needToEvaluate = true;
    previousClickedCell = {-1, -1};

    functions << "=SUM(" << "=MAX(" << "=MIN(" << "=SARIF(" << "=IF(" << "=CHOOSE(" << "=LOOK(" << "=SRCHPOS("
              << "=DATE(" << "=DAYS(" << "=SIN(" << "=COS(" << "=TAN(" << "=COT(" << "=LOG(" << "=";
}

Date::Date(const int &i1, const int &i2, const int &i3)
{
    day = i1;
    month = i2;
    year = i3;
}

Date::Date(const QString &str)
{
    QString buff = "";

    buff += str[0];
    buff += str[1];

    day = buff.toInt();
    buff = "";

    buff += str[3];
    buff += str[4];

    month = buff.toInt();
    buff = "";

    buff += str[6];
    buff += str[7];
    buff += str[8];
    buff += str[9];

    year = buff.toInt();
    buff = "";
}

short Date::NumDays()
{
    if (month ^ 2)
        return 30 + (month + (month / 8)) % 2;
    if (this->IsLeap())
        return 29;
    return 28;
}

bool Date::IsLeap()
{
    return ( (!(year % 4)) && (year % 100) ) || ( !(year % 400) )  ;
}

Date Date::NextDay()
{
    Date next(day, month, year);

    short num = next.NumDays();

    if (next.day ^ num)
    {
        ++next.day;
    }
    else
    {
        if (next.month ^ 12)
        {
            next.day = 1;
            ++next.month;
        }
        else
        {
            next.day = 1;
            next.month = 1;
            ++next.year;
        }
    }

    return next;
}

Date Date::PreviousDay()
{
    if (!(day ^ 1) && !(month ^ 1) && !(year ^ 1))
        return Date(0, 0, 0);

    Date prev(day, month, year);

    if (prev.day ^ 1)
    {
        --prev.day;
    }
    else
    {
        if (prev.month ^ 1)
        {
            --prev.month;
            prev.day = prev.NumDays();
        }
        else
        {
            --prev.year;
            prev.day = 31;
            prev.month = 12;
        }
    }

    return prev;
}

short Date::WeekNumber()
{
    short days = 0;

    Date temp(1, 1, year);

    for (; temp.month < this->month; ++temp.month)
        days += temp.NumDays();

    days += this->day - 1;

    return days / 7 + 1;
}

int Date::Duration(Date date, Date curr)
{
    if (date.year ^ curr.year)
    {
        int dyear = 0;

        bool minus = (date.year < curr.year);

        Date mindate = (minus) ? date : curr;
        Date maxdate = (minus) ? curr : date;

        if ((maxdate.year - mindate.year) > 1)
        {
            dyear += (maxdate.year - mindate.year - 1) * 365;

            int minyear = mindate.year + 1;
            int maxyear = maxdate.year - 1;

            if (minyear % 4)
            {
                minyear += 4 - (minyear % 4);
            }

            if (maxyear % 4)
            {
                maxyear -= maxyear % 4;
            }

            if (maxyear >= minyear)
                dyear += maxyear / 4 - minyear / 4 + 1;

            minyear = mindate.year + 1;
            maxyear = maxdate.year - 1;

            if (minyear % 100)
            {
                minyear += 100 - (minyear % 100);
            }

            if (maxyear % 100)
            {
                maxyear -= maxyear % 100;
            }

            if (maxyear >= minyear)
                dyear -= maxyear / 100 - minyear / 100 + 1;

            minyear = mindate.year + 1;
            maxyear = maxdate.year - 1;

            if (minyear % 400)
            {
                minyear += 400 - (minyear % 400);
            }

            if (maxyear % 400)
            {
                maxyear -= maxyear % 400;
            }

            if (maxyear >= minyear)
                dyear += maxyear / 400 - minyear / 400 + 1;
        }

        dyear += mindate.NumDays() - mindate.day;

        ++mindate.month;

        for (; mindate.month <= 12; ++mindate.month)
        {
            dyear += mindate.NumDays();
        }


        dyear += maxdate.day;

        --maxdate.month;

        for (; maxdate.month >= 1; --maxdate.month)
        {
            dyear += maxdate.NumDays();
        }

        return (minus) ? -dyear : dyear;
    }
    else
    {
        if (date.month ^ curr.month)
        {
            int dmonth = 0;

            bool minus = (date.month < curr.month);

            Date minmonth = (minus) ? date : curr;
            Date maxmonth = (minus) ? curr : date;

            dmonth += minmonth.NumDays() - minmonth.day;

            ++minmonth.month;

            for (; minmonth.month < maxmonth.month; ++minmonth.month)
            {
                dmonth += minmonth.NumDays();
            }

            dmonth += maxmonth.day;

            return (minus) ? -dmonth : dmonth;
        }
        else
        {
            return date.day - curr.day;
        }
    }

}

QString Date::DateToQString()
{
    QString result;

    result += QString::number(this->day);

    result += ".";

    result += QString::number(this->month);

    result += ".";

    result += QString::number(this->year);

    return result;
}

QString Functions::evaluation(QString &currentText)
{
    int function = isFunction(currentText);

    if (function ^ -1)
    {
        if ((currentText[currentText.length() - 1] != ')') && (function ^ 15)) currentText += ')';
        if (!function)           return sum(currentText);
        else if (function == 1)  return max(currentText);
        else if (function == 2)  return min(currentText);
        else if (function == 3)  return sa(currentText);
        else if (function == 4)  return esli(currentText);
        else if (function == 5)  return choose(currentText);
        else if (function == 6)  return look(currentText);
        else if (function == 7)  return searchPosition(currentText);
        else if (function == 8)  return date(currentText);
        else if (function == 9)  return days(currentText);
        else if (function == 10) return trig(currentText, 1);
        else if (function == 11) return trig(currentText, 2);
        else if (function == 12) return trig(currentText, 3);
        else if (function == 13) return trig(currentText, 4);
        else if (function == 14) return log(currentText);
        else if (function == 15) return expression(currentText);
   }

   return currentText;
}

int Functions::isFunction(QString &str)
{
    str = str.trimmed();
    str.replace(" ", "");
    str = str.toUpper();

    bool currentFlag = 1;

    for (int i = 0; i < functions.count(); ++i)
    {
        for (int j = 0; j < functions[i].length(); ++j)
        {
            if (functions[i][j] != str[j]) { currentFlag = 0; break; }
        }

        if (currentFlag) return i;
        currentFlag = 1;
    }

    return -1;
}

bool Functions::isNumber(const QChar &c)
{
    return ((c.isNumber()) || (c == '.') || (c == ','));
}

bool Functions::correctSumFunction(QString str, std::pair<int, int> &col1, std::pair<int, int> &col2)
{
    if (!std::regex_match(str.toStdString(), std::regex("^.{4}\\({1}[A-Z]{1}[0-9]+\\:{1}[A-Z]{1}[0-9]+\\){1}$")))
        return false;

    col1 = {}; col2 = {};

    int i = 5;
    int nums = 1;

    if (str[i].isLetter()) col1.first = str[i].unicode() - 65; else {col1 = {}; col2 = {}; return false; } ++i;
    while(str[i].isNumber()) { col1.second+= nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; col2 = {}; return false; }
    if (str[i] != ':') { col1 = {}; col2 = {}; return false; } ++i;

    nums = 1;

    if (str[i].isLetter()) col2.first = str[i].unicode() - 65; else { col1 = {}; col2 = {}; return false; } ++i;
    while(str[i].isNumber()) { col2.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (col1.first > col2.first) std::swap(col1.first, col2.first);
    if (col1.second > col2.second) std::swap(col1.second, col2.second);

    return true;
}

bool Functions::correctMaxFunction(QString str, std::pair<int, int> &col1, std::pair<int, int> &col2)
{
    if (!std::regex_match(str.toStdString(), std::regex("^.{4}\\({1}[A-Z]{1}[0-9]+\\:[A-Z]{1}[0-9]+\\){1}$")))
        return false;

    col1 = {}; col2 = {};

    int i = 5;
    int nums = 1;

    if (str[i].isLetter()) col1.first = str[i].unicode() - 65; else {col1 = {}; col2 = {}; return false; } ++i;
    while(str[i].isNumber()) { col1.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; col2 = {}; return false; }
    if (str[i] != ':') { col1 = {}; col2 = {}; return false; } ++i;

    nums = 1;

    if (str[i].isLetter()) col2.first = str[i].unicode() - 65; else { col1 = {}; col2 = {}; return false; } ++i;
    while(str[i].isNumber()) { col2.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (col1.first > col2.first) std::swap(col1.first, col2.first);
    if (col1.second > col2.second) std::swap(col1.second, col2.second);

    return true;
}

bool Functions::correctMinFunction(QString str, std::pair<int, int> &col1, std::pair<int, int> &col2)
{
    if (!std::regex_match(str.toStdString(), std::regex("^.{4}\\({1}[A-Z]{1}[0-9]+\\:[A-Z]{1}[0-9]+\\){1}$")))
        return false;

    col1 = {}; col2 = {};

    int i = 5;
    int nums = 1;

    if (str[i].isLetter()) col1.first = str[i].unicode() - 65; else {col1 = {}; col2 = {}; return false; } ++i;
    while(str[i].isNumber()) { col1.second+= nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; col2 = {}; return false; }
    if (str[i] != ':') { col1 = {}; col2 = {}; return false; } ++i;

    nums = 1;

    if (str[i].isLetter()) col2.first = str[i].unicode() - 65; else { col1 = {}; col2 = {}; return false; } ++i;
    while(str[i].isNumber()) { col2.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (col1.first > col2.first) std::swap(col1.first, col2.first);
    if (col1.second > col2.second) std::swap(col1.second, col2.second);

    return true;
}

bool Functions::correctSaFunction(QString str, std::pair<int, int> &col1, std::pair<int, int> &col2)
{
    if (!std::regex_match(str.toStdString(), std::regex("^.{6}\\({1}[A-Z]{1}[0-9]+\\:[A-Z]{1}[0-9]+\\){1}$")))
        return false;

    col1 = {}; col2 = {};

    int i = 7;
    int nums = 1;

    if (str[i].isLetter()) col1.first = str[i].unicode() - 65; else {col1 = {}; col2 = {}; return false; } ++i;
    while(str[i].isNumber()) { col1.second+= nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; col2 = {}; return false; }
    if (str[i] != ':') { col1 = {}; col2 = {}; return false; } ++i;

    nums = 1;

    if (str[i].isLetter()) col2.first = str[i].unicode() - 65; else { col1 = {}; col2 = {}; return false; } ++i;
    while(str[i].isNumber()) { col2.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (col1.first > col2.first) std::swap(col1.first, col2.first);
    if (col1.second > col2.second) std::swap(col1.second, col2.second);

    return true;
}

bool Functions::correctEsliFunction(QString str, QString &condition, QString &first, QString &second)
{
    if (!std::regex_match(str.toStdString(), std::regex("^.{3}\\({1}.+\\,{1}.+\\,{1}.+\\){1}$"))) return false;

    QString buffer{};
    QString cell{};

    condition = first = second = "";

    int i = 4;

    while (str[i] != ',') buffer += str[i++];

    if (!getString(buffer, condition)) return false;

    if (isString(condition))
    {
        if (!checkCondition(condition)) return false;

        removeLetters(condition);
    }

    ++i;

    buffer.clear();

    while (str[i] != ',') buffer += str[i++];

    if (!getString(buffer, first)) return false;

    ++i;

    buffer.clear();

    while (str[i] != ')') buffer += str[i++];

    if (!getString(buffer, second)) return false;

    return true;
}

bool Functions::correctChooseFunction(QString str, std::pair<int, int> &col1)
{
    if (!std::regex_match(str.toStdString(), std::regex("^.{7}\\({1}[A-Z]{1}[0-9]+\\){1}$"))) return false;

    col1 = {};

    int i = 8;
    int nums = 1;

    if (str[i].isLetter()) col1.first = str[i].unicode() - 65; else { col1 = {}; return false; } ++i;
    while (str[i].isNumber()) { col1.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; return false; }

    return true;
}

bool Functions::correctLookFunction(QString str, QString &search, std::pair<int, int> &col1, std::pair<int, int> &col2,
                                    std::pair<int, int> &col3, std::pair<int, int> &col4)
{
    if (!std::regex_match(str.toStdString(),
         std::regex("^.{5}\\({1}.+\\,{1}[A-Z]{1}[0-9]+\\:[A-Z]{1}[0-9]+\\,[A-Z]{1}[0-9]+\\:[A-Z]{1}[0-9]+\\){1}$")))
        return false;

    col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear();

    int i = 6;
    int nums = 1;

    while (str[i] != ',') { search += str[i]; ++i; } ++i;

    if (str[i].isLetter()) col1.first = str[i].unicode() - 65;
    else { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; } ++i;
    while (str[i].isNumber()) { col1.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; } nums = 1;
    if (str[i] != ':') { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; } ++i;

    if (str[i].isLetter()) col2.first = str[i].unicode() - 65;
    else { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; } ++i;
    while(str[i].isNumber()) { col2.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; } nums = 1;
    if (str[i] != ',') { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; } ++i;

    if (str[i].isLetter()) col3.first = str[i].unicode() - 65;
    else { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; } ++i;
    while (str[i].isNumber()) { col3.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; } nums = 1;
    if (str[i] != ':') { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; } ++i;

    if (str[i].isLetter()) col4.first = str[i].unicode() - 65;
    else { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; } ++i;
    while(str[i].isNumber()) { col4.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; col2 = {}; col3 = {}; col4 = {}; search.clear(); return false; }

    if (col1.first > col2.first) std::swap(col1.first, col2.first);
    if (col1.second > col2.second) std::swap(col1.second, col2.second);

    if (col3.first > col4.first) std::swap(col3.first, col4.first);
    if (col3.second > col4.second) std::swap(col3.second, col4.second);

    return true;
}

bool Functions::correctSearchPositionFunction(QString str, std::pair<int, int> &col1,
                                              std::pair<int, int> &col2, QString &search)
{
    if (!std::regex_match(str.toStdString(), std::regex("^.{8}\\({1}.+\\,[A-Z]{1}[0-9]+\\:[A-Z]{1}[0-9]+\\){1}$")))
        return false;

    col1 = {}; col2 = {}; search.clear();

    int i = 9;
    int nums = 1;

    while (str[i] != ',')  { search += str[i]; ++i; } ++i;

    if (str[i].isLetter()) { col1.first = str[i].unicode() - 65; }
    else { col1 = {}; col2 = {}; search.clear(); return false; } ++i;
    while (str[i].isNumber()) { col1.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; col2 = {}; search.clear(); return false; } nums = 1;
    if (str[i] != ':') { col1 = {}; col2 = {}; search.clear(); return false; } ++i;

    if (str[i].isLetter()) { col2.first = str[i].unicode() - 65; }
    else { col1 = {}; col2 = {}; search.clear(); return false; } ++i;
    while (str[i].isNumber()) { col2.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { col1 = {}; col2 = {}; search.clear(); return false; }

    if (col1.first > col2.first) std::swap(col1.first, col2.first);
    if (col1.second > col2.second) std::swap(col1.second, col2.second);

    return true;
}

bool Functions::correctDateFunction(QString str, std::pair<int, int> &c1, std::pair<int, int> &c2, std::pair<int, int> &c3)
{
    if (!std::regex_match(str.toStdString(),
                          std::regex("^.{5}\\({1}[A-Z]{1}[0-9]+\\,[A-Z]{1}[0-9]+\\,[A-Z]{1}[0-9]+\\){1}$")))
        return false;

    c1 = {}; c2 = {}; c3 = {};

    int i = 6;
    int nums = 1;

    if (str[i].isLetter()) { c1.first = str[i].unicode() - 65; } else { c1 = {}; c2 = {}; c3 = {}; return false; } ++i;
    while (str[i].isNumber()) { c1.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { c1 = {}; c2 = {}; c3 = {}; return false; } nums = 1;
    if (str[i] != ',') { c1 = {}; c2 = {}; c3 = {}; return false; } ++i;

    if (str[i].isLetter()) { c2.first = str[i].unicode() - 65; } else { c1 = {}; c2 = {}; c3 = {}; return false; } ++i;
    while (str[i].isNumber()) { c2.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { c1 = {}; c2 = {}; c3 = {}; return false; } nums = 1;
    if (str[i] != ',') { c1 = {}; c2 = {}; c3 = {}; return false; } ++i;

    if (str[i].isLetter()) { c3.first = str[i].unicode() - 65; } else { c1 = {}; c2 = {}; c3 = {}; return false; } ++i;
    while (str[i].isNumber()) { c3.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { c1 = {}; c2 = {}; c3 = {}; return false; }

    return true;
}

bool Functions::correctDaysFunction(QString str, std::pair<int, int> &c1, std::pair<int, int> &c2)
{
    if (!std::regex_match(str.toStdString(), std::regex("^.{5}\\({1}[A-Z]{1}[0-9]+\\,{1}[A-Z]{1}[0-9]{1}\\){1}$")))
        return false;

    c1 = {}; c2 = {};

    int i = 6;
    int nums = 1;

    if (str[i].isLetter()) { c1.first = str[i].unicode() - 65; } else { return false; } ++i;
    while (str[i].isNumber()) { c1.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { c1 = {}; c2 = {}; return false; } nums = 1;
    if (str[i] != ',') { c1 = {}; c2 = {}; return false; } ++i;

    if (str[i].isLetter()) { c2.first = str[i].unicode() - 65; } else { c1 = {}; c2 = {}; return false; } ++i;
    while (str[i].isNumber()) { c2.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

    if (nums == 1) { c1 = {}; c2 = {}; return false; }

    return true;
}

bool Functions::correctTrigFunction(QString str, std::pair<int, double> &c)
{
    if (std::regex_match(str.toStdString(), std::regex("^.{4}\\({1}[A-Z]{1}[0-9]+\\){1}$")))
    {
        c = {};

        int i = 5;
        int nums = 1;

        if (str[i].isLetter()) { c.first = str[i].unicode() - 65; } else return false; ++i;
        while (str[i].isNumber()) { c.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

        if (nums == 1) { c = {}; return false; }

        return true;
    }

    if (std::regex_match(str.toStdString(), std::regex("^.{4}\\({1}\\-?[0-9]+[.,]?[0-9]+\\){1}$"))
            || std::regex_match(str.toStdString(), std::regex("^.{4}\\({1}\\-?[0-9]+\\){1}")))
    {
        c = {-1, 0};

        int i = 5;
        QString buffer{};

        while (str[i] != ')') buffer += str[i++];

        c.second = buffer.toDouble();

        return true;
    }

    return false;
}

bool Functions::correctExpression(QString str, QString &out)
{
    str.remove(0, 1);

    for (int i = 0; i < str.length(); ++i)
    {
        if (!str[i].isLetterOrNumber() && str[i] != '.' && str[i] != ',' && str[i] != '+' && str[i] != '-'
                && str[i] != '/' && str[i] != '*' && str[i] != '^' && str[i] != '(' && str[i] != ')') return false;
    }

    return getExpression(str, out);
}

bool Functions::correctLogFunction(QString str, std::pair<int, double> &c)
{
    if (std::regex_match(str.toStdString(), std::regex("^.{4}\\({1}[A-Z]{1}[0-9]+\\){1}$")))
    {
        c = {};

        int i = 5;
        int nums = 1;

        if (str[i].isLetter()) { c.first = str[i].unicode() - 65; } ++i;
        while (str[i].isNumber()) { c.second += nums * (str[i].unicode() - 48); nums *= 10; ++i; }

        return true;
    }

    if (std::regex_match(str.toStdString(), std::regex("^.{4}\\({1}[0-9]+[.,]?[0-9]+\\){1}$"))
            || std::regex_match(str.toStdString(), std::regex("^.{4}\\({1}[0-9]+\\){1}$")))
    {
        QString buffer{};

        int i = 5;

        while (isNumber(str[i])) buffer += str[i++];

        c.first = -1;
        c.second = buffer.toDouble();

        return true;
     }

    return false;
}

bool Functions::isNumber(const QString &str)
{
    return (std::regex_match(str.toStdString(), std::regex("^\\-?[0-9]+[.,]?[0-9]+$")))
            || (std::regex_match(str.toStdString(), std::regex("^\\-?[0-9]+$")));
}

QString Functions::sum(const QString &currentText)
{
    std::pair<int, int> firstColumn {0, 0};
    std::pair<int, int> secondColumn {0, 0};

    if (!correctSumFunction(currentText.trimmed().toUpper(), firstColumn, secondColumn)) return "ERROR";
    else
    {
        --firstColumn.second;
        --secondColumn.second;

        double sum = 0;
        QString buff{};

        for (int i = firstColumn.first; i <= secondColumn.first; ++i)
        {
            for (int j = firstColumn.second; j <= secondColumn.second; ++j)
            {
                if (bee_cell_table->item(j, i)) buff = bee_cell_table->item(j, i)->text(); else buff = "";
                if (isNumber(buff)) sum += buff.toDouble();
            }
        }

        return QString::number(sum);
    }
}

QString Functions::max(const QString &currentText)
{
    std::pair<int, int> firstColumn {0, 0};
    std::pair<int, int> secondColumn {0, 0};

    if (!correctMaxFunction(currentText.trimmed().toUpper(), firstColumn, secondColumn)) return "ERROR";
    else
    {
        --firstColumn.second;
        --secondColumn.second;

        double maximum = std::numeric_limits<double>::lowest();
        QString buff{};

        for (int i = firstColumn.first; i <= secondColumn.first; ++i)
        {
            for (int j = firstColumn.second; j <= secondColumn.second; ++j)
            {
                if (!(bee_cell_table->item(j, i))) buff = "";
                else
                {
                    buff = bee_cell_table->item(j, i)->text();
                    if (isNumber(buff) && (buff.toInt() > maximum)) maximum = buff.toDouble();
                }
            }
        }

        return QString::number(maximum);
    }
}

QString Functions::min(const QString &currentText)
{
    std::pair<int, int> firstColumn {0, 0};
    std::pair<int, int> secondColumn {0, 0};

    if (!correctMinFunction(currentText.trimmed().toUpper(), firstColumn, secondColumn)) return "ERROR";
    else
    {
        --firstColumn.second;
        --secondColumn.second;

        double minimum = std::numeric_limits<double>::max();
        QString buff{};

        for (int i = firstColumn.first; i <= secondColumn.first; ++i)
        {
            for (int j = firstColumn.second; j <= secondColumn.second; ++j)
            {
                if (!(bee_cell_table->item(j, i))) buff = "";
                else
                {
                    buff = bee_cell_table->item(j, i)->text();
                    if (isNumber(buff) && (buff.toInt() < minimum)) minimum = buff.toDouble();
                }
            }
        }

        return QString::number(minimum);
    }
}

QString Functions::sa(const QString &currentText)
{
    std::pair<int, int> firstColumn {0, 0};
    std::pair<int, int> secondColumn {0, 0};

    if (!correctSaFunction(currentText.trimmed().toUpper(), firstColumn, secondColumn)) return "ERROR";
    else
    {
        --firstColumn.second;
        --secondColumn.second;

        double sum = 0.0;
        QString buff{};
        int count = 0;

        for (int i = firstColumn.first; i <= secondColumn.first; ++i)
        {
            for (int j = firstColumn.second; j <= secondColumn.second; ++j)
            {
                if (bee_cell_table->item(j, i)) buff = bee_cell_table->item(j, i)->text(); else buff = "";
                if (isNumber(buff)) sum += buff.toDouble();
                ++count;
            }
        }

        return QString::number(sum / count);
    }
}

QString Functions::esli(const QString &str)
{
    QString condition{};
    QString first{};
    QString second{};

    if (!correctEsliFunction(str, condition, first, second)) return "ERROR";

    exprtk::symbol_table<double> table;
    exprtk::expression<double> exp;

    exp.register_symbol_table(table);

    exprtk::parser<double> parser;

    if (!parser.compile(condition.toStdString(), exp)) return "ERROR";

    QString result = (exp.value() != 0.0) ? first : second;

    bool isString = false;

    for (int i = 0; i < result.length(); ++i)
    {
        if (result[i] == QChar(39)) { isString = true; break; }
    }

    if (isString)
    {
        return result.replace(QChar(39), QChar(0));
    }

    exprtk::symbol_table<double> tableDouble;
    exprtk::expression<double> expDouble;

    expDouble.register_symbol_table(tableDouble);

    exprtk::parser<double> parserDouble;

    if (!parserDouble.compile(result.toStdString(), expDouble)) return "ERROR";

    return QString::number(expDouble.value());
}

QString Functions::choose(const QString &str)
{
    std::pair<int, int> column{0, 0};

    if (!correctChooseFunction(str, column)) return "ERROR";
    else
    {
        --column.second;

        if (bee_cell_table->item(column.second, column.first)) return bee_cell_table->item(column.second, column.first)->text();
    }

    return "";
}

QString Functions::look(const QString &str)
{
    QString search{};
    std::pair<int, int> firstColumn{};
    std::pair<int, int> secondColumn{};
    std::pair<int, int> thirdColumn{};
    std::pair<int, int> fourthColumn{};

    if (!correctLookFunction(str, search, firstColumn, secondColumn, thirdColumn, fourthColumn)) return "ERROR";
    else
    {
        --firstColumn.second;
        --secondColumn.second;
        --thirdColumn.second;
        --fourthColumn.second;

        QString buff{};
        int count = 0;

        for (int i = firstColumn.first; i <= secondColumn.first; ++i)
        {
            for (int j = firstColumn.second; j <= secondColumn.second; ++j)
            {
                if (bee_cell_table->item(j, i)) buff = bee_cell_table->item(j, i)->text(); else buff = "";

                if (buff.toUpper() == search)
                {
                    for (int k = thirdColumn.first; k <= fourthColumn.first; ++k)
                    {
                        for (int l = thirdColumn.second; l <= fourthColumn.second; ++l)
                        {
                            if (bee_cell_table->item(l, k)) buff = bee_cell_table->item(l, k)->text(); else buff = "";

                            if (!count) return buff;
                            --count;
                        }
                    }

                    if (!count) return buff;
                    else return "NOT FOUND";
                }

                ++count;
            }
        }
    }

    return "NOT FOUND";
}

QString Functions::searchPosition(const QString &str)
{
    QString search{};
    std::pair<int, int> firstColumn{};
    std::pair<int, int> secondColumn{};

    if (!correctSearchPositionFunction(str, firstColumn, secondColumn, search)) return "ERROR";

    --firstColumn.second;
    --secondColumn.second;

    QString buff{};
    int count = 1;

    for (int i = firstColumn.first; i <= secondColumn.first; ++i)
    {
        for (int j = firstColumn.second; j <= secondColumn.second; ++j)
        {
            if (bee_cell_table->item(j, i)) buff = bee_cell_table->item(j, i)->text(); else buff = "";

            buff = buff.toUpper();

            if (search == buff) return QString::number(count);
            ++count;
        }
    }

    return "NOT FOUND";
}

QString Functions::date(const QString &str)
{
    std::pair<int, int> year{};
    std::pair<int, int> month{};
    std::pair<int, int> day{};

    if (!correctDateFunction(str, year, month, day)) return "ERROR";

    QString buff{};
    bool flag = 1;
    QString res{};
    int yearInt = 1;
    int monthInt = 1;
    int dayInt = 1;

    --year.second;
    --month.second;
    --day.second;

    if (bee_cell_table->item(year.second, year.first)) buff = bee_cell_table->item(year.second, year.first)->text();

    for (int i = 0; i < buff.length(); ++i)
    {
        if (!buff[i].isNumber()) { flag = 0; break; }
    }

    if (flag) { yearInt = buff.toInt(); } else return "NaN";

    if (bee_cell_table->item(month.second, month.first)) buff = bee_cell_table->item(month.second, month.first)->text();
    else buff = "";

    for (int i = 0; i < buff.length(); ++i)
    {
        if (!buff[i].isNumber()) { flag = 0; break; }
    }

    if (flag) { monthInt = buff.toInt(); } else return "NaN";

    if (bee_cell_table->item(day.second, day.first)) buff = bee_cell_table->item(day.second, day.first)->text();
    else buff = "";

    for (int i = 0; i < buff.length(); ++i)
    {
        if (!buff[i].isNumber()) { flag = 0; break; }
    }

    if (flag) { dayInt = buff.toInt(); } else return "NaN";

    if (!isCorrectDate(dayInt, monthInt, yearInt)) return "NaN";

    return QString::number(dayInt) + '.' + QString::number(monthInt) + '.' + QString::number(yearInt);
}

QString Functions::days(const QString &str)
{
    std::pair <int, int> first{};
    std::pair <int, int> second{};

    if (!correctDaysFunction(str, first, second)) return "ERROR";

    --first.second;
    --second.second;

    QString date{};
    int day = 1;
    int month = 1;
    int year = 1;
    int day2 = 1;
    int month2 = 1;
    int year2 = 1;

    if (bee_cell_table->item(first.second, first.first)) date = bee_cell_table->item(first.second, first.first)->text();

    if (!getDate(date, day, month, year)) return "NaN";

    if (bee_cell_table->item(second.second, second.first)) date = bee_cell_table->item(second.second, second.first)->text();

    if (!getDate(date,  day2, month2, year2)) return "NaN";

    if (!isCorrectDate(day, month, year) || !isCorrectDate(day2, month2, year2)) return "NaN";

    Date d1(day, month, year);
    Date d2(day2, month2, year2);

    return QString::number(d1.Duration(d1, d2));
}

QString Functions::trig(const QString &str, const int &n)
{
    std::pair <int, double> c{};
    double result = 0.0;

    if (!correctTrigFunction(str, c)) return "ERROR";

    if (c.first ^ -1)
    {
        --c.second;

        QString cell{};

        if (bee_cell_table->item(c.second, c.first)) cell = bee_cell_table->item(c.second, c.first)->text();

        if (!isNumber(cell)) return "NaN";

        switch (n)
        {
        case 1:
            result = sin(cell.toDouble());
            break;
        case 2:
            result = cos(cell.toDouble());
            break;
        case 3:
            result = tan(cell.toDouble());
            break;
        case 4:
            result = 1 / tan(cell.toDouble());
            break;
        default:
            break;
        }

        return QString::number(result);
    }

    switch (n)
    {
    case 1:
        result = sin(c.second);
        break;
    case 2:
        result = cos(c.second);
        break;
    case 3:
        result = tan(c.second);
        break;
    case 4:
        result = 1 / tan(c.second);
        break;
    default:
        break;
    }

    return QString::number(result);

}

QString Functions::expression(const QString &str)
{
    QString expr{};

    if (!correctExpression(str, expr)) return "NaN";

    exprtk::symbol_table<double> table;
    exprtk::expression<double> exp;

    exp.register_symbol_table(table);

    exprtk::parser<double> parser;

    if (!parser.compile(expr.toStdString(), exp)) return "ERROR";

    return QString::number(exp.value());
}

QString Functions::log(const QString &str)
{
    std::pair<int, double> c{};

    if (!correctLogFunction(str, c)) return "NaN";

    if (c.first != -1)
    {
        QString cell{};
        --c.second;

        if (bee_cell_table->item(c.second, c.first)) cell = bee_cell_table->item(c.second, c.first)->text();

        if (isNumber(cell)) return QString::number(std::log(cell.toDouble()));
        return "NaN";
    }

    return QString::number(std::log(c.second));
}

bool Functions::isString(const QString &s)
{
    for (int j = 0; j < s.length(); ++j)
    {
        if (s[j] == QChar(39))
        {
            return true;
        }
    }

    return false;
}

void Functions::removeLetters(QString &condition)
{
    QString withoutLetters{};

    for (int j = 0; j < condition.length(); ++j)
    {
        if (condition[j] == QChar(39))
        {
            ++j;
            while (condition[j].isLetter())
            {
                withoutLetters += QString::number(condition[j].unicode());
                if (j >= condition.length() - 1) break;
                ++j;
            }
            ++j;
            if (j >= condition.length() - 1) continue;
        }

        withoutLetters += condition[j];
    }

    condition = withoutLetters;
}

bool Functions::checkCondition(const QString &condition)
{
    for (int j = 0; j < condition.length(); ++j)
    {
        if (condition[j] == QChar(39))
        {
            ++j;

            while (condition[j] != QChar(39))
            {
                if (j++ >= condition.length() - 1) return false;
            }

            if (j++ >= condition.length() - 1) continue;
        }

        if (condition[j] != '=' && condition[j] != ' ' && condition[j] != '!') return false;
    }

    return true;
}

bool Functions::getString(QString &buffer, QString &condition)
{
    QString cell{};

    int x = 0;
    int y = 0;
    int nums = 1;

    buffer.replace("TRUE", "1");
    buffer.replace("FALSE", "0");

    for (int j = 0; j < buffer.length(); ++j)
    {
        if (buffer[j] == '"')
        {
            condition += buffer[j];
            ++j;
            if (j >= buffer.length() - 1) return false;
            while (buffer[j] != '"') { condition += buffer[j]; if (j++ == buffer.length() - 1) return false; }
        }

        if (buffer[j].isLetter())
        {
            if (j >= buffer.length() - 1) return false;

            x = buffer[j].unicode() - 65;

            if (!buffer[++j].isNumber()) return false;

            while (buffer[j].isNumber())
            {
                y += nums * (buffer[j].unicode() - 48);

                nums *= 10;

                ++j;

                if (j >= buffer.length() - 1) break;
            }

            nums = 1;

            --y;

            if (bee_cell_table->item(y, x)) { cell = bee_cell_table->item(y, x)->text(); }

            y = 0;

            if (isNumber(cell)) { condition += cell; }
            else { condition += QChar(39) + cell.toUpper() + QChar(39); }

            if (j >= buffer.length() - 1) continue;
        }

        condition += buffer[j];
    }

    condition.replace('"', QChar(39));

    return true;
}

bool Functions::getExpression(QString &str, QString &out)
{
    QString buffer{};
    int row = 0;
    int column = 0;
    int nums = 1;

    for (int i = 0; i < str.length(); ++i)
    {
        if (str[i].isLetter())
        {
            column = str[i].unicode() - 65;
            if (i == str.length() - 1) return false;
            if (!str[++i].isNumber()) return false;

            while (i < str.length())
            {
                if (!str[i].isNumber()) break;

                row += nums * (str[i].unicode() - 48);
                nums *= 10;
                ++i;
            }

            nums = 1;

            --row;

            if (bee_cell_table->item(row, column)) buffer = bee_cell_table->item(row, column)->text();

            row = 0;

            if (!isNumber(buffer)) return false;

            out += buffer;

            buffer.clear();
        }

        if (i < str.length()) out += str[i];
    }

    return true;
}

bool Functions::getDate(const QString &date, int &day, int &month, int &year)
{
    QString buffer{};
    int i = 0;

    if (date.count('.') != 2) return false;

    while (date[i] != '.')
    {
        if (!date[i].isDigit()) { return false; }

        buffer += date[i];

        ++i;
    }

    ++i;

    day = buffer.toInt();
    buffer.clear();

    while (date[i] != '.')
    {
        if (!date[i].isDigit()) return false;

        buffer += date[i];

        ++i;
    }

    ++i;

    month = buffer.toInt();
    buffer.clear();

    while (i < date.length())
    {

        if (!date[i].isDigit()) return false;

        buffer += date[i];

        ++i;
    }

    year = buffer.toInt();

    return true;
}

bool Functions::isCorrectDate(const int &day, const int &month, const int &year)
{
    if (day < 1 || day > 31) return false;

    if (month < 1 || month > 12) return false;

    if (year < 1 || year > 9999) return false;

    if (month == 2)
    {
        if (( (!(year % 4)) && (year % 100) ) || ( !(year % 400) ))
        {
            if (day > 29) return false;
        }
        else
        {
            if (day > 28) return false;
        }
    }
    else
    {
        if (day > 30 + (month + (month / 8)) % 2) return false;
    }

    return true;
}

void Functions::cellChanged(const int& row, const int& column)
{
    if (std::make_pair(row, column) != previousClickedCell)
    {
        QString currentText = bee_cell_table->item(row, column)->text();
        currentText.replace(" ", "");
        if (currentText[0] == '=')
        {
            QString newEvaluation = evaluation(currentText);

            bee_cell_table->item(row, column)->setText(evaluation(currentText));

            if (currentText != newEvaluation)
            {
                expressions.erase(std::make_pair(row, column));

                expressions.insert(std::make_pair(std::make_pair(row, column),
                                                  std::make_pair(std::make_pair(row, column), currentText)));
            }
        }
        else
        {
            expressions.erase(std::make_pair(row, column));

            for (auto it : expressions)
            {
                if (it.first != std::make_pair(row, column))
                {
                    bee_cell_table->item(it.first.first, it.first.second)->setText(it.second.second);
                }
            }
        }
    }
}

void Functions::cellClicked(const int &row, const int &column)
{
    auto it = expressions.find(std::make_pair(row, column));

    if ((previousClickedCell != std::make_pair(-1, -1)) && (std::make_pair(row, column) != previousClickedCell))
    {
        int r = previousClickedCell.first;
        int c = previousClickedCell.second;

        previousClickedCell = std::make_pair(-1, -1);

        cellChanged(r, c);
    }

    if (it != expressions.end())
    {
        previousClickedCell = std::make_pair(row, column);

        bee_cell_table->item(row, column)->setText(it->second.second);
    }
    else previousClickedCell = std::make_pair(-1, -1);
}
