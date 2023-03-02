/*
    Function which uses the geometrical data and setups the initial polygon
    by calling the get_vertices and initialise_edges function.

    filename: A char array (string) variable which represents the filename of
        the file to read in to construct the intial vertices

    ***vertices: A pointer to a pointer to an array of vertices which is
        supposed to store the vertices in the DCEL

    *num_vertices: A pointer to an int variable which represents the number of
        vertices in the vertices array

    *cur_max_num_vertices: Pointer to an int variable which represents the
        current max number of vertices the vertices array can hold

    **edges: A pointer to an edge_t array which stores all the edges in the DCEL

    *num_edges: A pointer to an int variable which represents the number of
        edges in the edges array

    *cur_max_num_edges: A pointer to an int variable which represents the
        current max number of edges the edges array can hold

    Returns: none
 */
void setup_polygon(char *filename, vertex_t ***vertices, int *num_vertices,
                    int *cur_max_num_vertices, edge_t **edges, int *num_edges,
                    int *cur_max_num_edges);

/*
    Function which reads in the arguments given in the filename and constructs
    the initial vertices in the vertices array.

    filename: A char array (string) variable which represents the filename of
        the file to read in to construct the intial vertices

    ***vertices: A pointer to a pointer to an array of vertices which is
        supposed to store the vertices in the DCEL

    *num_vertices: A pointer to an int variable which represents the number of
        vertices in the vertices array

    *cur_max_num_vertices: Pointer to an int variable which represents the
        current max number of vertices the vertices array can hold

    Returns: none
 */
void get_vertices(char *filename, vertex_t ***vertices, int *num_vertices,
                  int *cur_max_num_vertices);

/*
    Function which uses the initial vertices to create the initial edges of the
    polygon along with the auxiliary function build_half_edge initial.

    **edges: A pointer to an edge_t array which stores all the edges in the DCEL

    *num_edges: A pointer to an int variable which represents the number of
        edges in the edges array

    *cur_max_num_edges: A pointer to an int variable which represents the
        current max number of edges the edges array can hold

    ***vertices: A pointer to a pointer to an array of vertices which is
        supposed to store the vertices in the DCEL

    *num_vertices: A pointer to an int variable which represents the number of
        vertices in the vertices array

    Returns: none
 */
void initialise_edges(edge_t **edges, int *num_edges, int *cur_max_num_edges,
                      vertex_t ***vertices, int *num_vertices);

/*
    Auxiliary function which builds a single initial half edge and links it
    with the previous half edge created (if any). Called by the initialise_edges
    function.

    *cur_half_edge: A pointer to the current half edge that is being built

    ***vertices: A pointer to a pointer to an array of vertices which is
        supposed to store the vertices in the DCEL

    index: The intended index of this half edge in the edge array

    *prev_half_edge: A pointer to the previous half edge that was built

    last_edge: An int variable which represents whether the function should be
        run on the configuration to build the last half edge

 */
void build_half_edge_initial(half_edge_t *cur_half_edge, vertex_t ***vertices,
                             int index, half_edge_t *prev_half_edge,
                             int last_edge);

/*
    Function that takes 2 half edges and links them together by setting the
    previous pointer of the this_half_edge argument to the prev_half_edge
    argument and the next pointer of the prev_half_edge argument to the
    this_half_edge argument

    *this_half_edge: A pointer to a half edge intended to be the one that comes
        after of the 2 half edge arguments

    *prev_half_edge: A pointer to a half edge intended to be the one that comes
        before of the 2 half edge arguments

    Returns: none
 */
void link_half_edges(half_edge_t *this_half_edge, half_edge_t *prev_half_edge);

/*
    Function which calculates and determines if a point is in a half plane
    using formula provided by specification. If it is, return 1; otherwise
    return 0

    point: A vertex_t variable which represents the point of interest

    vertex_1: A vertex_t variable which represents the start vertex
        of the half edge

    vertex_2: A vertex_t variable which represents the end vertex
        of the half edge

    Returns: 1 if point is determined to be in half plane; 0 otherwise
 */
int is_point_in_half_edge(vertex_t point, vertex_t vertex1, vertex_t vertex2);

/*
    Calculates the gradient of a half edge using its start and end
    vertices and returns it.

    vertex_1: A vertex_t variable which represents the start vertex
        of the half edge

    vertex_2: A vertex_t variable which represents the end vertex
        of the half edge

    Returns: A double variable which is the gradient of the half edge
 */
double calc_gradient(vertex_t vertex_1, vertex_t vertex_2);

/*
    Calculates the intercept of a half edge using its gradient and end vertex
    and returns it.

    gradient: A double variable which represents the gradient of the half edge
        obtained through calling the calc_gradient function

    vertex_2: A vertex_t variable which represents the end vertex
        of the half edge

    Returns: A double variable which is the intercept of the half edge
 */
double calc_intercept(double gradient, vertex_t vertex2);

/*
    Function which calculates the y-relation of a half edge and returns it.

    point: A vertex_t variable which represents the point of interest

    vertex_1: A vertex_t variable which represents the start vertex
        of the half edge

    vertex_2: A vertex_t variable which represents the end vertex
        of the half edge

    Returns: A double variable which is the y-relation of the half edge
 */
double get_y_relation(vertex_t point, vertex_t vertex_1, vertex_t vertex_2);

/*
    Function used for adding an edge into an array which does all the checks
    and assignments to ensure nothing goes wrong.
    
    **edges: A pointer to an edge_t array which stores all the edges in the DCEL

    *num_edges: A pointer to an int variable which represents the number of
        edges in the edges array

    *cur_max_num_edges: A pointer to an int variable which represents the
        current max number of edges the edges array can hold

    *edge_to_add: A pointer to an edge that is intended to be added into the
        edges array
    
    Returns: none
 */
void add_edge_into_array(edge_t **edges, int *num_edges, int *cur_max_num_edges,
                         edge_t *edge_to_add);
/*
    Calculates the midpoint of 2 vertices and returns it as a vertex.

    vertex_1: A vertex_t variable which represents the start vertex
        of the half edge

    vertex_2: A vertex_t variable which represents the end vertex
        of the half edge

    Returns: A vertex_t variable which represents the midpoint of the 2
        vertices passed in as the arguments
 */
vertex_t calc_midpoint(vertex_t vertex_1, vertex_t vertex_2);

