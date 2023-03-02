#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define COL_NUM 6 /* Constant for number of columns in csv data */
#define ROW_DELIM "," /* The delimiter for the csv row entries */
#define INITIAL_MAX_ROW_NUM 5 /* Initial assumption of maximum row numbers for initial malloc call */

typedef struct watchtower {
    char *watchtower_ID;
    char *postcode;
    int population_served;
    char *contact_name;
    double x_coord;
    double y_coord;
} watchtower_t;

void read_CSV(watchtower_t *watchtowerList, char *filename, int *num_rows, int *cur_max_num_rows);

int main(int argc, char **argv) {
    
    char *filename = NULL;

    int num_rows = 0, cur_max_num_rows = INITIAL_MAX_ROW_NUM;
    

    watchtower_t *watchtowerList = (watchtower_t *) malloc(sizeof(watchtower_t ) * cur_max_num_rows);
    
    /* Obtain filename from the 1st command line argument given then read the data in */
    filename = argv[1];
    
    read_CSV(watchtowerList, filename, &num_rows, &cur_max_num_rows);
    
    return 0;
}

void read_CSV(watchtower_t *watchtowerList, char *filename, int *num_rows, int *cur_max_num_rows) {
    
    char *line = NULL;
    size_t lineBufferLength = 512;

    char *delimiter = ROW_DELIM;
    char *row_entry;

    /* Open the file for reading */
    FILE *fp = fopen(filename, "r");

    /* Consumes the header row */
    getline(&line, &lineBufferLength, fp);

    /* Read the rest of the file until end of file */    
    while(getline(&line, &lineBufferLength, fp) != EOF) {
                
        if(*num_rows == *cur_max_num_rows){
            *cur_max_num_rows *= 2;
            watchtowerList = (watchtower_t *) realloc(watchtowerList, (*cur_max_num_rows * sizeof(watchtower_t)));
        }

        row_entry = strtok(line, delimiter);	
        watchtowerList[*num_rows].watchtower_ID = (char *) malloc ((strlen(row_entry) + 1) * sizeof(char));
        assert(watchtowerList[*num_rows].watchtower_ID != NULL);
        strcpy(watchtowerList[*num_rows].watchtower_ID, row_entry);
    
        row_entry = strtok(NULL, delimiter);	
        watchtowerList[*num_rows].postcode = (char *) malloc ((strlen(row_entry) + 1) * sizeof(char));
        assert(watchtowerList[*num_rows].postcode != NULL);
        strcpy(watchtowerList[*num_rows].postcode, row_entry);

        row_entry = strtok(NULL, delimiter);
        watchtowerList[*num_rows].population_served = atoi(row_entry);           

        row_entry = strtok(NULL, delimiter);	
        watchtowerList[*num_rows].contact_name = (char *) malloc ((strlen(row_entry) + 1) * sizeof(char));
        assert(watchtowerList[*num_rows].contact_name != NULL);
        strcpy(watchtowerList[*num_rows].contact_name, row_entry);

        char **discard_ptr = NULL;
        row_entry = strtok(NULL, delimiter);
        watchtowerList[*num_rows].x_coord = strtod(row_entry, discard_ptr);
        
        row_entry = strtok(NULL, delimiter);
        watchtowerList[*num_rows].y_coord = strtod(row_entry, discard_ptr);
        
        (*num_rows)++;
        fflush(stdout);
        
    }    


}
