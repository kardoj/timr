#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>

#include "configuration.h"
#include "project.h"
#include "tree.h"

const string Configuration::DATE_PARAM_KEY = "--date";
const string Configuration::DATE_PARAM_KEY_SHORT = "-d";
const string Configuration::PROJECT_PARAM_KEY = "--project";
const string Configuration::PROJECT_PARAM_KEY_SHORT = "-p";

Configuration::Configuration()
{
    read_config();
}

Configuration::~Configuration() {}

void Configuration::read_config()
{
    FILE *fp;
    char row[MAX_CONFIG_ROW_LENGTH];
    string row_str, key, value;

    fp = fopen(Tree::CONFIG_FILE, "r");
    if (fp != NULL)
    {
        while(!feof(fp))
        {
            if (fgets(row, MAX_CONFIG_ROW_LENGTH, fp) == NULL) break;
            row_str = string(row);
            int split_pos = row_str.find_first_of("=");
            if (split_pos == -1)
            {
                cout << "A line in the configuration file is missing an equals sign.";
                break;
            }
            else
            {
                key = row_str.substr(0, split_pos);
                value = row_str.substr(split_pos + 1);

                if (key.compare(string("date")) == 0)
                {
                    set_date(value);
                }
                else if (key.compare(string("project_id")) == 0)
                {
                    set_project_id(atoi(value.c_str()));
                }
            }
        }
        fclose(fp);
    }
    else
    {
        cout << "There was a problem reading the configuration file.";
    }
}

bool Configuration::write_config()
{
    FILE *fp;
    fp = fopen(Tree::CONFIG_FILE, "w");
    char row[MAX_CONFIG_ROW_LENGTH];
    if (fp != NULL)
    {
        // Write all the params one by one
        sprintf(row, "date=%s\n", this->dte.c_str());
        fputs(row, fp);

        sprintf(row, "project_id=%d\n", this->project_id);
        fputs(row, fp);

        fclose(fp);
        return true;
    }
    else
    {
        cout << "There was a problem opening the configuration file.";
        return false;
    }
}

bool Configuration::set_from_param(string key, string value)
{
    if (key.compare(DATE_PARAM_KEY) == 0 || key.compare(DATE_PARAM_KEY_SHORT) == 0)
    {
        set_date(value);
        return true;
    }
    else if (key.compare(PROJECT_PARAM_KEY) == 0 || key.compare(PROJECT_PARAM_KEY_SHORT) == 0)
    {
        return set_project_id(atoi(value.c_str()));
    }
    else
    {
        cout << "Unrecognized key \"" << key<< "\" was ignored." << endl;
        return false;
    }
}

int Configuration::get_project_id()
{
    return this->project_id;
}

bool Configuration::set_project_id(int project_id)
{
    if (Project::exists(project_id))
    {
        this->project_id = project_id;
        return true;
    }
    else
    {
        cout << "Unknown project id \"" << project_id << "\" ignored." << endl;
        return false;
    }
}

string Configuration::get_date()
{
    return this->dte;
}

void Configuration::set_date(string dte)
{
    // TODO: Check if the supplied date is a valid date
    this->dte = dte;
}
