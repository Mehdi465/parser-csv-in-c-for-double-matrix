#include "matrix/matrix.h"
#include "matrix/ops.h"

typedef struct {
    Matrix * m;
    char ** names;
    char ** labels;
} acpData;


int countRows(char * file_name);
int countColumns(const char *file_name);
void removeWhitespaces(char * word);
void removeQuotaMarks(char * word);
char** str_split(char* a_str, const char a_delim);
char ** getLabels(char * first_line);
acpData * parser_csv(char * file_name);