#include "voronoi1.h"
#include "watchtowers_io.h"
#include "memory_funcs.h"
#include "geometry.h"
/*
  Opens the file of the given filename argument, consumes the initial header row
  and then loops through each row in the CSV file and extracts row entries based
  on the type and stores it in the watchtower_t array. The array is initially
  set to size 5 and the realloc_check_watchtowers function is called to allocate
  extra memory if necessary. All char* arrays (strings) are dynamically
  allocated in order to minimise use of space.
*/
void read_CSV(watchtower_t **watchtower_list, char *filename,
              int *num_watchtowers, int *cur_max_num_watchtowers) {

    char *line = NULL;
    size_t lineBufferLength = MAX_LINE_LENGTH;

    char *delimiter = COMMA_DELIM;
    char *row_entry;

    *watchtower_list = (watchtower_t *) malloc((*cur_max_num_watchtowers) *
            sizeof(watchtower_t ));

    /* Open the file for reading */
    FILE *fp = fopen(filename, "r");

    /* Consumes the header row */
    getline(&line, &lineBufferLength, fp);

    /* Read the rest of the file until end of file */
    while(getline(&line, &lineBufferLength, fp) != EOF) {

        /* Checks if there is enough space before reading and storing
           in each row of data */
        realloc_check_watchtowers(watchtower_list, num_watchtowers,
                                  cur_max_num_watchtowers);

        /* Gets the first part of the line (first row entry) delimited by a
           comma and then stores it as the ID into a dynamically allocated
           char* array which is part of a watchtower_t struct and stores it in
           the watchtower_t array */
        row_entry = strtok(line, delimiter);
        (*watchtower_list)[*num_watchtowers].watchtower_ID =
                (char *) malloc ((strlen(row_entry) + SPACE_FOR_NULL_CHAR) 
                                 * sizeof(char));
        assert((*watchtower_list)[*num_watchtowers].watchtower_ID != NULL);
        strcpy((*watchtower_list)[*num_watchtowers].watchtower_ID, row_entry);

        /* Same as above but for postcode */
        row_entry = strtok(NULL, delimiter);
        (*watchtower_list)[*num_watchtowers].postcode =
                (char *) malloc ((strlen(row_entry) + SPACE_FOR_NULL_CHAR) 
                                 * sizeof(char));
        assert((*watchtower_list)[*num_watchtowers].postcode != NULL);
        strcpy((*watchtower_list)[*num_watchtowers].postcode, row_entry);

        /* Same process to delimit the row entry but calls atoi to convert the
         * string to an int instead and dynamic allocation is not neccessary */
        row_entry = strtok(NULL, delimiter);
        (*watchtower_list)[*num_watchtowers].population_served =
                atoi(row_entry);

        /* Similar as the initial one but for contact name */
        row_entry = strtok(NULL, delimiter);
        (*watchtower_list)[*num_watchtowers].contact_name =
                (char *) malloc ((strlen(row_entry) + SPACE_FOR_NULL_CHAR) 
                                 * sizeof(char));
        assert((*watchtower_list)[*num_watchtowers].contact_name != NULL);
        strcpy((*watchtower_list)[*num_watchtowers].contact_name, row_entry);

        /* Declare a 2D char array to use the strtod function,
         * nothing is actually stored here */
        char **discard_ptr = NULL;

        /* Same process to get the row entry for both but uses strtod
         * to convert the row entry into a double */
        row_entry = strtok(NULL, delimiter);
        (*watchtower_list)[*num_watchtowers].x_coord = strtod(row_entry,
                                                              discard_ptr);

        row_entry = strtok(NULL, delimiter);
        (*watchtower_list)[*num_watchtowers].y_coord = strtod(row_entry,
                                                              discard_ptr);

        (*num_watchtowers)++;
        fflush(stdout);

    }

    /* Close the file and free the line pointer since we're done */
    fclose(fp);
    free(line);
}

/*
    For each face stored in the faces array, loop through all the watchtowers
    and calls the is_watchtower_in_face function to check whether the watchtower
    is in a given face. If it is, output the watchtower in proper formatting as
    described by the specification. The population served of each face is
    stored temporarily in an array and then outputted at the end to suit
    the specification.
*/
void perform_watchtower_in_face_check(watchtower_t *watchtower_list,
                                      int num_watchtowers, face_t *faces,
                                      int num_faces,
                                      char *filename_to_write_to) {

    watchtower_t cur_watchtower;

    /* Declare an int array to store the population served for each face */
    int tmp_population_served_by_face_holder[num_faces];

    FILE* fp = fopen(filename_to_write_to, "w");

    for(int i = 0; i < num_faces; i++) {
        fprintf(fp, "%d\n", i);
        int total_population_served_in_face = 0;

        for(int j = 0; j < num_watchtowers; j++) {
            cur_watchtower = watchtower_list[j];

            if(is_watchtower_in_face(cur_watchtower, faces[i])) {
                fprintf(fp, "Watchtower ID: %s, Postcode: %s, "
                            "Population Served: %d, "
                            "Watchtower Point of Contact Name: "
                            "%s, x: %lf, y: %lf\n",
                            cur_watchtower.watchtower_ID, 
                            cur_watchtower.postcode,
                            cur_watchtower.population_served,
                            cur_watchtower.contact_name,
                            cur_watchtower.x_coord, cur_watchtower.y_coord);

                total_population_served_in_face +=
                        watchtower_list[j].population_served;
            }

        }

        tmp_population_served_by_face_holder[i] =
                total_population_served_in_face;
    }

    for(int k = 0; k < num_faces; k++) {
        fprintf(fp, "Face %d population served: %d\n", k,
                tmp_population_served_by_face_holder[k]);
    }
    fclose(fp);
}

/*
    A function which goes through each half edge in a face by using the linked
    list behaviour of a face and checks whether a point is in each half edge by
    calling the is_point_in_half_edge function located in the geometry.c file.
    If a point is in all half edges, the point is determined to be in a face
    and the function returns 1; otherwise returns 0.
*/
int is_watchtower_in_face(watchtower_t cur_watchtower, face_t cur_face) {

    /* Assume the watchtower is in the face by default */
    int watchtower_is_in_face = BOOL_TRUE;

    /* Declare variables to store the initial half edge and the current
     * half edge so the do while loop knows when it's done with all the half
     * edges in the face */
    half_edge_t *initial_half_edge = cur_face.half_edge_in_face;
    half_edge_t *cur_half_edge = initial_half_edge;

    vertex_t cur_watchtower_position;
    cur_watchtower_position.x_pos = cur_watchtower.x_coord;
    cur_watchtower_position.y_pos = cur_watchtower.y_coord;

    /* Perform the check for whether the watchtower is in a given half edge
     * and if at any point false is returned, the watchtower is determined
     * to be not in the face and the loop is broken. Otherwise, if the loop
     * ends normally, then the function has gone through all the half edges
     * in the face and the watchtower is in the face. */
    do {
        if(!is_point_in_half_edge(cur_watchtower_position,
                                   *(cur_half_edge->start_vertex),
                                   *(cur_half_edge->end_vertex))){
            watchtower_is_in_face = BOOL_FALSE;
            break;
        }
        cur_half_edge = cur_half_edge->next;
    } while(cur_half_edge != initial_half_edge);

    return watchtower_is_in_face;
}

    
