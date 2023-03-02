/*
    Generic function which checks if enough space is in the watchtower_list
    watchtower_t array and calls realloc if necessary.

    **watchtower_list: A pointer to a watchtower_t array which the function is
        supposed to check for

    *num_watchtowers: Pointer to an int variable which represents the number of
        watchtowers in the watchtower_list array

    *cur_max_num_watchtowers: Pointer to an int variable which represents the
        current max number of watchtowers the watchtower_list array can store

    Returns: none
 */
void realloc_check_watchtowers(watchtower_t **watchtower_list, 
                               int *num_watchtowers, 
                               int *cur_max_num_watchtowers);

/*
    Generic function which checks if enough space is in the vertices vertex_t
    array and calls realloc is necessary.

    ***vertices: A pointer to a pointer to a vertex_t array which stores all
        the vertices in the DCEL

    *num_vertices: A pointer to an int variable which represents the number of
        vertices in the vertices array

    *cur_max_num_vertices: Pointer to an int variable which represents the
        current max number of vertices the vertices array can hold

    Returns: none
 */
void realloc_check_vertices(vertex_t ***vertices, int *num_vertices,
                            int *cur_max_num_vertices);

/*

    Generic function which checks if enough space is in the vertices vertex_t
    array and calls realloc is necessary.

    **edges: A pointer to an edge_t array which stores all the edges in the DCEL

    *num_edges: A pointer to an int variable which represents the number of
        edges in the edges array

    *cur_max_num_edges: A pointer to an int variable which represents the
        current max number of edges the edges array can hold

    Returns: none
 */
void realloc_check_edges(edge_t **edges, int *num_edges, 
                         int *cur_max_num_edges);

/*
    Generic function which checks if enough space is in the faces face_t
    array and calls realloc is necessary.

    **faces: A pointer to a face_t array which stores all the faces in the DCEL

    *num_faces: A pointer to an int variable which represents the number of
        faces in the faces array

    *cur_max_num_faces: A pointer to an int variable which represents the
        current max number of faces the faces array can hold

    Returns: none
 */
void realloc_check_faces(face_t **faces, int *num_faces, 
                         int *cur_max_num_faces);

/*
    Function which frees the dynamically allocated strings in each watchtower_t
    variable in the watchtower_list array and then frees array itself as well.

    **watchtower_list: A pointer to a watchtower_t array which the function is
        supposed to check for

    *num_watchtowers: Pointer to an int variable which represents the number of
        watchtowers in the watchtower_list array

    Returns: none
 */
void free_csv(watchtower_t **watchtower_list, int *num_watchtowers);

/*
    Goes through all the edges in the edges array and frees all it's half_edges
    and then frees the edges array itself.

    **edges: A pointer to an edge_t array which stores all the edges in the DCEL

    *num_edges: A pointer to an int variable which represents the number of
        edges in the edges array

    Returns: none
 */
void free_edges(edge_t **edges, int num_edges);

/*
    Goes through all the vertices in the vertices array and frees all of them
    and then free the vertices array itself.

    ***vertices: A pointer to a pointer to a vertex_t array which stores all
        the vertices in the DCEL

    *num_vertices: A pointer to an int variable which represents the number of
        vertices in the vertices array

    Returns: none
 */
void free_vertices(vertex_t ***vertices, int num_vertices);
