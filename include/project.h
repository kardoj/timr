#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include "configuration.h"
#include "cruditem.h"

class Project : public CrudItem
{
    public:
        static const char *const DEFAULT_PROJECT_NAME;
        static const int MAX_PROJECT_ROW_LENGTH = 300;

        Project();
        virtual ~Project();
        static bool add(const char name[], const std::string datetime);
        static bool exists(const std::string project_id_or_name, int &project_id);
        static bool list(const int selected_project_id);

        static std::string get_next_id_and_increment(const std::string path, const std::string file_open_error);
};

#endif // PROJECT_H
