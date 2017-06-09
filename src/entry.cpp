#include <iostream>
#include "date.h"
#include "entry.h"
#include "period.h"
#include "tree.h"

using namespace std;

Entry::Entry() {}
Entry::~Entry() {}

bool Entry::add(string start_time, string end_time, string description, string dte, int project_id)
{
    Period period = Period(start_time, end_time);

    if (!period.is_valid())
    {
        cout << "Invalid time period entered. Nothing to do." << endl;
        return false;
    }

    Date d = Date(dte);
    int y = d.get_year();
    int m = d.get_month();
    int dy = d.get_day();
    int wdy = d.get_wday();

    char year[5], month[3], day[3], wday[2];
    sprintf(year, "%d", y);
    sprintf(month, "%d", m);
    sprintf(day, "%d", dy);
    sprintf(wday, "%d", wdy);

    string year_str = string(year);
    string mon_str = string(month);

    Tree::ensure_year_dir(year_str);
    Tree::ensure_month_dir(year_str, mon_str);

    string datetime = Date::current_date_with_time();
    string path = string(Tree::ENTRIES_DIR) + "/" + year_str + "/" + mon_str + "/" + string(day) + ".txt";

    FILE *fp;
    int max_id_length = 13;
    char id[max_id_length];

    fp = fopen(Tree::ENTRIES_ID_FILE, "r");
    if (fp != NULL)
    {
        fgets(id, max_id_length, fp);
        fclose(fp);
    }
    else
    {
        cout << "There was a problem opening entries id file. Nothing to do." << endl;
        return false;
    }

    fp = fopen(path.c_str(), "a");

    if (fp != NULL)
    {
        char p_id[max_id_length], minutes[max_id_length];
        sprintf(p_id, "%d", project_id);
        sprintf(minutes, "%d", period.minutes());
        string entry =
            string(id) + " " +
            dte + " " +
            wday + " " +
            string(p_id) + " " +
            start_time + " " +
            end_time + " " +
            string(minutes) + " " +
            "\"" + description + "\" " +
            datetime + " " +
            datetime + "\n";
        fputs(entry.c_str(), fp);
        fclose(fp);
    }
    else
    {
        cout << "Something went wrong while opening the entry file." << endl;
        return false;
    }

    fp = fopen(Tree::ENTRIES_ID_FILE, "w");
    if (fp != NULL)
    {
        int id_int = atoi(id) + 1;
        sprintf(id, "%d", id_int);
        fputs(id, fp);
        fclose(fp);
    }
    else
    {
        return false;
    }

    return true;
}