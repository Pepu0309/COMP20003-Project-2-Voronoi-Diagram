#include "voronoi1.h"
#include "watchtowers_io.h"
#include "memory_funcs.h"

/*
    Generic function which checks if enough space is in the watchtower_list
    watchtower_t array and calls realloc if necessary.
 */
void realloc_check_watchtowers(watchtower_t **watchtower_list,
                               int *num_watchtowers,
                               int *cur_max_num_watchtowers) {
    if(*num_watchtowers >= *cur_max_num_watchtowers) {
        (*cur_max_num_watchtowers) *= DOUBLE_CUR_SPACE;
        *watchtower_list = (watchtower_t *) realloc(*watchtower_list,
                                                    (*cur_max_num_watchtowers) *
                                                    sizeof(watchtower_t));
        assert(*watchtower_list != NULL);
    }
}

/*
    Generic function which checks if enough space is in the vertices vertex_t
    array and calls realloc is necessary.
 */
void realloc_check_vertices(vertex_t ***vertices, int *num_vertices,
                            int *cur_max_num_vertices) {
    if(*num_vertices >= *cur_max_num_vertices){
        (*cur_max_num_vertices) *= DOUBLE_CUR_SPACE;
        *vertices = (vertex_t **) realloc(*vertices, (*cur_max_num_vertices) *
                                          sizeof(vertex_t *));
        assert(*vertices != NULL);
    }
}

/*
    Generic function which checks if enough space is in the vertices vertex_t
    array and calls realloc is necessary.
 */
void realloc_check_edges(edge_t **edges, int *num_edges,
                         int *cur_max_num_edges) {
    if(*num_edges >= *cur_max_num_edges){
        (*cur_max_num_edges) *= DOUBLE_CUR_SPACE;
        *edges = (edge_t *) realloc(*edges, (*cur_max_num_edges) *
                                    sizeof(edge_t));
        assert(*edges != NULL);
    }
}

/*
    Generic function which checks if enough space is in the faces face_t
    array and calls realloc is necessary.
 */
void realloc_check_faces(face_t **faces, int *num_faces,
                         int *cur_max_num_faces) {
    if(*num_faces >= *cur_max_num_faces){
        (*cur_max_num_faces) *= DOUBLE_CUR_SPACE;
        *faces = (face_t *) realloc(*faces, (*cur_max_num_faces) *
                                    sizeof(face_t));
        assert(*faces != NULL);
    }
}

/*
    Function which frees the dynamically allocated strings in each watchtower_t
    variable in the watchtower_list array and then frees array itself as well.
 */
void free_csv(watchtower_t **watchtower_list, int *num_watchtowers) {
    
    for(int i = 0; i < *num_watchtowers; i++) {
        free((*watchtower_list)[i].watchtower_ID);
        free((*watchtower_list)[i].postcode);
        free((*watchtower_list)[i].contact_name);
    }

    free(*watchtower_list);
}

/*
    Goes through all the edges in the edges array and frees all it's half_edges
    and then frees the edges array itself.
 */
void free_edges(edge_t **edges, int num_edges) {
    for(int i = 0; i < num_edges; i++) {
        free((*edges)[i].inner_half_edge);
        if((*edges)[i].outer_half_edge != NULL) {
            free((*edges)[i].outer_half_edge);
        }
    }

    free(*edges);
}

/*
    Goes through all the vertices in the vertices array and frees all of them
    and then free the vertices array itself.
 */
void free_vertices(vertex_t ***vertices, int num_vertices) {
    for(int i = 0; i < num_vertices; i++) {
        free((*vertices)[i]);
    }
    free(*vertices);
}


