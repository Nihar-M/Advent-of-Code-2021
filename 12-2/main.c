
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define NODES_MAX 128
#define NODE_CONNECTIONS_MAX 16

typedef struct Node_t
{
    char name[8];
    int cave_type; // 0 is small, 1 is big
    int visited;
    struct Node_t *connections[NODE_CONNECTIONS_MAX];
    int connections_count;
} 
Node_t;

Node_t nodes[NODES_MAX];
int nodes_count = 0;

void dump_node(Node_t *node)
{
    printf("Node name: %s (type=%d)\n", node->name, nodes->cave_type);
    for (int j = 0; j < nodes->connections_count; j++)
        printf("Connected to %s\n", nodes->connections[j]->name);
}

int is_name(Node_t node, char *name)
{
    return !(strncmp(node.name, name, 8));
}

Node_t *get_node(char *name)
{
    for (int i = 0; i < nodes_count; i++)
        if (is_name(nodes[i], name))
            return nodes + i;
    return NULL;
}


// may want to check for duplicate connections
void add_connection(Node_t *a, Node_t *b)
{
    assert(a->connections_count < NODE_CONNECTIONS_MAX - 1);
    a->connections[a->connections_count++] = b;
    
    assert(b->connections_count < NODE_CONNECTIONS_MAX - 1);
    b->connections[b->connections_count++] = a;
}

void add_node (char *name)
{
    if (get_node(name) != NULL) return;
    
    assert(nodes_count < NODES_MAX - 1);
    
    for (int i = 0; i < 8; i++)
        nodes[nodes_count].name[i] = name[i];

    nodes[nodes_count].cave_type = (isupper(nodes[nodes_count].name[0]));
    nodes[nodes_count].connections_count = 0;
    nodes_count++;
}

int main()
{
    char file[] = "input.txt";

    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }
    {    
    char a_name[8];
    char b_name[8];
    while (!feof(fd))
    {
        // fscanf(fd, "%[^-] %[^\n]", a_name, b_name);
        fscanf(fd, "%[^-]", a_name);
        fgetc(fd);
        fscanf(fd, "%[^\n]", b_name);
        
        if (fgetc(fd) != '\n') break;

        printf("Adding nodes (%s||%s)\n", a_name, b_name);
        add_node(a_name);
        add_node(b_name);
        add_connection(get_node(a_name), get_node(b_name));
    }
    }
    
    printf("Node Info\n");
    for (int i = 0; i < nodes_count; i++)
    {
        printf("Node name: %s (type=%d)\n", nodes[i].name, nodes[i].cave_type);
        for (int j = 0; j < nodes[i].connections_count; j++)
            printf("\tConnected to %s\n", nodes[i].connections[j]->name);
    }

    
    int number_of_paths = 0;
    Node_t path[NODES_MAX];
    path[0] = *get_node("start");
    int path_len = 1;

    int used_second_visit = 0;

    while (path_len > 0)
    {
        // printf("Current Path state:\n\t");
        // for (int i = 0; i < path_len; i++)
        //     printf("(%d:%s)", i, path[i].name);
        // printf("\n");

        used_second_visit = 0;
        for (int i = 0; i < path_len; i++)
            if (path[i].cave_type == 0)
                for (int j = i+1; j < path_len; j++)
                    if (is_name(path[i], path[j].name))
                        used_second_visit = 1;

        Node_t *top = (path + path_len-1);
        // dump_node(top);

        if (top->connections_count == 0){
            path_len--;
            // printf("No connections left here\n");
            continue;
        }
        else if (is_name(*(top->connections[top->connections_count-1]),"end")){
            number_of_paths++;
            top->connections_count--;
            // path_len--;
            // printf("Found the ending node\n");
            continue; 
        }       
        else {
            Node_t *next = top->connections[top->connections_count-1];
            if (next->cave_type != 0){
                assert(path_len < NODES_MAX - 2); 
                path[path_len++] = *next;
                top->connections_count--;
                // printf("Big cave found\n");
                continue;
            }
            else {
                int already_visited = 0;
                for (int i = 0; i < path_len; i++)
                    if (is_name(path[i],next->name)){
                        already_visited = 1;
                        break;
                    }
                if ((already_visited && used_second_visit) || is_name(*next, "start")){
                    top->connections_count--;
                    // printf("Already visited small cave\n");
                    continue;
                } 
                else if (already_visited && !used_second_visit) {
                    used_second_visit = 1; 
                    assert(path_len < NODES_MAX - 2); 
                    path[path_len++] = *next;
                    top->connections_count--;
                    // printf("Using second visit to small cave\n");
                    continue;
                } 
                else {
                    assert(path_len < NODES_MAX - 2); 
                    path[path_len++] = *next;
                    top->connections_count--;
                    // printf("Small cave found\n");
                    continue;
                }
            }            
        }
    }

    printf("Number of paths: %d\n", number_of_paths); 
    fclose(fd);
 
    return 0;
}
