#include "voronoi1.h"
#include "geometry.h"
#include "split.h"
#include "memory_funcs.h"
/*
   Primary function for splitting a polygon. Reads the arguments needed for
   splitting and calls other functions to accomplish the splitting process.
   Details of the splitting process and the steps can be found inside the
   implementation of this function and the other function it calls with comments
   indicating the specific intention of the code.
*/
void split_polygon(vertex_t ***vertices, int *num_vertices,
                   int *cur_max_num_vertices, edge_t **edges, int *num_edges,
                   int *cur_max_num_edges, face_t **faces, int *num_faces,
                   int *cur_max_num_faces) {

    int split_edge_num1, split_edge_num2;
    vertex_t *tmp_vertex = (vertex_t *) malloc(sizeof(vertex_t));
    half_edge_t *half_edge_of_interest1;
    half_edge_t *half_edge_of_interest2;

    /* Reads in the 2 arguments of a line of splits one by one until
     * end of file */
    while(scanf("%d %d", &split_edge_num1, &split_edge_num2)!= EOF) {

        /* Assign some variables to determine our vertices of interest in
         * this split */
        vertex_t start_vertex_of_first_edge_to_split =
                *((*edges)[split_edge_num1].inner_half_edge->start_vertex);
        vertex_t end_vertex_of_first_edge_to_split =
                *((*edges)[split_edge_num1].inner_half_edge->end_vertex);

        vertex_t start_vertex_of_second_edge_to_split =
                *((*edges)[split_edge_num2].inner_half_edge->start_vertex);
        vertex_t end_vertex_of_second_edge_to_split =
                *((*edges)[split_edge_num2].inner_half_edge->end_vertex);

        /* Creates a vertex in the middle of the 2 vertices that connect the
         * edge given as an argument in the file (which split_edge_num1
         * represents) */
        realloc_check_vertices(vertices, num_vertices, cur_max_num_vertices);
        (*vertices)[*num_vertices] = (vertex_t *) malloc(sizeof(vertex_t));
        *((*vertices)[*num_vertices]) =
                calc_midpoint(start_vertex_of_first_edge_to_split,
                              end_vertex_of_first_edge_to_split);
        (*num_vertices)++;

        /* Same here but with split_edge_num_2 */
        realloc_check_vertices(vertices, num_vertices, cur_max_num_vertices);
        (*vertices)[*num_vertices] = (vertex_t *) malloc(sizeof(vertex_t));
        *((*vertices)[*num_vertices]) =
                calc_midpoint(start_vertex_of_second_edge_to_split,
                              end_vertex_of_second_edge_to_split);
        (*num_vertices)++;

        /* Create an extra temporary vertex in the middle of the last 2 vertices
         * which are the 2 formed by the function calls above */
        *tmp_vertex = calc_midpoint(*((*vertices)[*num_vertices-2]),
                                    *((*vertices)[*num_vertices-1]));

        /* Checks if the point is in both half-edges of interest, if not for
         * either one of them, swap to their counterpart */
        if (is_point_in_half_edge(*tmp_vertex,
                                   start_vertex_of_first_edge_to_split,
                                   end_vertex_of_first_edge_to_split)) {
            half_edge_of_interest1 = (*edges)[split_edge_num1].inner_half_edge;
        } else {
            half_edge_of_interest1 =
                    (*edges)[split_edge_num1].inner_half_edge->other;
        }

        if(is_point_in_half_edge(*tmp_vertex,
                                  start_vertex_of_second_edge_to_split,
                                  end_vertex_of_second_edge_to_split)) {
            half_edge_of_interest2 = (*edges)[split_edge_num2].inner_half_edge;
        } else {
            half_edge_of_interest2 =
                    (*edges)[split_edge_num2].inner_half_edge->other;
        }

        /* The splitting process is then conducted with the arguments */
        conduct_splitting(edges, num_edges, cur_max_num_edges,
                          half_edge_of_interest1, half_edge_of_interest2,
                          *vertices, num_vertices, faces, num_faces,
                          cur_max_num_faces);
    }

    free(tmp_vertex);
}

