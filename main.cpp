#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "configuration.h"
#include "project.h"
#include "tree.h"

using namespace std;

// All supported commands
const string ADD = "add";
const string ADD_PROJECT = "addproject";
const string DATE = "date";
const string INIT = "init";
const string PROJECTS = "projects";
const string SET = "set";

void set_date_and_time(char *dte, char *tme)
{
    time_t t = time(0);
    tm *local = localtime(&t);
    sprintf(dte, "%d-%d-%d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    sprintf(tme, "%d:%d:%d", local->tm_hour, local->tm_min, local->tm_sec);
}

bool command_is(string command, char *argv[])
{
    return command.compare(argv[1]) == 0;
}

int main(int argc, char *argv[])
{
    char dte[11], tme[9];
    Configuration configuration;

    setlocale(LC_CTYPE, "");

    // The first argument is always the program name
    if (argc < 2)
    {
        cout << "No command entered, nothing to do." << endl;
        return 0;
    }

    if (!command_is(INIT, argv) && !Tree::is_eon_dir())
    {
        cout << "Directory is not an eon directory.\nRun eon init to make it one." << endl;
        return 0;
    }

    set_date_and_time(dte, tme);
    if (!command_is(INIT, argv)) configuration = Configuration();
    if (command_is(ADD, argv))
    {
        cout << "Running the add command" << endl;
    }
    else if (command_is(INIT, argv))
    {
        if (Tree::is_eon_dir())
        {
            cout << "Directory is already an eon directory." << endl;
        }
        else
        {
            if (Tree::init(dte, tme)) {
                cout << "Created a new eon directory." << endl;
            }
            else
            {
                cout << "Something went wrong while setting up eon." << endl;
            }
        }
    }
    else if (command_is(ADD_PROJECT, argv))
    {
        if (argc < 3)
        {
            cout << "No project name entered, nothing to do." << endl;
        }
        else
        {
            if (Project::add(argv[2], dte, tme))
            {
                cout << "New project \"" << argv[2] << "\" was added." << endl;
            }
            else
            {
                cout << "There was a problem adding a new project." << endl;
            }
        }
    }
    else if (command_is(PROJECTS, argv))
    {
        Project::list_projects(configuration);
    }
    else if (command_is(DATE, argv))
    {
        cout << "Currently set date: " << configuration.get_date() << endl;
    }
    else if (command_is(SET, argv))
    {
        if (argc == 2)
        {
            cout << "No parameters supplied to set. Nothing to do." << endl;
            return 0;
        }

        vector<string> keys, values;

        unsigned next, argc_u;
        argc_u = argc;
        for(unsigned i = 2; i < argc_u; i++)
        {
            next = i + 1;
            if (next < argc_u)
            {
                keys.push_back(argv[i]);
                values.push_back(argv[next]);
                i = next;
            }
            else
            {
                cout << "Parsing the parameters failed. Nothing to do." << endl;
                return 0;
            }
        }

        unsigned key_count = keys.size();
        unsigned updated = 0;

        for (unsigned i = 0; i < key_count; i++)
        {
            if (configuration.set_from_param(keys.at(i), values.at(i))) updated++;
        }

        if (updated > 0 && configuration.write())
        {
            cout << "Found " << key_count << ", successfully updated " << updated << " key(s)." << endl;
        }
    }
    else
    {
        cout << "Unknown command \"" << argv[1] << "\". Nothing to do." << endl;
    }

    return 0;
}
