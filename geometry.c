#include "voronoi1.h"
#include "geometry.h"
#include "memory_funcs.h"
#include "split.h"
/*
    Function which uses the geometrical data and setups the initial polygon
    by calling the get_vertices and initialise_edges function.
 */
void setup_polygon(char *filename, vertex_t ***vertices, int *num_vertices,
                   int *cur_max_num_vertices, edge_t **edges, int *num_edges,
                   int *cur_max_num_edges) {

    get_vertices(filename, vertices, num_vertices, cur_max_num_vertices);
    /* Initialise the array of edges through malloc-ing all the memory required;
    number of edges equals to number of vertices */
    initialise_edges(edges, num_edges, cur_max_num_edges, vertices,
                     num_vertices);

}

/*
    Function which reads in the arguments given in the filename and constructs
    the initial vertices in the vertices array. The space for each vertex is
    created by malloc, and then the x and y coordinates are read in from the
    given filename argument and stored in the respective fields of the vertex_t
    struct. Memory checking and cleanup is also done while and after executing
    the function.
 */
void get_vertices(char *filename, vertex_t ***vertices, int *num_vertices,
                  int *cur_max_num_vertices) {

    char *line = NULL;
    size_t lineBufferLength = MAX_LINE_LENGTH;

    /* Open the file for reading */
    FILE *fp = fopen(filename, "r");

    char *delimiter = SPACE_DELIM;


    *vertices = (vertex_t **) calloc(*cur_max_num_vertices, sizeof(vertex_t *));

    while(getline(&line, &lineBufferLength, fp) != EOF) {

        realloc_check_vertices(vertices, num_vertices, cur_max_num_vertices);

        char **discard_ptr = NULL;
        (*vertices)[*num_vertices] = (vertex_t *) malloc(sizeof(vertex_t));

        (*vertices)[*num_vertices]->x_pos = strtod(strtok(line, delimiter),
                                                   discard_ptr);
        (*vertices)[*num_vertices]->y_pos = strtod(strtok(NULL, delimiter),
                                                   discard_ptr);

        (*num_vertices)++;

        fflush(stdout);
    }

    fclose(fp);
    free(line);
    line = NULL;
}

/*
    Once the vertices are read in, this function is called to initialise the
    edges of the initial polygon. Details of the process is described
    in the function itself through comments above the code chunks.
 */
void initialise_edges(edge_t **edges, int *num_edges, int *cur_max_num_edges,
                      vertex_t ***vertices, int *num_vertices) {

    /* At the start, the number of edges is equal to the number of vertices
       (comes from the definition of a polygon) */
    *num_edges = *num_vertices;
    *cur_max_num_edges = *num_edges;

    /* By default, the outer_half_edge pointer becomes NULL through
     * using calloc */
    *edges = (edge_t *) calloc((*cur_max_num_edges), sizeof(edge_t));
    assert(*edges != NULL);

    half_edge_t *cur_half_edge = NULL;
    half_edge_t *prev_half_edge = NULL;

    /* Edge i-1 is formed by Vertex i and i-1, e.g. Edge 0 is formed by
       Vertex 1 and Vertex 0. The logic is used for everything here,
       including the function calls. */
    for(int i = 1; i < *num_edges; i++) {
        realloc_check_edges(edges, num_edges, cur_max_num_edges);
        (*edges)[i-1].inner_half_edge =
                (half_edge_t *) calloc(1,sizeof(half_edge_t));
        (*edges)[i-1].outer_half_edge = NULL;
        (*edges)[i-1].inner_half_edge->other = NULL;

        cur_half_edge = (*edges)[i-1].inner_half_edge;
        build_half_edge_initial(cur_half_edge, vertices, i, prev_half_edge,
                                BOOL_FALSE);
        /* Set the prev half edge to point to the cur half edge for the next
         * iteration */
        prev_half_edge = cur_half_edge;
    }
    /* Build the half edge going from the last vertex to the 1st one */
    realloc_check_edges(edges, num_edges, cur_max_num_edges);
    (*edges)[(*num_edges)-1].inner_half_edge =
            (half_edge_t *) calloc(1, sizeof(half_edge_t));
    (*edges)[(*num_edges)-1].outer_half_edge = NULL;
    (*edges)[(*num_edges)-1].inner_half_edge->other = NULL;

    cur_half_edge = (*edges)[(*num_edges)-1].inner_half_edge;
    /* Pass num_edges because the index-1 logic in the build_half_edge function
     * automatically handles the need to -1 */
    build_half_edge_initial(cur_half_edge, vertices, *num_edges, prev_half_edge,
                            BOOL_TRUE);

    link_half_edges((*edges)[0].inner_half_edge,
                    (*edges)[(*num_edges)-1].inner_half_edge);

}
/*
    Function which builds a half edge of the initial polygon by utilising
    the property of "Edge i-1 is created by connecting vertices i and i-1" in
    general. Hence, if the last_edge argument is passed as BOOL_FALSE, the
    function will go through the general case and create the half edge
    by setting its end vertex to vertex i and its start vertex to i-1. If a
    previous half edge exists, they will also be linked. The face_index of
    this initial half edge is then set to 0 and its edge_index is set to be
    i-1 following the property described above. If BOOL_TRUE is passed,
    as the last_edge argument, the end vertex becomes the first vertex,
    everything else remains the same.
 */