/*
   Secondary splitting function which conducts the actual splitting process,
   calls other functions to assist with the splitting process too. Edge cases
   are handled within another function that this function calls. Details of
   splitting are described in the function implementation itself with
   comments indicating the intention of the code.
   Note: half_edge_of_interest1 is the start_half_edge,
   half_edge_of_interest2 is the end_half_edge.
*/
void conduct_splitting(edge_t **edges, int *num_edges, int *cur_max_num_edges,
                       half_edge_t *half_edge_of_interest1,
                       half_edge_t *half_edge_of_interest2, vertex_t **vertices,
                       int *num_vertices, face_t **faces, int *num_faces,
                       int *cur_max_num_faces) {

    /* Boolean flag which determine whether the splitting is an adjacent
     * case or not */
    int adjacent_case = BOOL_FALSE;

    /* If the next pointer of the start half edge and the*/
    if(((half_edge_of_interest1->previous == half_edge_of_interest2) &&
    (half_edge_of_interest2->next == half_edge_of_interest1)) ||
    ((half_edge_of_interest1->next == half_edge_of_interest2) &&
    (half_edge_of_interest2->previous == half_edge_of_interest1))) {
        adjacent_case = BOOL_TRUE;
    }

    /* Store the end vertex of half edge 1 and start vertex of half edge 2
     * as temporary vertices */
    vertex_t *tmp_end_vertex_old_start_half_edge =
            half_edge_of_interest1->end_vertex;
    vertex_t *tmp_start_vertex_old_end_half_edge =
            half_edge_of_interest2->start_vertex;

    /* The end vertex of the 1st half edge we are conducting the split on
     * becomes the 1st new vertex we created from the split */
    vertex_t *first_new_vertex_from_split = (vertices)[(*num_vertices)-2];
    half_edge_of_interest1->end_vertex = first_new_vertex_from_split;

    /* The start vertex of the 2nd half edge we are conducting the split on
     * becomes the 2nd new vertex we created from the split */
    vertex_t *second_new_vertex_from_split = (vertices)[(*num_vertices)-1];
    half_edge_of_interest2->start_vertex = second_new_vertex_from_split;

    edge_t *new_edge_from_split = 
            (edge_t *) calloc(SINGLE_SPACE, sizeof(edge_t));
    assert(new_edge_from_split);

    /* New edge is created from the split based on the 2 new vertices created
     * but we create one of the half edges first */
    new_edge_from_split->inner_half_edge =
            (half_edge_t *) calloc(SINGLE_SPACE, sizeof(half_edge_t));
    assert(new_edge_from_split->inner_half_edge != NULL);
    new_edge_from_split->inner_half_edge->start_vertex =
            first_new_vertex_from_split;
    new_edge_from_split->inner_half_edge->end_vertex =
            second_new_vertex_from_split;


    /* Store the next pointer of the start half edge (half_edge_of_interest1)
     * and the previous pointer of the end half edge (half_edge_of_interest2) */
    half_edge_t *tmp_next_half_edge1 = half_edge_of_interest1->next;
    half_edge_t *tmp_prev_half_edge2 = half_edge_of_interest2->previous;

    /* Link the half edges by setting the previous pointer of the new half edge
     * to the start half edge and their next pointer of that start half edge to
     * the new half edge. Opposite procedure for the end half edge; next pointer
     * of new half edge set to the end half edge and the previous pointer of end
     * half edge set to the new half edge */
    link_half_edges(new_edge_from_split->inner_half_edge,
                    half_edge_of_interest1);
    link_half_edges(half_edge_of_interest2,
                    new_edge_from_split->inner_half_edge);

    /* Create the other half edge of the edge and the start and end vertex of
     * that is the reverse of the previous. All the arguments of the half edge
     * are then assigned such that the link of the pair is established */
    new_edge_from_split->inner_half_edge->other =
            (half_edge_t *) calloc(SINGLE_SPACE, sizeof(half_edge_t));
    assert(new_edge_from_split != NULL);
    new_edge_from_split->inner_half_edge->other->start_vertex =
            second_new_vertex_from_split;
    new_edge_from_split->inner_half_edge->other->end_vertex =
            first_new_vertex_from_split;
    new_edge_from_split->outer_half_edge =
            new_edge_from_split->inner_half_edge->other;
    new_edge_from_split->outer_half_edge->other =
            new_edge_from_split->inner_half_edge;
    new_edge_from_split->inner_half_edge->other =
            new_edge_from_split->outer_half_edge;

    /* Performs a check to see if realloc is needed before adding this
     * new edge and then add it */
    realloc_check_edges(edges, num_edges, cur_max_num_edges);
    add_edge_into_array(edges, num_edges,
                        cur_max_num_edges, new_edge_from_split);

    /* The pair of the half edge created from joining the 2 edges that we split
     * on is assigned for use later */
    half_edge_t other_of_half_edge_created =
            *(new_edge_from_split->inner_half_edge->other);

    /* The split process is then continued by assigning 2 pointers for the 2
     * new extra half edges created from the split. */
    half_edge_t *other_start_half_edge =
            (half_edge_t *) calloc(SINGLE_SPACE, sizeof(half_edge_t));
    assert(other_start_half_edge != NULL);
    half_edge_t *other_end_half_edge =
            (half_edge_t *) calloc(SINGLE_SPACE, sizeof(half_edge_t));
    assert(other_end_half_edge != NULL);

    /* The other start half edge goes from the midpoint (the 1st of the new
     * vertices created earlier) of the old start half edge to the end vertex of
     * the old start half edge which we stored earlier. This new half edge is
     * then linked to other half edge of the new edge we created
     * from the split. */
    other_start_half_edge->start_vertex = first_new_vertex_from_split;
    other_start_half_edge->end_vertex = tmp_end_vertex_old_start_half_edge;
    link_half_edges(other_start_half_edge, &other_of_half_edge_created);

    /* The other end half edge goes from the start vertex of the old end half
     * edge to the midpoint of the old end half edge (which was the 2nd of the
     * new vertices created earlier). Same procedure is done as above and
     * they are linked. */
    other_end_half_edge->start_vertex = tmp_start_vertex_old_end_half_edge;
    other_end_half_edge->end_vertex = second_new_vertex_from_split;
    link_half_edges(&other_of_half_edge_created, other_end_half_edge);

    /* Create 2 new edges which are associated with the other start and
     * end half edges */
    edge_t *new_edge_other_start_half_edge =
            (edge_t *) calloc(SINGLE_SPACE, sizeof(edge_t));
    assert(new_edge_other_start_half_edge != NULL);
    new_edge_other_start_half_edge->inner_half_edge = other_start_half_edge;

    edge_t *new_edge_other_end_half_edge =
            (edge_t *) calloc(SINGLE_SPACE, sizeof(edge_t));
    assert(new_edge_other_end_half_edge);
    new_edge_other_end_half_edge->inner_half_edge = other_end_half_edge;

    /* Add these new edges into the array */
    add_edge_into_array(edges, num_edges, cur_max_num_edges,
                        new_edge_other_start_half_edge);
    add_edge_into_array(edges, num_edges, cur_max_num_edges,
                        new_edge_other_end_half_edge);

    /* And then tie up all the loose ends by linking everything in order. */
    link_half_edges(new_edge_from_split->outer_half_edge,
                    new_edge_other_end_half_edge->inner_half_edge);
    link_half_edges(new_edge_other_start_half_edge->inner_half_edge,
                    new_edge_from_split->outer_half_edge);

    /* The next and previous pointers of the half edges we're splitting on
     * (arg 1 and arg 2) that we stored previously are connected to the relevant
     * half edges formed from the splits. The next pointer of the split edge
     * num 1 argument (first edge we were splitting on) is now the next of the
     * new half edge that we formed from splitting it the previous pointer of
     * what used to be the next half edge of the start half edge is also stored
     * to be the new half edge formed from splitting the start half edge.
     * Opposite applies here where the previous pointer of the new half edge
     * formed from splitting split edge num 2 argument (second edge we were
     * splitting on) is set to be the previous pointer of the old half edge
     * (that was the split edge num 2 argument). */
    link_half_edges(tmp_next_half_edge1,
                    new_edge_other_start_half_edge->inner_half_edge);
    link_half_edges(new_edge_other_end_half_edge->inner_half_edge,
                    tmp_prev_half_edge2);

    /* Deal with Edge Case 1 when the 1st half edge of interest
     * (1st argument that we split on) has a pair */
    if(half_edge_of_interest1->other != NULL) {

        /* Assign memory for the pair of other_start_half_edge (the new half
         * edge we created from splitting half_edge_of_interest1) and then the
         * conduct_split_with_pairs function will go through the appropriate
         * process to correctly create assign the pair it's relevant data */
        other_start_half_edge->other =
                (half_edge_t *) calloc(SINGLE_SPACE, sizeof(half_edge_t));
        assert(other_start_half_edge->other != NULL);
        conduct_split_with_pairs(half_edge_of_interest1->other,
                                 other_start_half_edge,
                                 first_new_vertex_from_split, START);

        /* Once we're done the other argument of the pair is set to the original
         * and the link is established */
        other_start_half_edge->other->other = other_start_half_edge;
        (*edges)[other_start_half_edge->edge_index].outer_half_edge =
                other_start_half_edge->other;
    }

    /* Deal with Edge Case 2 when the 2nd half edge of interest (2nd argument
     * that we split on) has a pair */
    if(half_edge_of_interest2->other != NULL){

        /* Same process as above but with the pair of other_end_half_edge (the
         * new half edge we created from splitting half_edge_of_interest2) */
        other_end_half_edge->other =
                (half_edge_t *) calloc(SINGLE_SPACE, sizeof(half_edge_t));
        assert(other_end_half_edge->other != NULL);
        conduct_split_with_pairs(half_edge_of_interest2->other,
                                 other_end_half_edge,
                                 second_new_vertex_from_split, END);

        other_end_half_edge->other->other = other_end_half_edge;
        (*edges)[other_end_half_edge->edge_index].outer_half_edge =
                other_end_half_edge->other;
    }

    if(adjacent_case){
        half_edge_of_interest1->next = new_edge_from_split->inner_half_edge;
        half_edge_of_interest2->previous = new_edge_from_split->inner_half_edge;

        link_half_edges(other_end_half_edge, other_start_half_edge);
    }

    /* After the split we have 1 more face (this is always guaranteed). */
    (*num_faces)++;

    /* The old edges get their face numbering preserved */
    change_all_half_edge_to_face_x(half_edge_of_interest1,
                                   half_edge_of_interest1->face_index,
                                   faces, num_faces, cur_max_num_faces);

    /* The new edges get the newest face numbering */
    change_all_half_edge_to_face_x(other_start_half_edge, (*num_faces)-1,
                                   faces, num_faces, cur_max_num_faces);

    /* Free all the temporary variables used at the end of the function */
    free(new_edge_from_split);
    free(new_edge_other_start_half_edge);
    free(new_edge_other_end_half_edge);
}
/*
   Tertiary Splitting Function which handles the "DCEL Split with pairs" edge
   cases by conducting the extra processes of splitting when one of the edges
   given in the argument that the program is splitting on has a pair. Refer
   to calling context in the conduct_splitting function for the information
   of when this function is used.
*/
void conduct_split_with_pairs(half_edge_t *half_edge_of_interest_pair,
                              half_edge_t
                              *one_of_new_half_edge_created_from_split,
                              vertex_t *midpoint, int start_or_end) {

    /* The pair of the half edge of interest 1 is passed in whereas the original
     * one_of_new_half_edge_created_from_split (not the pair) is passed in to
     * get access to the elements of the original in the function to
     * assign to the pair */

    /* This new paired half edge has the same face as the old start half edge */
    one_of_new_half_edge_created_from_split->other->face_index =
            half_edge_of_interest_pair->face_index;

    /* It also belongs to the same edge as it's pair (which is obvious) */
    one_of_new_half_edge_created_from_split->other->edge_index =
            one_of_new_half_edge_created_from_split->edge_index;

    /* The case where the pair of the new half edge (a.k.a.
     * one_of_new_half_edge_created_from_split) comes after the old half edge
     * (a.k.a. *half_edge_of_interest1_pair) */

    if(start_or_end == END) {
        /* Edge Case 2 (DCEL Split with Pairs) */

        /* This is when the pair of the new half edge created from split should
         * come after the pair of the half edge of interest 2 (the pair of the
         * 2nd split edge num argument) */

        /* The process for edge case 2 exactly mirrors that of edge case 1 */
        one_of_new_half_edge_created_from_split->other->start_vertex = midpoint;
        one_of_new_half_edge_created_from_split->other->end_vertex =
                half_edge_of_interest_pair->end_vertex;

        half_edge_t *next_half_edge_of_interest_pair =
                half_edge_of_interest_pair->next;

        half_edge_of_interest_pair->end_vertex = midpoint;

        link_half_edges(one_of_new_half_edge_created_from_split->other,
                        half_edge_of_interest_pair);

        link_half_edges(next_half_edge_of_interest_pair,
                        one_of_new_half_edge_created_from_split->other);

    } else {
        /* Edge Case 1 (DCEL Split with Pairs) */

        /* This is when the pair of the new half edge created from the split
         * should come before the pair of the half edge of interest 1
         * (the pair of the 1st split edge num argument) */

        /* The pair of the new half edge created from the splitting the 1st half
         * edge ends at the midpoint between the 2 (same as their pairs,
         * midpoint has already been calculated earlier and passed into the
         * function). The start vertex is the same as the start vertex of the
         * pair of the half edge we were splitting on previously. */
        one_of_new_half_edge_created_from_split->other->end_vertex = midpoint;
        one_of_new_half_edge_created_from_split->other->start_vertex =
                half_edge_of_interest_pair->start_vertex;

        /* Store the previous half edge of the pair of half edge of
         * interest 1 */
        half_edge_t *prev_half_edge_of_interest1_pair =
                half_edge_of_interest_pair->previous;

        /* Make the start vertex of it the midpoint between them instead */
        half_edge_of_interest_pair->start_vertex = midpoint;

        /* Now the vertices are all correct and we set the pointers so that they
         * are in order. The pair of the new half edge created from split should
         * come before the pair of the half edge of interest 1 (1st half edge we
         * were splitting on) and then the previous of the pair of the half edge
         * we were splitting on should come before the pair of the new half edge
         * we created from the split */
        link_half_edges(half_edge_of_interest_pair,
                        one_of_new_half_edge_created_from_split->other);
        link_half_edges(one_of_new_half_edge_created_from_split->other,
                        prev_half_edge_of_interest1_pair);


    }
}

