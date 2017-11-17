#include <cal.h>
#include <time.h>

const char m[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                       {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

int is_year(int year)
{
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
        return 1;
    return 0;
}

int base_days(void)
{
    return (time(NULL) / 60 / 60 + 8) / 24;
}

int now_year(void)
{
    int year = BASE_YEAR;
    int days = base_days();
    
    while (days >= 365 + is_year(year))
    {
        days = days - 365 - is_year(year++); 
    }
    return year;
}

int year_count_days(int s, int e)
{
    int days = 0;

    while (s < e)
    {
        days += 365 + is_year(s++); 
    }
    return days;
}

int now_month(void)
{
    int month = 1;
    int days = base_days();
    int year = now_year();
    int y = is_year(year);

    days -= year_count_days(BASE_YEAR, year);

    while (days >= m[y][month - 1])
    {
        days -= m[y][month - 1];
        month++;
    }
    return month; 
}

int month_count_days(int year, int month)
{
    int days = 0;
    int y = is_year(year);

    while (month > 1)
    {
        days += m[y][--month];
    }
    return days;
}

int now_day(void)
{
    int year = now_year();

    return base_days() - year_count_days(BASE_YEAR, year) - month_count_days(year, now_month());
}

int get_week(int year, int month, int day)
{
    int days = year_count_days(BASE_YEAR, year) + month_count_days(year, month) + day;

    days = days % 7;
    
    if (days > 3)
        return days - 3;
    return days + 4;
}

void get_data(int year, int month, char (*data)[COL])
{
    int i, j, d = 1;
    int week, y = is_year(year);
    int prev, next = 1;

    week = get_week(year, month, 1) - 1;

    if (month == 1)
        prev = m[is_year(year - 1)][11] - week + 1;
    else
        prev = m[y][month - 2] - week + 1;

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++) 
        {
            if (i == 0)
            {
                if (j >= week)
                    data[i][j] = d++;
                else
                    data[i][j] = prev++;
            }
            else if (i != 0 && d <= m[y][month - 1])
                data[i][j] = d++;
            else
                data[i][j] = next++;
        }
    }
}
