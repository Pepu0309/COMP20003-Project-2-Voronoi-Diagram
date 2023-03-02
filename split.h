/*
    Primary splitting function which reads in the edge number arguments to split
    on and calls other functions to assist in conducting the whole splitting
    process.

    ***vertices: A pointer to a pointer to a vertex_t array which stores all
        the vertices in the DCEL

    *num_vertices: A pointer to an int variable which represents the number of
        vertices in the vertices array

    *cur_max_num_vertices: Pointer to an int variable which represents the
        current max number of vertices the vertices array can hold

    **edges: A pointer to an edge_t array which stores all the edges in the DCEL

    *num_edges: A pointer to an int variable which represents the number of
        edges in the edges array

    *cur_max_num_edges: A pointer to an int variable which represents the
        current max number of edges the edges array can hold

    **faces: A pointer to a face_t array which stores all the faces in the DCEL

    *num_faces: A pointer to an int variable which represents the number of
        faces in the faces array

    *cur_max_num_faces: A pointer to an int variable which represents the
        current max number of faces the faces array can hold

    Returns: none
*/
void split_polygon(vertex_t ***vertices, int *num_vertices,
                   int *cur_max_num_vertices, edge_t **edges, int *num_edges,
                   int *cur_max_num_edges, face_t **faces, int *num_faces,
                   int *cur_max_num_faces);

/*
    Secondary splitting function which executes the steps of the splitting
    process and calls other functions to accomplish it.

    **edges: A pointer to an edge_t array which stores all the edges in the DCEL

    *num_edges: A pointer to an int variable which represents the number of
        edges in the edges array

    *cur_max_num_edges: A pointer to an int variable which represents the
        current max number of edges the edges array can hold

    *half_edge_of_interest1: Pointer to the first half edge argument given
        from reading in the contents of the file. The alias of this
        half edge in the program is also "start_half_edge"

    *half_edge_of_interest2: Pointer to the second half edge argument given
        from reading in the contents of the file. The alias of this
        half edge in the program is also "end_half_edge"

    **vertices: A pointer to a vertex_t array which stores all
        the vertices in the DCEL (as this function does not modify the vertices)

    *num_vertices: A pointer to an int variable which represents the number of
        vertices in the vertices array

    **faces: A pointer to a face_t array which stores all the faces in the DCEL

    *num_faces: A pointer to an int variable which represents the number of
        faces in the faces array

    *cur_max_num_faces: A pointer to an int variable which represents the
        current max number of faces the faces array can hold

    Returns: none
*/
void conduct_splitting(edge_t **edges, int *num_edges, int *cur_max_num_edges,
                       half_edge_t *half_edge_of_interest1,
                       half_edge_t *half_edge_of_interest2, vertex_t **vertices,
                       int *num_vertices, face_t **faces, int *num_faces,
                       int *cur_max_num_faces);

/*
    Tertiary splitting function which handles the "DCEL split with pairs" edge
    cases of the splitting process.

    *half_edge_of_interest_pair: A pointer to the pair of the half edge of
        interest that is associated with the "DCEL split with pairs" edge case

    *one_of_new_half_edge_created_from_split: A pointer to the one of the new
        half edges created from the split (that isn't the one made by joining
        the 2 midpoints of the edges)

    *midpoint: A pointer to a vertex which represents a midpoint which is
        intended to be the new start or end vertex of the above 2 half edges
        depending on which edge case gets triggered

    start_or_end: An int variable which represents which edge case is being
        performed, used in conjunction with the START and END constants defined
        in the voronoi1.h header file. START used for edge case 1 (where the
        start_half_edge a.k.a. half_edge_of_interest1 has a pair) and END
        for vice versa

    Returns: none
 */
void conduct_split_with_pairs(half_edge_t *half_edge_of_interest_pair,
                              half_edge_t
                              *one_of_new_half_edge_created_from_split,
                              vertex_t *midpoint, int start_or_end);

/*
    Function which changes all the half edges to a given face specified by an
    int argument.

    *initial_half_edge: Pointer to an initial half edge of which the function
        should start the process on and loop through using the next pointer

    x: An int variable representing the face number to change each half edge to

    **faces: A pointer to a face_t array which stores all the faces in the DCEL

    *num_faces: A pointer to an int variable which represents the number of
        faces in the faces array

    *cur_max_num_faces: A pointer to an int variable which represents the
        current max number of faces the faces array can hold

    Returns: none
*/
void change_all_half_edge_to_face_x(half_edge_t *initial_half_edge, int x,
                                    face_t **faces, int *num_faces,
                                    int *cur_max_num_faces);

