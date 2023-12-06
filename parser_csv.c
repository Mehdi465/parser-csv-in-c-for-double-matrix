#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser_csv.h"
#include "matrix/matrix.h"
#include "matrix/ops.h"
#include <math.h>

#define LINE_MAX_SIZE 1000

// Return the number of rows in a csv file fot create a matrix
int countRows(char * file_name){
    FILE *fd = fopen(file_name,"r");
    int n_rows = 1;
    char c;

    // if cannot open the file
    if (fd == NULL){
        fprintf(stderr, "Error opening file: %s\n", file_name);
        exit(-1);
    }

    // loop over the character.
    while ((c = fgetc(fd)) != EOF) {
        if (c == '\n'){
            n_rows ++;            
        }
    }

    fclose(fd);
    return n_rows;

}

// Return the number of lines in a csv file fot create a matrix
int countColumns(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", file_name);
        exit(-1); 
    }

    // I supose that the first cannot be greater than 1000
    char line[1000]; 
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        exit(-1); // Return -1 if unable to read the first line
    }

    fclose(file);

    // Count the number of delimiters (commas) in the first line
    int columnCount = 1; // Initialize to 1 because there is at least one column
    for (size_t i = 0; i < strlen(line); ++i) {
        if (line[i] == ',') {
            columnCount++;
        }
    }

    return columnCount;
}

// Remove whitespaces from a char*. example : |  "Modele"   , -> "Modele",
void removeWhitespaces(char * word){
    char *str = word;

    char *write = str, *read = str;
    do {
    if ((*read != ' ') ){
        *write++ = *read;
        }
    /* if (*read != '\"'){
        *write++ = *read;
        } */
    } while (*read++);

}


void removeQuotaMarks(char * word){
    char *str = word;

    char *write = str, *read = str;
    do {
    if ((*read != '\"') ){
        *write++ = *read;
        }
    } while (*read++);

}

// To delimite with the coma.
char** str_split(char* a_str, const char a_delim){

    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


char ** getLabels(char * file_name){

    int n_cols = countColumns(file_name);

    FILE *fd = fopen(file_name,"r");


    if (fd == NULL){
        perror("Could not find the document");
        exit(-1);
    }

    //first line can not exceed 1000 char
    char buffer[LINE_MAX_SIZE];
    
    char ** res = (char **) malloc(n_cols*sizeof(char));

    if (res == NULL){
        perror("Malloc in getLabels failed");
        exit(-1);
    }

    fgets(buffer,LINE_MAX_SIZE,fd);

    removeQuotaMarks(buffer);

    res = str_split(buffer,',');

    if (!res){
        perror("cannot split the first line in getLabels");
        exit(-1);
    }

    fclose(fd);

    return res;
}

int isDouble(const char *str){
    char *endptr;
    strtod(str,&endptr);

    return (*endptr == '\0' && endptr != str);
}


acpData * parser_csv(char * file_name){
    FILE *fd = fopen(file_name,"r");
    if (fd == NULL){
        perror("Could not find the file");
        exit(-1);
    }

    char buffer[LINE_MAX_SIZE];
    acpData * res = malloc(sizeof(*res));
    int n_cols = countColumns(file_name);
    int n_rows = countRows(file_name);

    res->labels = getLabels(file_name);
    res->m = matrix_create(n_rows,n_cols);
    
    int index = 1;
    char ** test;
    while(fgets(buffer,LINE_MAX_SIZE,fd)){
        int index_line = 0;
        removeWhitespaces(buffer);
        removeQuotaMarks(buffer);
        printf("line %d : %s",index,buffer);
        test = str_split(buffer,','); 

        while(*test[index_line] != '\n'){
            int k = 0;
            printf("%f \n",res->m->entries[index_line][k]);
            if(isDouble(test[index_line])){
                char * ptr;
                res->m->entries[index_line][k] = strtod(test[index_line],&ptr);

                printf("%f \n",res->m->entries[index_line][k]);
                k++;

                ptr = NULL;
                free(ptr);
            }
            index_line ++;
        }
        
        index ++;
        
    } 

    fclose(fd);
    test = NULL;
    fd = NULL;
    free(fd);
    free(test);

    return res;
}






// test Part ---------------------------------------------------------

void test_isDouble(){
    char val0[10] = "1";
    char val1[10] = "14131";
    char val2[10] = "-156";
    char val3[10] = "0";
    char val4[10] = "Non";
    char val5[10] = "o";

    printf("val0 : %d \n",isDouble(val0));
    printf("val1 : %d \n",isDouble(val1));
    printf("val2 : %d \n",isDouble(val2));
    printf("val3 : %d \n",isDouble(val3));
    printf("val4 : %d \n",isDouble(val4));
    printf("val5 : %d \n",isDouble(val5));

}

void test_whitespaces(){

    printf("---------test whitespaces------------- \n");
    char s[20] = "  \" \"tes\"    t ";
    removeWhitespaces(s);
    printf("%s \n",s);
}


void test_quotaMarks(){
    printf("---------test Quotations marks------------- \n");

    char s[20] = "\"  tes\"    t \"";
    removeQuotaMarks(s);
    printf("%s \n",s);

    FILE *fd = fopen("data/autos_acp.csv","r");

    if (fd == NULL){
        perror("Could not find the file");
        exit(-1);
    }

    char buffer[256];
    char *data;
    fgets(buffer,sizeof(buffer),fd);
    data = strtok(buffer,",");

    removeQuotaMarks(data);

    printf(" first element : %s \n",data);

    data = strtok(NULL,",");

    printf(" second element : %s \n",data);

    data = NULL;
    free(data);

    fclose(fd);

    

}



void test_getLabels(){

    printf("---------test getLabels------------- \n");

    char ** res = getLabels("data/autos_acp.csv");

    printf("premier element : %s", res[0]);
    
    for (int k = 0; *(res + k);k++){
        printf("%s \n",res[k]);
        free(*(res + k));
    }

    res = NULL;

    free(res);

}

void test_parser(){
    acpData * test = parser_csv("data/autos_acp.csv");

    //printf("%s",test->labels[0]);

    test = NULL;
    free(test);
}





int main(){

    // Test part
    /* int n_rows = countRows("data/autos_acp.csv");
    int n_cols = countColumns("data/autos_acp.csv");
    printf("%d\n",n_rows);
    printf("%d\n",n_cols);


    // -----------------------------------
    test_whitespaces();

    test_quotaMarks();
    */
    // -----------------------------------
    //test_getLabels();
    //test_isDouble();

    test_parser();


    return 0;

}