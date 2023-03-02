/*
    Reads in the CSV data from the given filename argument and stores it in the
    	watchtower_t array

    **watchtower_list: Pointer to a watchtower_t array which stores the
    	watchtower data from the CSV

    *filename: A char array (string) which represents the filename of the CSV
    	to read the data from

    *num_watchtowers: Pointer to an int variable which represents the number of
        watchtowers in thewatchtower_list array 

    *cur_max_num_watchtowers: Pointer to an int variable which represents the
        current max number of watchtowers the watchtower_list array can store
        

    Returns: none
*/
void read_CSV(watchtower_t **watchtower_list, char *filename,
              int *num_watchtowers, int *cur_max_num_watchtowers);

/*
    For each face stored in the faces array, loop through all the watchtowers
    and check whether it belongs in the face. Outputs the watchtower
    information and population served according to the formatting described
    by the specification

    *watchtower_list: A watchtower_t array which stores the watchtower data
        from the CSV

    num_watchtowers: An int variable which represents the number of watchtowers
        in the watchtower_list array 

    *faces: A face_t array which stores all the faces in the DCEL
        (each face contains a pointer to a half edge in the face)

    num_faces: An int variable which represents the number of faces in the DCEL
        and face array 

    filename_to_write_to: A char array (string) which represents the filename
        of the file to write to

   Returns: none
*/
void perform_watchtower_in_face_check(watchtower_t *watchtower_list,
                                      int num_watchtowers, face_t *faces,
                                      int num_faces,
                                      char *filename_to_write_to);
/*
    Function that goes through all the half edges in a given face and determines
    if a watchtower is in the face. If it does, then return 1, if it doesn't 
    then return 0.

    cur_watchtower: A watchtower_t variable which is the current watchtower of
        interest

    cur_face: A face_t variable which is the current face of interest

    Returns: 1 if the watchtower is in the face; 0 otherwise
*/
int is_watchtower_in_face(watchtower_t cur_watchtower, face_t cur_face);