void build_half_edge_initial(half_edge_t *cur_half_edge, vertex_t ***vertices,
                             int index, half_edge_t *prev_half_edge,
                             int last_edge) {

    /* If the last edge boolean flag is passed, the half-edge is connected from
     * the last vertex to the first one */
    if(last_edge){
        cur_half_edge->end_vertex = (*vertices)[0];
    } else {
        cur_half_edge->end_vertex = (*vertices)[index];
    }

    (cur_half_edge->start_vertex) = (*vertices)[index-1];

    if(prev_half_edge != NULL){
        link_half_edges(cur_half_edge, prev_half_edge);
    }

    /* All edges are initially face 0 */
    cur_half_edge->face_index = INITIAL_FACE_NUM;
    /* As mentioned, Edge i-1 is formed by vertex i and vertex i-1, and so this
     * half edge is part of Edge i-1 */
    cur_half_edge->edge_index = index-1;

}

/*
    Function that takes 2 half edges and links them together by setting the
    previous pointer of the this_half_edge argument to the prev_half_edge
    argument and the next pointer of the prev_half_edge argument to the
    this_half_edge argument
*/
void link_half_edges(half_edge_t *this_half_edge, half_edge_t *prev_half_edge) {
    this_half_edge->previous = prev_half_edge;
    prev_half_edge->next = this_half_edge;
}

/*
    Function which determines whether a point is in a half edge using the
    formulas described in the "Determining whether a particular point
    is in a given face" subsection in the "Hints" section of the specification.
    The start vertices (vertex_1) and end vertices (vertex_2) are used in the
    calculations of determining whether the point is in the half edge.
    As all half edges are supposed to be going clockwise, In the case where the
    x coordinates of both vertices are the same (vertical line), if the y
    coordinate of the end vertex is larger than the y coordinate of
    the start vertex then the point has to be to the right of it and
    vice versa for when the y coordinate of the end vertex is smaller. If the
    x coordinates are different, then we use the y-relation formula and call
    the function to calculate the y-relation to determine if the point is in
    the half edge.
 */
int is_point_in_half_edge(vertex_t point, vertex_t vertex_1,
                          vertex_t vertex_2) {
    if(vertex_1.x_pos == vertex_2.x_pos) {
        if(vertex_1.y_pos >= vertex_2.y_pos) {
            if(point.x_pos <= vertex_1.x_pos) {
                return BOOL_TRUE;
            }
        } else if (vertex_1.y_pos <= vertex_2.y_pos) {
            if(point.x_pos >= vertex_1.x_pos) {
                return BOOL_TRUE;
            }
        }
    } else {
        if((vertex_1.x_pos >= vertex_2.x_pos) &&
            (get_y_relation(point, vertex_1, vertex_2) >= 0)) {
            return BOOL_TRUE;
        } else if((vertex_1.x_pos <= vertex_2.x_pos) &&
                  (get_y_relation(point, vertex_1, vertex_2) <=0)) {
            return BOOL_TRUE;
        }
    }
    return BOOL_FALSE;
}
/*
    Simple function that calculates the gradient of the half edge using it's
    start and end vertices and returns it.
 */
double calc_gradient(vertex_t vertex_1, vertex_t vertex_2) {
    return ((vertex_2.y_pos - vertex_1.y_pos) /
            (vertex_2.x_pos - vertex_1.x_pos));
}

/*
    Simple function that calculates the intercept of the half edge using its
    gradient and end vertex and returns it.
 */
double calc_intercept(double gradient, vertex_t vertex_2) {
    return (vertex_2.y_pos - (gradient*vertex_2.x_pos));
}

/*
    Calculates the y-relation of the half edge using the gradient, intercept
    and y-predicted as described the specification and returns it.
 */
double get_y_relation(vertex_t point, vertex_t vertex_1, vertex_t vertex_2) {
    double gradient = calc_gradient(vertex_1, vertex_2);
    double intercept = calc_intercept(gradient, vertex_2);

    double y_predicted = gradient * point.x_pos + intercept;

    return (point.y_pos - y_predicted);
}

/*
    General function used for adding an edge into an array. The process is
    described more in detail in the function itself.
 */
void add_edge_into_array(edge_t **edges, int *num_edges, int *cur_max_num_edges,
                         edge_t *edge_to_add) {

    /* Check if there is enough space in the array, if not, call realloc */
    realloc_check_edges(edges, num_edges, cur_max_num_edges);

    /* Add the edge to the array */
    (*edges)[*num_edges] = *edge_to_add;

    /* Set the edge index of the inner half edge to the current num edges */
    edge_to_add->inner_half_edge->edge_index = *num_edges;

    /* And if there is a paired half edge, set the edge index of it too and
     * make it the outer half edge */
    if((edge_to_add->inner_half_edge->other) != NULL) {
        edge_to_add->inner_half_edge->other->edge_index = *num_edges;
        edge_to_add->outer_half_edge = edge_to_add->inner_half_edge->other;
    }

    (*num_edges)++;
}
/*
    Simple function which calculates the midpoint between 2 vertices and
    returns it as a vertex_t variable.
 */
vertex_t calc_midpoint(vertex_t vertex_1, vertex_t vertex_2) {
    vertex_t midpoint;
    midpoint.x_pos = vertex_1.x_pos + (HALF_DOUBLE)*(vertex_2.x_pos -
            vertex_1.x_pos);
    midpoint.y_pos = vertex_1.y_pos + (HALF_DOUBLE)*(vertex_2.y_pos -
            vertex_1.y_pos);

    return midpoint;
}






