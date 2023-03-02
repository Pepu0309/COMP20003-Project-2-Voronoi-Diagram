/* These are helpers for the graphics library, gtk. */
struct gtkContext;

/* Call to start GTK. */
void setupGTK(int *argc, char ***argv, char *title);

/* Set watchtower selected */
void setGTKWatchTower(int vertex);

/* Set vertex selected */
void setGTKVertex(int vertex);

/* Set edge selected */
void setGTKEdge(int edge);

/* Set bounds of GTK - points are scaled and placed based on these values. */
void setupGTKBounds(double newxmin, double newxmax, double newymin, double newymax);

/* Set background points. 
    pointCount -> The number of watchtower datapoints
    displayData -> This is the data (e.g. your watchtower list)
    getDataString -> This function is used to get a string for a given watchTower (currently unused, 
        feel free to make NULL)
    freeDataString -> This function is used to free the string returned by getDataString (to allow
        returning strings which might not need to be freed, also currently unused)
    getPointx -> This function takes the displayData and then gets the x-value of the watchtower at
        the index of the second argument
    getPointy -> This function takes the displayData and then gets the y-value of the watchtower at
        the index of the second argument
*/
void setupGTKPoints(int pointCount, void *displayData, char *(*getDataString)(void *, int), 
    void (*freeDataString)(char *), double (*getPointx)(int, void *), 
    double (*getPointy)(int, void *));

/* Set DCEL points.
    dcelVerticesCount -> The number of vertices in the polygon(s).
    dcel -> The DCEL data structure containing all the polygon faces, edges and vertices.
    getDCELVertexX -> Get the x-coordinate of the vertex at the index of the second argument.
    getDCELVertexY -> Get the y-coordinate of the vertex at the index of the second argument.
*/
void setupGTKPolyPoints(int dcelVerticesCount, void *dcel, 
    double (*getDCELVertexX)(void *, int), double (*getDCELVertexY)(void *, int));

/* Set DCEL edges, hasEdge is checked, hasEdgePair only checked if hasEdge is true.
    edgeCount -> The number of edges in the DCEL polygons.
    dcel -> The DCEL data structure containing all the polygon faces, edges and vertices.
    getDCELEdgeVertexStart -> Gets the index of the vertex at the start of the given edge's
        assigned half-edge, the first argument is the DCEL data structure passed earlier.
        This will only be called if the DCELhasEdge function returns 1 for the edge.
    getDCELEdgeVertexPairStart -> Gets the index of the vertex at the start of the pair of 
        the given given edge's assigned half-edge, the first argument is the DCEL data structure
        passed earlier. This will only be called if the DCELhasEdgePair function returns 1 for 
        the edge.
    getDCELEdgeVertexEnd -> Gets the index of the vertex at the end of the given edge's
        assigned half-edge, the first argument is the DCEL data structure passed earlier.
        This will only be called if the DCELhasEdge function returns 1 for the edge.
    getDCELEdgeVertexPairEnd -> Gets the index of the vertex at the end of the pair of 
        the given given edge's assigned half-edge, the first argument is the DCEL data structure
        passed earlier. This will only be called if the DCELhasEdgePair function returns 1 for 
        the edge.
    DCELhasEdge -> Pointer to a function which returns 1 if the halfEdge associated with the edge is 
        present.
    DCELhasEdgePair -> Pointer to a function which returns 1 if the halfEdge associated with the edge
        has a pair halfEdge which is present.
*/
void setupGTKPolyLines(int edgeCount, void *dcel, 
    int (*getDCELEdgeVertexStart)(void *, int),
    int (*getDCELEdgeVertexPairStart)(void *, int),
    int (*getDCELEdgeVertexEnd)(void *, int),
    int (*getDCELEdgeVertexPairEnd)(void *, int), int (*DCELhasEdge)(void *, int),
    int (*DCELhasEdgePair)(void *, int));

/* Set the action to the given function.
    doAction -> A function which is called twice for every click, the first argument
        is the actionContext passed, the first argument is the x-coordinate and the
        second argument is the y-coordinate.
    actionContext -> A pointer passed to the doAction function along with the coordinates,
        this can be any data.
*/
void setupGTKAction(void (*doAction)(void *, double, double), void *actionContext);

/* Begin interactive GTK portion. */
void runGTKInteraction();

/* Free GTK allocations. */
void freeGTK();