/*
   Sets the half_edge_in_face pointer of a face in the faces array to the
   initial_half_edge half edge argument and then goes through all the half edges
   that is supposed to belong to a face x by using an initial half edge pointer
   and then using the next pointer of the half edge to go through all the
   half edges until it goes back to the initial half edge. Function
   realloc_check_faces is also called as the function is directly responsible
   for adding a new face into the faces array.
 */
void change_all_half_edge_to_face_x(half_edge_t *initial_half_edge,
                                    int x, face_t **faces, int *num_faces,
                                    int *cur_max_num_faces) {

    realloc_check_faces(faces, num_faces, cur_max_num_faces);

    half_edge_t *cur_half_edge = initial_half_edge;
    (*faces)[x].half_edge_in_face = initial_half_edge;

    do {
        cur_half_edge->face_index = x;
        cur_half_edge = cur_half_edge->next;
    } while (cur_half_edge != initial_half_edge);
}

void print_edges(edge_t **edges, int num_edges) {
    for(int i = 0; i<num_edges; i++) {
        half_edge_t inner_half_edge = *((*edges)[i].inner_half_edge);

        printf("@E%d %lf %lf %lf %lf\n", i, inner_half_edge.start_vertex->x_pos,
               inner_half_edge.start_vertex->y_pos,
               inner_half_edge.end_vertex->x_pos,
               inner_half_edge.end_vertex->y_pos);

        if(inner_half_edge.other != NULL){
            half_edge_t outer_half_edge = *(inner_half_edge.other);
            printf("@E%d %lf %lf %lf %lf\n", -i,
                   outer_half_edge.start_vertex->x_pos,
                   outer_half_edge.start_vertex->y_pos,
                   outer_half_edge.end_vertex->x_pos,
                   outer_half_edge.end_vertex->y_pos);
        }

    }
}
