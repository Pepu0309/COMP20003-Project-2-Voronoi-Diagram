/* Add any includes you need here */
#include "voronoi1.h"
#include "watchtowers_io.h"
#include "geometry.h"
#include "split.h"
#include "memory_funcs.h"

/* This block of code will run if -DUSE_GUI is passed to gcc */
#ifdef USE_GUI
/* GUI Application Extensions */
#include "gtk.h"
#endif

/* This block of code will run if -DUSE_GUI is passed to gcc */
#ifdef USE_GUI
/* 
This is a simple function which says the point which was clicked on the map. 
You can do plenty of things with this, e.g. search through all the watchtowers and
find the closest point and print it out, calculate which face the click is in (if any),
etc. If you pass the dcel as the data, you may have to typecast it back to your dcel 
struct type, or change the function definition here. If you want to collect a bunch 
of context data (e.g. you want to perform one split at a time after each click). 
Note that this click action will occur twice, so keep a click counter to only act on
every second click. =)
*/
void doAction(void *data, double mouseX, double mouseY);

void doAction(void *data, double mouseX, double mouseY){
    printf("Mouse clicked at (%f, %f)\n", mouseX, mouseY);
}


#define WT_CHANGE 0
#define POLY_CHANGE 1
#define EDGE_CHANGE 2
/* This function uses data[1] to cycle to the edge given by 
data[0] // 2, with data[0] incrementing for every click. This
is useful for working out which watchtower is which, which polygon
vertex is which and which edge is which. */
void doCyleOnClick(int *data, double mouseX, double mouseY);

void doCyleOnClick(int *data, double mouseX, double mouseY){
    data[1] = data[1] + 1;
    if(data[1] % 2 == 1){
        switch(data[0]){
            case WT_CHANGE:
                setGTKWatchTower(data[1] / 2);
                printf("Switched WatchTower to %d\n", data[1] / 2);
                break;
            
            case POLY_CHANGE:
                setGTKVertex(data[1] / 2);
                printf("Switched Vertex to %d\n", data[1] / 2);
                break;
            
            case EDGE_CHANGE:
                setGTKEdge(data[1] / 2);
                printf("Switched Edge to %d\n", data[1] / 2);
                break;
            default:
                break;
        }
    }

    //printf("Mouse clicked at (%f, %f)\n", mouseX, mouseY);
}

#endif

