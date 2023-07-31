#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<assert.h>
#include<string.h>
typedef struct graph *Graph; 
struct Edge {
         int label; 
           int source;
          int  dest;
          float weight;
  };
  
struct graph{
	int vertices;
	int num_edges;
        struct Edge* edges;
};

struct parent_rank{
 	int* parent;
        int* rank;

};

struct parent_rank* new_pr(int v){
	struct parent_rank* new_pr = malloc(sizeof(new_pr));
        new_pr->parent = malloc(sizeof(v));
	new_pr->rank = malloc (sizeof(v));
	return new_pr;
}
void print_outfile (struct Edge edge, FILE *out ){
       fprintf(out,  "%4d: ",  edge.label );
        fprintf(out, "(%d, %d) ", edge.source, edge.dest);
         fprintf(out, "%.1f\n", edge.weight);
}
Graph new_graph(int v, int e){
         Graph g  = malloc(sizeof( Graph));
	 g->vertices = v; 
         g->num_edges = e;
         g->edges = malloc(e * sizeof(struct Edge));
	 return g;  
}
int find(int s, struct parent_rank* pr){
	while( s != pr->parent[s] ){
	       s =  pr->parent[s];
	}
	return s;
}

struct output {
	struct Edge* edge;	
} ;

struct output* new_output (int vert) {
	struct output* out = malloc(sizeof(struct output));
	out->edge = malloc(vert*sizeof(struct Edge));
	return out;
}
void Union (int s, int d, struct parent_rank* pr){
	int source = find(s, pr);
 	int dest = find(d, pr);
	if( source == dest){
		return;
	}
	if( pr->rank[source] > pr->rank[dest]){
		pr->parent[dest] = source; 
	}
	else{
	    pr->parent[source] = dest; 
            if(pr->rank[source] == pr->rank[dest]){
		pr->rank[dest] = pr->rank[dest] + 1; 
	}
	}
}
struct output* Kruskal(Graph g , int v , int e){
        struct output *out = new_output(v); 
   	struct parent_rank *pr = new_pr(v);
	for (int i = 1; i <= v ; i++ ){
       		pr->parent[i] = i; 
		pr->rank[i] = 0;  	
        }
        // 1. set all the edges from low to high weight
        //        sort g->edges[count].weight
        //             bubble sort 
        for (int i = e - 1 ; i >= 0 ; i -= 1){
	    int j = 0;
            while (j < i ){
 		if (g->edges[j].weight  > g->edges[j+1].weight){
                	struct Edge min = g->edges[j];
			g->edges[j] = g->edges[j+1];
			g->edges[j+1] = min; 
		}
		j += 1;
	}
        }
       // for (int i = 0; i < e ; i++){
       // 	printf("%.1f\n", g->edges[i].weight); 
       // }
        int c = 0 ;
        for (int i = 0 ; i < e ; i++){
	
		if(find(g->edges[i].source, pr ) !=find(g->edges[i].dest, pr) ){
 			out->edge[c] = g->edges[i];
                        c+= 1; 
			Union(g->edges[i].source, g->edges[i].dest, pr);	
                }
	}
        return out; 
       	
}
int main(int argc, char * argv[]){
    FILE* infile; 
    FILE* outfile; 
    int vertices;
    int edge_num;
    char buffer[500];  
   
   infile = fopen(argv[1], "r");
   
   if (infile == NULL){
       printf ("reading file error\n");
       exit (EXIT_FAILURE);

   } 
   outfile = fopen(argv[2], "w");
   if (outfile == NULL){
        printf("write file error\n");
        exit(EXIT_FAILURE);
   }
   
   fscanf( infile , "%s" , buffer) ;
   vertices = atoi(buffer);
   //printf("%d", atoi(buffer));   
   fscanf( infile, "%s" , buffer) ;
   edge_num = atoi(buffer);
   //printf("%d", atoi(buffer));
    Graph g = new_graph(vertices, edge_num);
    int count = 0; 
    char line[500] ; 
    while(fscanf (infile, "%s", line ) != EOF){
          g->edges[count].label = count + 1; 
          g->edges[count].source = (atoi(line));
          //printf("%d  ",  g->edges[count].label);
          //printf("%d  ",   g->edges[count].source); 
          fscanf (infile, "%s", line );
          g->edges[count].dest = (atoi(line));
          //printf("%d  ",  g->edges[count].dest);
          fscanf (infile, "%s", line );
          g->edges[count].weight = (atof(line));
           //printf("%.1f  \n",  g->edges[count].weight);
          count += 1; 
  }
    struct output* out  = Kruskal(g, vertices, edge_num);
   float total = 0; 
   for (int i = 0; i < vertices-1; i ++){
 	print_outfile(out->edge[i], outfile);  		
	total += out->edge[i].weight; 
   }
// fprintf(out, "%.1f\n", edge.weight);	     
  
   fprintf(outfile, "Total Weight = %.2f\n", total );
   fclose(infile);
   fclose(outfile);
   return EXIT_SUCCESS;
}
