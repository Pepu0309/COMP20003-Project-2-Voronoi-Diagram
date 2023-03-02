#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define COL_NUM 6
#define INITIAL_ARRAY_MAX 5
#define COMMA_DELIM ","
#define SPACE_DELIM " "
#define MAX_LINE_LENGTH 512
#define SPACE_FOR_NULL_CHAR 1
#define SINGLE_SPACE 1
#define DOUBLE_CUR_SPACE 2
#define BOOL_TRUE 1
#define BOOL_FALSE 0
#define INITIAL_FACE_NUM 0
#define HALF_DOUBLE 1.0/2.0
#define START 3 /* Flag which represents that the DCEL split with pairs
                 * edge case based on the start half edge should be executed */
#define END 4 /* Flag which represents that the DCEL split with pairs
                 * edge case based on the end half edge should be executed */

/* Struct data type which represents a watchtower. Struct elements are 
   declared according to the header row of the csv file. */
typedef struct watchtower {
    char *watchtower_ID;
    char *postcode;
    int population_served;
    char *contact_name;
    double x_coord;
    double y_coord;
} watchtower_t;

/* Struct data type which represents a vertex in the DCEL. 
   Each vertex has a pair of x,y coordinates */
typedef struct vertex{
    double x_pos;
    double y_pos;
} vertex_t;

/* Struct data type which represents a half edge in the DCEL. 
   Each half edge contains pointers to it's end and start vertices 
   as well as pointers to its next, previous and twin half edge. 
   The half edge also contains 2 int variables representing the 
   edge and face it belongs to. */
typedef struct half_edge half_edge_t;
struct half_edge{
    vertex_t *end_vertex;
    vertex_t *start_vertex;
    half_edge_t *next;
    half_edge_t *previous;
    half_edge_t *other;
    int face_index;
    int edge_index;
};

/* Struct data type which represents an edge in the DCEL. Each edge
   has at least one half edge but may have 2 paired half edges. */
typedef struct edge{
    half_edge_t *inner_half_edge;
    half_edge_t *outer_half_edge;
} edge_t;

/* Struct data type which represents a face. Each face contains a pointer to
   a given half edge that resides in the face. */
typedef struct face{
    half_edge_t *half_edge_in_face; // Any half edge in the face
} face_t;

/*--------------------Visualisation Script Functions--------------------------*/
double getWTx(int index, watchtower_t *watchtower_list);

double getWTy(int index, watchtower_t *watchtower_list);

double getDCELVertexX(vertex_t *vertex, int index);

double getDCELVertexY(vertex_t *vertex, int index);