int main(int argc, char **argv) {
    /* Setup and construction can go here! */
    char *filename = NULL;
    int num_watchtowers = 0;
    int cur_max_num_watchtowers = INITIAL_ARRAY_MAX;

    watchtower_t *watchtower_list = NULL;
    /* Obtain filename from the 1st command line argument given then
     * read the csv data in */
    filename = argv[1];

    read_CSV(&watchtower_list, filename, &num_watchtowers,
             &cur_max_num_watchtowers);

    /* Done with reading csv so we move over to the second command line argument
     * to read the initial polygon in and store it */
    filename = argv[2];

    int num_vertices = 0;
    int cur_max_num_vertices = INITIAL_ARRAY_MAX;
    int num_edges = 0;
    int cur_max_num_edges = INITIAL_ARRAY_MAX;

    vertex_t **vertices = NULL;
    edge_t *edges = NULL;


    setup_polygon(filename, &vertices, &num_vertices, &cur_max_num_vertices,
                   &edges, &num_edges, &cur_max_num_edges);

    /* Manually setup the initial face since the initial polygon only has 1 face
     * (face 0) whereas the face points to an edge in the face
     * (edge 0 in this case just as default). */

    int num_faces = 0;
    int cur_max_num_faces = INITIAL_ARRAY_MAX;
    face_t *faces = (face_t *) calloc(cur_max_num_faces, sizeof(face_t));

    faces[num_faces].half_edge_in_face = edges[0].inner_half_edge;
    num_faces++;

    /* Now that everything is setup, the splitting is conducted to form the
     * final DCEL */

    split_polygon(&vertices, &num_vertices, &cur_max_num_vertices, &edges,
                  &num_edges, &cur_max_num_edges, &faces, &num_faces,
                  &cur_max_num_faces);

    /* Splitting is done, final DCEL is built, do the checking for watchtowers
     * and faces and write the output to the 3rd command line argument in
     * the required format */
    filename = argv[3];
    perform_watchtower_in_face_check(watchtower_list, num_watchtowers, faces,
                                     num_faces, filename);
    /*------------------------------------------------------------------------*/

    #ifdef USE_GUI
    /* Use GUI Application Extensions */
    setupGTK(&argc, &argv, "Assignment 1: Graph Visualisation");
    
    /* Visualisation: Set Bounds */
    #define VICTORIAXMIN (140.9)
    #define VICTORIAXMAX (150.0)
    /* Use larger bounds so we get a square aspect ratio and see Victoria. */
    #define VICTORIAYMIN (-41.85)
    // #define VICTORIAYMIN (-39.2)
    #define VICTORIAYMAX (-32.0)
    //#define VICTORIAYMAX (-33.9)
    
    setupGTKBounds(VICTORIAXMIN, VICTORIAXMAX, VICTORIAYMIN, VICTORIAYMAX);

    double getWTx(int index, watchtower_t *watchtower_list) {
        return watchtower_list[index].x_coord;
    }

    double getWTy(int index, watchtower_t *watchtower_list) {
        return watchtower_list[index].y_coord;
    }
    
    /* This section connects your watch tower points into the visualisation. */
    /* Visualisation: Points */
    int pointCount = num_watchtowers;
    setupGTKPoints(pointCount, 
        (void *) watchtower_list,
         NULL,
         NULL,
        (double (*)(int, void *)) &getWTx,
        (double (*)(int, void *)) &getWTy
    ); 

    
    double getDCELVertexX(vertex_t *vertices, int index) {
        return vertices[index].x_pos;
    }

    double getDCELVertexY(vertex_t *vertices, int index) {
        return vertices[index].y_pos;
    }
    /* This section connects your DCEL vertices into the visualisation. */
    /* Visualisation: DCEL points */
    //int dcelVerticesCount = getDCELPointCount(dcel);
    setupGTKPolyPoints(num_vertices,
        (void *) vertices,
        (double (*)(void *, int)) &getDCELVertexX,
        (double (*)(void *, int)) &getDCELVertexY
    );
    
    /* This section connects your DCEL lines into the visualisation. */
    /* Visualisation: DCEL lines */
    
    /*  
    
    int getDCELEdgeVertexPairStart() {

    }

    int DCELhasEdge(edge_t)
    int dcelEdgeCount = num_edges;
    setupGTKPolyLines(dcelEdgeCount,
        (void *) dcel,
        (int (*)(void *, int)) &getDCELEdgeVertexStart,
        (int (*)(void *, int)) &getDCELEdgeVertexPairStart,
        (int (*)(void *, int)) &getDCELEdgeVertexEnd,
        (int (*)(void *, int)) &getDCELEdgeVertexPairEnd,
        (int (*)(void *, int)) &DCELhasEdge,
        (int (*)(void *, int)) &DCELhasEdgePair
    );*/
        
    
    /* 
    This section handles clicks, two functions are provided, one which shows
    the location of the click and a second which can be used to cycle through points
    connected into GTK.
    */
    /* Show location of mouse-click */
    /* setupGTKAction((void (*)(void *, double, double)) &doAction, NULL); */

    /* Cycles through points. */
    int gtkActionData[2];
    /* Uncomment to set cycle to watchtowers. */
    gtkActionData[0] = WT_CHANGE;
    /* Uncomment to set cycle to polygon vertices. */
    // gtkActionData[0] = POLY_CHANGE;
    /* Uncomment to set cycle to edges. */
    // gtkActionData[0] = EDGE_CHANGE;
    /* 
    The number of click actions triggered, will be twice the number of intended
    clicks. 
    */
    gtkActionData[1] = 0;
    
    setupGTKAction((void (*)(void *, double, double)) &doCyleOnClick, 
                   (void *) &(gtkActionData[0]));
    
    /* Hand over interaction to GTK, handle any interaction through clicks. */
    runGTKInteraction();

    /* NB: GTK has memory leaks so no need to have clean output. */
    freeGTK();
    #endif
    
    /* General cleanup can go here! */
    /* Call functions that frees everything that was dynamically allocated */
    free_csv(&watchtower_list, &num_watchtowers);
    free_vertices(&vertices, num_vertices);
    free(faces);
    free_edges(&edges, num_edges);
    return EXIT_SUCCESS;
}

