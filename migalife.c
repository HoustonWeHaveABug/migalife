#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define RULE_LEN 9
#define MAX_NDS 33554432
#define MAX_BNS 134217728
#define MAX_CELLS 8388608
#define TAB_SIZE 10
#define ALLOC_NDS 65536
#define ALLOC_BNS 262144
#define ALLOC_CELLS 16384
#define CELLS_SIDE 128
#define INPUT_RLE 1
#define INPUT_LIF 2
#define INPUT_HLF 3
#define OUTPUT_NONE 1
#define OUTPUT_VIEW 2
#define OUTPUT_FILE 3
#define BN_DIGITS 9
#define BN_MAX 1000000000
#define LVS_SIZE 65536
#define ON_CELL 65536
#define N1_CELL 32
#define N2_CELL 64
#define N3_CELL 512
#define N4_CELL 1024
#define N1N2_CELLS 96
#define N1N3_CELLS 544
#define N2N4_CELLS 1088
#define N3N4_CELLS 1536
#define N1_MASK 1879
#define N2_MASK 3758
#define N3_MASK 30064
#define N4_MASK 60128
#define UNDEFINED -1
#define EMPTY 0
#define CELL_NW 1
#define CELL_NE 2
#define CELL_SW 16
#define CELL_SE 32
#define QT_NW 51
#define QT_NE 204
#define QT_SW 13056
#define QT_SE 52224
#define HALF_N 255
#define HALF_W 13107
#define HALF_E 52428
#define HALF_S 65280
#define FULL 65535
#define HASH_NW 3
#define HASH_NE 5
#define HASH_SW 9
#define HASH_SE 17
#define MATRIX_SIZE 16
#define CMD_LEN 8

#ifdef LONGLONG_PTR
typedef unsigned long long uinthash_t;
#else
typedef unsigned long uinthash_t;
#endif

typedef struct node_s node_t;

struct node_s {
	int *splk;
	union {
		struct {
			node_t *nw;
			node_t *ne;
			node_t *sw;
			node_t *se;
			node_t *next;
		}
		nds;
		struct {
			int nw;
			int ne;
			int sw;
			int se;
			int next;
		}
		lvs;
	}
	nd;
	node_t *link;
};

typedef struct cell_s cell_t;

struct cell_s {
	int x;
	int y;
	int sn;
	cell_t *link;
};

void usage(char *);
void free_o_nds(void);
void free_cells(void);
void free_globals(void);
void exit_on_error(const char *, ...);
void log_data_operation(int, const char *, const char *);
int bits_set(int);
int next_prime(int);
void update_splk(int *, int *, int *, int *, node_t *);
void alloc_bns(void);
void reset_bns(void);
void new_bn(int, int, int **);
int *init_bn(int, int);
int *copy_bn(int, int *);
void add_bn1(int, int *, int *);
void add_bn2(int, int *, int *);
void add_bn3(int, int **, int *);
int *add_bns(int *, int *);
void print_bn(const char *, int *);
void alloc_r_nds(void);
void g_r_nd(node_t *);
void alloc_f_nds(void);
node_t *g_c_f_nd(void);
void alloc_f_cells(void);
void update_r_nds(int *, int *, int *, int *);
int collect_f_nds(void);
void alloc_nds(void);
node_t *g_nd(void);
void nd_from_nds(node_t *, node_t *, node_t *, node_t *, node_t *);
void nd_from_lvs(int, int, int, int, node_t *nd);
void alloc_o_nds(void);
node_t *g_o_nd(void);
void new_o_nd_from_nds(node_t **);
void new_o_nd_from_lvs(node_t **);
void alloc_cells(void);
void init_cells(void);
void new_hash_from_xy(cell_t *);
void new_cell(int, int, int);
void add_digit(int, int *, char []);
void read_rules(int *, int, int *, char [], int *, char [], int *);
int set_rule(char []);
int read_value(int *);
void read_rle(int *, int *, int *);
void read_lif(int *, int *, int *);
void resize_universe(int *, int *, int *, int *, int *, int *);
void nd_from_cells(int *, node_t **);
int lf_from_value(int);
node_t *nd_from_value(int);
void read_hlf(int *, int *, int *, int *, node_t **);
void new_hash(uinthash_t, node_t *);
node_t *hash_from_nds(node_t *, node_t *, node_t *, node_t *);
node_t *hash_from_lvs(int, int, int, int);
void unmark_nd(int, node_t *);
void set_sb_rules(int, int, int);
void alloc_misc_nds(int);
node_t *canonalize_nd(int, node_t *);
void uproot_nd(int, int *, node_t **);
int update_cell(int, int, int);
int next_qt(int);
void next_qts(node_t *);
int next_lf1(node_t *);
int next_lf2(node_t *);
void next_lfs2(node_t *);
node_t *next_nd(int, node_t *);
void pop_nd(node_t *);
int *gen(int);
void process_nd(int *, int **, node_t **);
void clear_next(int, node_t *);
void process_next_nd(int, int *, int **, node_t **);
void print_cells(const char *);
cell_t *get_hash_from_xy(int, int);
void update_sn(int, int);
void process_cell(int, int, int, cell_t *, int *, cell_t **);
void process_gen(void);
void process_level(int);
void process_gens_loop(int);
void process_gens(int);
void set_rc(int, int, int, int, int *, int *, int *, int *);
void matrix_from_value(int, int, int, int, int);
void matrix_from_lf(int, int, int, int, int);
void matrix_from_nd(int, int, int, int, int, node_t *);
void update_path(char *, node_t *, int *, node_t **, char **);
void write_nd(int l, node_t *);
void output_nd(int, int, char *[], int, node_t *);

char *g_rules, g_sdigits[RULE_LEN+1], g_bdigits[RULE_LEN+1];
int g_hashlife, g_level, *g_gen, g_max_nds, g_collect, g_population, g_verbose, g_max_bns, g_ind_tab_bns, **g_bns, g_ind_bns, g_max_r_nds, g_ind_tab_r_nds, g_ind_r_nds, *g_c_max_r_nds, *g_c_ind_tab_r_nds, *g_c_ind_r_nds, g_max_f_nds, g_ind_tab_f_nds, g_ind_f_nds, g_c_max_f_nds, g_c_ind_tab_f_nds, g_c_ind_f_nds, g_tab_nds, g_ind_tab_nds, *g_h_nds_size, g_ind_nds, *g_free_from_nds, *g_free_from_lvs, *g_save_from_nds, *g_save_from_lvs, g_pops[LVS_SIZE], g_next_qts[LVS_SIZE], g_max_o_nds, g_ind_tab_o_nds, g_ind_o_nds, g_max_f_cells, g_ind_tab_f_cells, g_ind_f_cells, g_c_max_f_cells, g_c_ind_tab_f_cells, g_c_ind_f_cells, g_max_cells, g_cells_side, g_ind_tab_cells, *g_h_cells_size, *g_h_cells_side, g_ind_cells, g_survival, g_birth, g_nd_key, g_lf_n, g_lf_w, g_lf_c, g_lf_e, g_lf_s, g_qt_n, g_qt_w, g_qt_c, g_qt_e, g_qt_s, g_bn_size, g_matrix[MATRIX_SIZE][MATRIX_SIZE];
FILE *g_file;
node_t ***g_r_nds, ***g_f_nds, ***g_h_nds, **g_nds, **g_e_nds, **g_o_nds, **g_nds_n, **g_nds_w, **g_nds_c, **g_nds_e, **g_nds_s;
cell_t ***g_f_cells, ***g_h_cells, **g_cells;

void usage(char *program) {
	fprintf(stderr, "Usage is %s [-h] [-l level | -g generation] [-n nodes] [-c] [-p] [-r rules] [-i input] [-f file] [-o output] [-v]\n", program);
	fflush(stderr);
}

void free_o_nds(void) {
	if (g_o_nds) {
		int i;
		for (i = 0; i <= g_ind_tab_o_nds; i++) {
			free(g_o_nds[i]);
		}
		free(g_o_nds);
		g_o_nds = (node_t **)NULL;
	}
}

void free_cells(void) {
	if (g_cells) {
		int i;
		for (i = 0; i <= g_ind_tab_cells && g_cells[i]; i++) {
			free(g_cells[i]);
		}
		free(g_cells);
		g_cells = (cell_t **)NULL;
	}
	if (g_h_cells_side) {
		free(g_h_cells_side);
		g_h_cells_side = (int *)NULL;
	}
	if (g_h_cells) {
		int i;
		for (i = 0; i <= g_ind_tab_cells && g_h_cells[i]; i++) {
			free(g_h_cells[i]);
		}
		free(g_h_cells);
		g_h_cells = (cell_t ***)NULL;
	}
	if (g_h_cells_size) {
		free(g_h_cells_size);
		g_h_cells_size = (int *)NULL;
	}
}

void free_globals(void) {
	free_o_nds();
	free_cells();
	if (!g_hashlife && g_f_cells) {
		int i;
		for (i = 0; i <= g_ind_tab_f_cells && g_f_cells[i]; i++) {
			free(g_f_cells[i]);
		}
		free(g_f_cells);
	}
	if (g_nds_s) {
		free(g_nds_s);
	}
	if (g_nds_e) {
		free(g_nds_e);
	}
	if (g_nds_c) {
		free(g_nds_c);
	}
	if (g_nds_w) {
		free(g_nds_w);
	}
	if (g_nds_n) {
		free(g_nds_n);
	}
	if (g_e_nds) {
		free(g_e_nds);
	}
	if (g_nds) {
		int i;
		for (i = 0; i <= g_ind_tab_nds && g_nds[i]; i++) {
			free(g_nds[i]);
		}
		free(g_nds);
	}
	if (g_h_nds) {
		int i;
		for (i = 0; i <= g_ind_tab_nds && g_h_nds[i]; i++) {
			free(g_h_nds[i]);
		}
		free(g_h_nds);
	}
	if (g_h_nds_size) {
		free(g_h_nds_size);
	}
	if (g_collect) {
		if (g_f_nds) {
			int i;
			for (i = 0; i <= g_ind_tab_f_nds && g_f_nds[i]; i++) {
				free(g_f_nds[i]);
			}
			free(g_f_nds);
		}
		if (g_c_ind_r_nds) {
			free(g_c_ind_r_nds);
		}
		if (g_c_ind_tab_r_nds) {
			free(g_c_ind_tab_r_nds);
		}
		if (g_c_max_r_nds) {
			free(g_c_max_r_nds);
		}
		if (g_r_nds) {
			int i;
			for (i = 0; i <= g_ind_tab_r_nds && g_r_nds[i]; i++) {
				free(g_r_nds[i]);
			}
			free(g_r_nds);
		}
	}
	if (g_bns) {
		int i;
		for (i = 0; i <= g_ind_tab_bns && g_bns[i]; i++) {
			free(g_bns[i]);
		}
		free(g_bns);
	}
	if (g_file != stdin) {
		fclose(g_file);
	}
	free(g_gen);
}

void exit_on_error(const char *message, ...) {
	va_list args;
	va_start(args, message);
	vfprintf(stderr, message, args);
	va_end(args);
	fprintf(stderr, "\n");
	fflush(stderr);
	free_globals();
	exit(EXIT_FAILURE);
}

void log_data_operation(int size, const char *data, const char *message) {
	if (g_verbose) {
		size == 1 ? fprintf(stderr, "%i %s %s\n", size, data, message):fprintf(stderr, "%i %ss %s\n", size, data, message);
		fflush(stderr);
	}
}

int bits_set(int lf) {
	int b;
	for (b = 0; lf; b++) {
		lf &= lf-1;
	}
	return b;
}

int next_prime(int p) {
	int n;
	p |= 1;
	do {
		for (n = 3; n*n <= p && p%n > 0; n += 2);
		if (n*n <= p) {
			p += 2;
		}
	}
	while (n*n <= p);
	return p;
}

void update_splk(int *from_nds, int *from_lvs, int *to_nds, int *to_lvs, node_t *nd) {
	if (nd->splk == from_nds) {
		update_splk(from_nds, from_lvs, to_nds, to_lvs, nd->nd.nds.nw);
		update_splk(from_nds, from_lvs, to_nds, to_lvs, nd->nd.nds.ne);
		update_splk(from_nds, from_lvs, to_nds, to_lvs, nd->nd.nds.sw);
		update_splk(from_nds, from_lvs, to_nds, to_lvs, nd->nd.nds.se);
		nd->splk = to_nds;
	}
	else if (nd->splk == from_lvs) {
		nd->splk = to_lvs;
	}
}

void alloc_bns(void) {
	if (!g_bns[g_ind_tab_bns]) {
		g_bns[g_ind_tab_bns] = (int *)malloc(sizeof(int)*(size_t)g_max_bns);
		if (!g_bns[g_ind_tab_bns]) {
			exit_on_error("Could not allocate memory for g_bns[%i]", g_ind_tab_bns);
		}
		log_data_operation(g_max_bns, "bignumber", "allocated");
	}
	g_ind_bns = 0;
}

void reset_bns(void) {
	if (g_ind_tab_bns > 0) {
		g_max_bns = ALLOC_BNS;
		g_ind_tab_bns = 0;
	}
	g_ind_bns = 4;
}

void new_bn(int size, int v, int **bn) {
	if (g_ind_tab_bns < TAB_SIZE-1 || g_ind_bns+size+1 < g_max_bns) {
		if (g_ind_bns+size+1 >= g_max_bns) {
			if (g_max_bns < MAX_BNS) {
				g_max_bns <<= 1;
			}
			g_ind_tab_bns++;
			alloc_bns();
		}
		*bn = g_bns[g_ind_tab_bns]+g_ind_bns;
		**bn = v;
		g_ind_bns += size+1;
	}
	else {
		exit_on_error("No more bignumbers available");
	}
}

int *init_bn(int size, int v) {
	int *r = NULL, i;
	new_bn(size, size, &r);
	for (i = 1; i <= size; i++) {
		r[i] = v;
	}
	return r;
}

int *copy_bn(int size, int *bn) {
	int *r = NULL, i;
	new_bn(size, size, &r);
	for (i = 1; i <= *bn; i++) {
		r[i] = bn[i];
	}
	return r;
}

void add_bn1(int size, int *bn1, int *bn2) {
	if (size <= *bn2) {
		bn1[size] += bn2[size];
	}
}

void add_bn2(int size, int *bn1, int *bn2) {
	if (size <= *bn2) {
		bn1[size] += bn2[size];
		if (bn1[size] >= BN_MAX) {
			bn1[size+1] += bn1[size]/BN_MAX;
			bn1[size] %= BN_MAX;
		}
	}
}

void add_bn3(int size, int **bn1, int *bn2) {
	if (size <= *bn2) {
		(*bn1)[size] += bn2[size];
		if ((*bn1)[size] >= BN_MAX) {
			if (size == **bn1) {
				if (g_ind_bns+**bn1+1 <= g_max_bns) {
					(**bn1)++;
					g_ind_bns++;
				}
				else {
					*bn1 = copy_bn(**bn1+1, *bn1);
				}
				(*bn1)[size+1] = 0;
			}
			(*bn1)[size+1] += (*bn1)[size]/BN_MAX;
			(*bn1)[size] %= BN_MAX;
		}
	}
}

int *add_bns(int *bn1, int *bn2) {
	int size, *r, i;
	size = *bn1;
	if (size < *bn2) {
		size = *bn2;
	}
	r = init_bn(size, 0);
	for (i = 1; i < size; i++) {
		add_bn1(i, r, bn1);
		add_bn2(i, r, bn2);
	}
	add_bn1(size, r, bn1);
	add_bn3(size, &r, bn2);
	return r;
}

void print_bn(const char *msg, int *bn) {
	int i;
	fprintf(stderr, "%s %i", msg, bn[*bn]);
	for (i = *bn-1; i > 0; i--) {
		fprintf(stderr, ",%0*i", BN_DIGITS, bn[i]);
	}
	fprintf(stderr, "\n");
	fflush(stderr);
}

void alloc_r_nds(void) {
	if (!g_r_nds[g_ind_tab_r_nds]) {
		g_r_nds[g_ind_tab_r_nds] = (node_t **)malloc(sizeof(node_t *)*(size_t)g_max_r_nds);
		if (!g_r_nds[g_ind_tab_r_nds]) {
			exit_on_error("Could not allocate memory for g_r_nds[%i]", g_ind_tab_r_nds);
		}
		log_data_operation(g_max_r_nds, "root node", "allocated");
	}
	g_ind_r_nds = 0;
}

void g_r_nd(node_t *nd) {
	if (g_ind_tab_r_nds < TAB_SIZE-1 || g_ind_r_nds < g_max_r_nds) {
		if (g_ind_r_nds == g_max_r_nds) {
			if (g_max_r_nds < MAX_NDS) {
				g_max_r_nds <<= 1;
			}
			g_ind_tab_r_nds++;
			alloc_r_nds();
		}
		g_r_nds[g_ind_tab_r_nds][g_ind_r_nds] = nd;
		g_ind_r_nds++;
	}
	else {
		exit_on_error("No more root nodes available");
	}
}

void alloc_f_nds(void) {
	if (!g_f_nds[g_ind_tab_f_nds]) {
		g_f_nds[g_ind_tab_f_nds] = (node_t **)malloc(sizeof(node_t *)*(size_t)g_max_f_nds);
		if (!g_f_nds[g_ind_tab_f_nds]) {
			exit_on_error("Could not allocate memory for g_f_nds[%i]", g_ind_tab_f_nds);
		}
		log_data_operation(g_max_f_nds, "free node", "allocated");
	}
	g_ind_f_nds = 0;
}

node_t *g_c_f_nd(void) {
	node_t *nd;
	if (g_c_ind_f_nds == g_c_max_f_nds) {
		if (g_c_max_f_nds < MAX_NDS) {
			g_c_max_f_nds <<= 1;
		}
		g_c_ind_tab_f_nds++;
		g_c_ind_f_nds = 0;
	}
	nd = g_f_nds[g_c_ind_tab_f_nds][g_c_ind_f_nds];
	g_c_ind_f_nds++;
	return nd;
}

void alloc_f_cells(void) {
	if (!g_f_cells[g_ind_tab_f_cells]) {
		g_f_cells[g_ind_tab_f_cells] = (cell_t **)malloc(sizeof(cell_t *)*(size_t)g_max_f_cells);
		if (!g_f_cells[g_ind_tab_f_cells]) {
			exit_on_error("Could not allocate memory for g_f_cells[%i]", g_ind_tab_f_cells);
		}
		log_data_operation(g_max_f_cells, "free cell", "allocated");
	}
	g_ind_f_cells = 0;
}

void update_r_nds(int *from_nds, int *from_lvs, int *to_nds, int *to_lvs) {
	int i;
	if (g_ind_tab_r_nds > 0) {
		int max_r_nds = ALLOC_NDS;
		for (i = 0; i < g_ind_tab_r_nds-1; i++) {
			int j;
			for (j = 0; j < max_r_nds; j++) {
				if (g_r_nds[i][j]) {
					update_splk(from_nds, from_lvs, to_nds, to_lvs, g_r_nds[i][j]);
				}
			}
			if (max_r_nds < MAX_NDS) {
				max_r_nds <<= 1;
			}
		}
		for (i = 0; i < max_r_nds; i++) {
			if (g_r_nds[g_ind_tab_r_nds-1][i]) {
				update_splk(from_nds, from_lvs, to_nds, to_lvs, g_r_nds[g_ind_tab_r_nds-1][i]);
			}
		}
	}
	for (i = 0; i < g_ind_r_nds; i++) {
		if (g_r_nds[g_ind_tab_r_nds][i]) {
			update_splk(from_nds, from_lvs, to_nds, to_lvs, g_r_nds[g_ind_tab_r_nds][i]);
		}
	}
}

int collect_f_nds(void) {
	int total_f_nds = 0;
	if (g_collect == 2) {
		int i;
		if (g_ind_tab_f_nds > 0) {
			g_max_f_nds = ALLOC_NDS;
			g_ind_tab_f_nds = 0;
			g_c_max_f_nds = ALLOC_NDS;
			g_c_ind_tab_f_nds = 0;
		}
		g_ind_f_nds = 0;
		g_c_ind_f_nds = 0;
		update_r_nds(g_free_from_nds, g_free_from_lvs, g_save_from_nds, g_save_from_lvs);
		for (i = 0; i <= g_ind_tab_nds; i++) {
			int j;
			for (j = 0; j < g_h_nds_size[i]; j++) {
				node_t *l_nd = (node_t *)NULL, *nd;
				for (nd = g_h_nds[i][j]; nd; nd = nd->link) {
					if (nd->splk == g_free_from_nds || nd->splk == g_free_from_lvs) {
						if (l_nd) {
							l_nd->link = nd->link;
						}
						else {
							g_h_nds[i][j] = nd->link;
						}
						if (g_ind_tab_f_nds < TAB_SIZE-1 || g_ind_f_nds < g_max_f_nds) {
							if (g_ind_f_nds == g_max_f_nds) {
								if (g_max_f_nds < MAX_NDS) {
									g_max_f_nds <<= 1;
								}
								g_ind_tab_f_nds++;
								alloc_f_nds();
							}
							g_f_nds[g_ind_tab_f_nds][g_ind_f_nds] = nd;
							g_ind_f_nds++;
						}
						else {
							exit_on_error("No more free nodes available");
						}
						total_f_nds++;
					}
					else if (nd->splk == g_save_from_nds) {
						if (nd->nd.nds.next && (nd->nd.nds.next->splk == g_free_from_nds || nd->nd.nds.next->splk == g_free_from_lvs)) {
							nd->nd.nds.next = (node_t *)NULL;
						}
						l_nd = nd;
					}
					else {
						l_nd = nd;
					}
				}
			}
		}
		update_r_nds(g_save_from_nds, g_save_from_lvs, g_free_from_nds, g_free_from_lvs);
		log_data_operation(total_f_nds, "free node", "collected");
	}
	return total_f_nds;
}

void alloc_nds(void) {
	int i;
	g_h_nds_size[g_ind_tab_nds] = next_prime(g_max_nds);
	g_h_nds[g_ind_tab_nds] = (node_t **)malloc(sizeof(node_t *)*(size_t)g_h_nds_size[g_ind_tab_nds]);
	if (!g_h_nds[g_ind_tab_nds]) {
		exit_on_error("Could not allocate memory for g_h_nds[%i]", g_ind_tab_nds);
	}
	for (i = 0; i < g_h_nds_size[g_ind_tab_nds]; i++) {
		g_h_nds[g_ind_tab_nds][i] = (node_t *)NULL;
	}
	g_nds[g_ind_tab_nds] = (node_t *)malloc(sizeof(node_t)*(size_t)g_max_nds);
	if (!g_nds[g_ind_tab_nds]) {
		exit_on_error("Could not allocate memory for g_nds[%i]", g_ind_tab_nds);
	}
	log_data_operation(g_max_nds, "node", "allocated");
	g_ind_nds = 0;
}

node_t *g_nd(void) {
	node_t *nd;
	if (g_ind_nds == g_max_nds) {
		if (g_max_nds < MAX_NDS) {
			g_max_nds <<= 1;
		}
		g_ind_tab_nds++;
		alloc_nds();
	}
	nd = g_nds[g_ind_tab_nds]+g_ind_nds;
	g_ind_nds++;
	return nd;
}

void nd_from_nds(node_t *nw, node_t *ne, node_t *sw, node_t *se, node_t *nd) {
	nd->splk = g_free_from_nds;
	nd->nd.nds.nw = nw;
	nd->nd.nds.ne = ne;
	nd->nd.nds.sw = sw;
	nd->nd.nds.se = se;
	nd->nd.nds.next = (node_t *)NULL;
	nd->link = (node_t *)NULL;
}

void nd_from_lvs(int nw, int ne, int sw, int se, node_t *nd) {
	nd->splk = g_free_from_lvs;
	nd->nd.lvs.nw = nw;
	nd->nd.lvs.ne = ne;
	nd->nd.lvs.sw = sw;
	nd->nd.lvs.se = se;
	nd->nd.lvs.next = UNDEFINED;
	nd->link = (node_t *)NULL;
}

void alloc_o_nds(void) {
	g_o_nds[g_ind_tab_o_nds] = (node_t *)malloc(sizeof(node_t)*(size_t)g_max_o_nds);
	if (!g_o_nds[g_ind_tab_o_nds]) {
		exit_on_error("Could not allocate memory for g_o_nds[%i]", g_ind_tab_o_nds);
	}
	log_data_operation(g_max_o_nds, "original node", "allocated");
	g_ind_o_nds = 0;
}

node_t *g_o_nd(void) {
	node_t *nd;
	if (g_ind_o_nds == g_max_o_nds) {
		if (g_max_o_nds < MAX_NDS) {
			g_max_o_nds <<= 1;
		}
		g_ind_tab_o_nds++;
		alloc_o_nds();
	}
	nd = g_o_nds[g_ind_tab_o_nds]+g_ind_o_nds;
	g_ind_o_nds++;
	return nd;
}

void new_o_nd_from_nds(node_t **nd) {
	if (g_ind_tab_o_nds < TAB_SIZE-1 || g_ind_o_nds < g_max_o_nds) {
		*nd = g_o_nd();
		nd_from_nds((node_t *)NULL, (node_t *)NULL, (node_t *)NULL, (node_t *)NULL, *nd);
	}
	else {
		exit_on_error("No more original nodes available");
	}
}

void new_o_nd_from_lvs(node_t **nd) {
	if (g_ind_tab_o_nds < TAB_SIZE-1 || g_ind_o_nds < g_max_o_nds) {
		*nd = g_o_nd();
		nd_from_lvs(0, 0, 0, 0, *nd);
	}
	else {
		exit_on_error("No more original nodes available");
	}
}

void alloc_cells(void) {
	int i;
	g_h_cells_size[g_ind_tab_cells] = next_prime(g_max_cells);
	g_h_cells[g_ind_tab_cells] = (cell_t **)malloc(sizeof(cell_t *)*(size_t)g_h_cells_size[g_ind_tab_cells]);
	if (!g_h_cells[g_ind_tab_cells]) {
		exit_on_error("Could not allocate memory for g_h_cells[%i]", g_ind_tab_cells);
	}
	for (i = 0; i < g_h_cells_size[g_ind_tab_cells]; i++) {
		g_h_cells[g_ind_tab_cells][i] = (cell_t *)NULL;
	}
	g_h_cells_side[g_ind_tab_cells] = g_cells_side;
	g_cells[g_ind_tab_cells] = (cell_t *)malloc(sizeof(cell_t)*(size_t)g_max_cells);
	if (!g_cells[g_ind_tab_cells]) {
		exit_on_error("Could not allocate memory for g_cells[%i]", g_ind_tab_cells);
	}
	log_data_operation(g_max_cells, "cell", "allocated");
	g_ind_cells = 0;
}

void init_cells(void) {
	g_max_cells = ALLOC_CELLS;
	g_cells_side = CELLS_SIDE;
	g_ind_tab_cells = 0;
	g_h_cells_size = (int *)malloc(sizeof(int)*TAB_SIZE);
	if (!g_h_cells_size) {
		exit_on_error("Could not allocate memory for g_h_cells_size");
	}
	g_h_cells = (cell_t ***)malloc(sizeof(cell_t **)*TAB_SIZE);
	if (!g_h_cells) {
		exit_on_error("Could not allocate memory for g_h_cells");
	}
	g_h_cells_side = (int *)malloc(sizeof(int)*TAB_SIZE);
	if (!g_h_cells_side) {
		exit_on_error("Could not allocate memory for g_h_cells_side");
	}
	g_cells = (cell_t **)malloc(sizeof(cell_t *)*TAB_SIZE);
	if (!g_cells) {
		exit_on_error("Could not allocate memory for g_cells");
	}
}

void new_hash_from_xy(cell_t *cell) {
	uinthash_t modulo = ((uinthash_t)cell->x*(uinthash_t)g_h_cells_side[g_ind_tab_cells]+(uinthash_t)cell->y)%(uinthash_t)g_h_cells_size[g_ind_tab_cells];
	cell->link = g_h_cells[g_ind_tab_cells][modulo];
	g_h_cells[g_ind_tab_cells][modulo] = cell;
}

void new_cell(int x, int y, int sn) {
	if (g_ind_tab_cells < TAB_SIZE-1 || g_ind_cells < g_max_cells) {
		if (g_ind_cells == g_max_cells) {
			if (g_max_cells < MAX_CELLS) {
				g_max_cells <<= 2;
				g_cells_side <<= 1;
			}
			g_ind_tab_cells++;
			alloc_cells();
		}
		g_cells[g_ind_tab_cells][g_ind_cells].x = x;
		g_cells[g_ind_tab_cells][g_ind_cells].y = y;
		g_cells[g_ind_tab_cells][g_ind_cells].sn = sn;
		new_hash_from_xy(&g_cells[g_ind_tab_cells][g_ind_cells]);
		g_ind_cells++;
	}
	else {
		exit_on_error("No more cells available");
	}
}

void add_digit(int d, int *n, char digits[]) {
	if (d >= '0' && d <= '8' && !strchr(digits, d)) {
		(*n)++;
		digits[*n-2] = (char)d;
	}
}

void read_rules(int *c, int sb, int *n1, char digits1[], int *n2, char digits2[], int *slash) {
	if (!g_rules) {
		for (; *c >= '0' && *c <= '9'; *c = fgetc(g_file)) {
			add_digit(*c, n1, digits1);
		}
		if (*c == '/') {
			*c = fgetc(g_file);
			if (*c == sb) {
				*c = fgetc(g_file);
			}
			else if (*c >= '0' && *c <= '9') {
				add_digit(*c, n2, digits2);
				*c = fgetc(g_file);
			}
			for (; *c >= '0' && *c <= '9'; *c = fgetc(g_file)) {
				add_digit(*c, n2, digits2);
			}
			*slash = 1;
		}
	}
}

int set_rule(char digits[]) {
	int rule = 0, i;
	for (i = 0; digits[i]; i++) {
		if (digits[i] >= '0' && digits[i] <= '8' && !(rule & (1 << (digits[i]-'0')))) {
			rule |= (1 << (digits[i]-'0'));
		}
	}
	return rule;
}

int read_value(int *c) {
	int r = 0, s;
	for (; *c == ' ' || *c == '\t'; *c = fgetc(g_file));
	if (*c == '-') {
		s = -1;
		*c = fgetc(g_file);
	}
	else {
		s = 1;
	}
	for (; *c >= '0' && *c <= '9'; *c = fgetc(g_file)) {
		r = r > 0 ? 10*r+(*c-'0'):*c-'0';
	}
	return r*s;
}

void read_rle(int *ns, int *nb, int *slash) {
	int rp = 0, x = 0, y = 0, c;
	do {
		c = fgetc(g_file);
		if (c == '#') {
			do {
				c = fgetc(g_file);
			}
			while (c != '\n' && c != EOF);
		}
		else if (c == 'x') {
			do {
				c = fgetc(g_file);
				if (c == 'B') {
					c = fgetc(g_file);
					read_rules(&c, 'S', nb, g_bdigits, ns, g_sdigits, slash);
				}
				else if (c == 'S') {
					c = fgetc(g_file);
					read_rules(&c, 'B', ns, g_sdigits, nb, g_bdigits, slash);
				}
			}
			while (c != '\n' && c != EOF);
		}
		else if (c >= '0' && c <= '9') {
			rp = rp > 0 ? 10*rp+(c-'0'):c-'0';
		}
		else if (c == 'b') {
			if (rp == 0) {
				rp = 1;
			}
			x += rp;
			rp = 0;
		}
		else if (c == 'o') {
			int i;
			if (rp == 0) {
				rp = 1;
			}
			for (i = 0; i < rp; i++) {
				new_cell(x, y, ON_CELL);
				x++;
			}
			rp = 0;
		}
		else if (c == '$' || c == '!') {
			if (rp == 0) {
				rp = 1;
			}
			if (x > 0) {
				x = 0;
			}
			y += rp;
			rp = 0;
		}
	}
	while (c != '!' && c != EOF);
	for (; c != EOF; c = fgetc(g_file));
}

void read_lif(int *ns, int *nb, int *slash) {
	int px = 0, x = px, py = 0, c;
	do {
		c = fgetc(g_file);
		if (c == '#') {
			c = fgetc(g_file);
			if (c == 'N') {
				if (!g_rules) {
					add_digit('2', ns, g_sdigits);
					add_digit('3', ns, g_sdigits);
					add_digit('3', nb, g_bdigits);
				}
			}
			else if (c == 'R') {
				do {
					c = fgetc(g_file);
				}
				while (c == ' ' || c == '\t');
				read_rules(&c, 'B', ns, g_sdigits, nb, g_bdigits, slash);
			}
			else if (c == 'P') {
				c = fgetc(g_file);
				px = read_value(&c);
				x = px;
				py = read_value(&c);
			}
			for (; c != '\n' && c != EOF; c = fgetc(g_file));
		}
		else if (c == '.') {
			x++;
		}
		else if (c == '*' || c == 'O' || c == 'o') {
			new_cell(x, py, ON_CELL);
			x++;
		}
		else if (c == '-' || (c >= '0' && c <= '9')) {
			px = read_value(&c);
			x = px;
			py = read_value(&c);
			new_cell(x, py, ON_CELL);
		}
		else if (c == '\n') {
			if (x > px) {
				x = px;
				py++;
			}
		}
	}
	while (c != EOF);
}

void resize_universe(int *l, int *min_x, int *min_y, int *max_x, int *max_y, int *offset) {
	(*l)++;
	*min_x -= *offset;
	*min_y -= *offset;
	*max_x += *offset;
	*max_y += *offset;
	*offset <<= 1;
}

void nd_from_cells(int *max_l, node_t **r_nd) {
	int min_x1 = INT_MAX, min_y1 = INT_MAX, max_x1 = INT_MIN, max_y1 = INT_MIN, min_x2, min_y2, max_x2, max_y2, offset, i;
	for (i = 0; i <= g_ind_tab_cells; i++) {
		int j;
		for (j = 0; j < g_h_cells_size[i]; j++) {
			cell_t *cell;
			for (cell = g_h_cells[i][j]; cell; cell = cell->link) {
				if (cell->x < min_x1) {
					min_x1 = cell->x;
				}
				if (cell->x > max_x1) {
					max_x1 = cell->x;
				}
				if (cell->y < min_y1) {
					min_y1 = cell->y;
				}
				if (cell->y > max_y1) {
					max_y1 = cell->y;
				}
			}
		}
	}
	if (min_x1 > max_x1 || min_y1 > max_y1) {
		min_x1 = 0;
		min_y1 = 0;
		max_x1 = 0;
		max_y1 = 0;
	}
	*max_l = -2;
	min_x2 = (min_x1+max_x1) >> 1;
	min_y2 = (min_y1+max_y1) >> 1;
	max_x2 = min_x2+1;
	max_y2 = min_y2+1;
	offset = 1;
	while (min_x2 > min_x1 || min_y2 > min_y1 || max_x2 < max_x1 || max_y2 < max_y1) {
		resize_universe(max_l, &min_x2, &min_y2, &max_x2, &max_y2, &offset);
	}
	resize_universe(max_l, &min_x2, &min_y2, &max_x2, &max_y2, &offset);
	while (*max_l < 1) {
		resize_universe(max_l, &min_x2, &min_y2, &max_x2, &max_y2, &offset);
	}
	g_max_o_nds = ALLOC_NDS;
	g_ind_tab_o_nds = 0;
	g_o_nds = (node_t **)malloc(sizeof(node_t *)*TAB_SIZE);
	if (!g_o_nds) {
		exit_on_error("Could not allocate memory for g_o_nds");
	}
	alloc_o_nds();
	new_o_nd_from_nds(r_nd);
	for (i = 0; i <= g_ind_tab_cells; i++) {
		int j;
		for (j = 0; j < g_h_cells_size[i]; j++) {
			cell_t *cell;
			for (cell = g_h_cells[i][j]; cell; cell = cell->link) {
				int min_x = min_x2, min_y = min_y2, max_x = max_x2, max_y = max_y2, max_nw_x, max_nw_y, l;
				node_t *nd = *r_nd;
				for (l = *max_l-1; l > 0; l--) {
					max_nw_x = (min_x+max_x) >> 1;
					max_nw_y = (min_y+max_y) >> 1;
					if (cell->x <= max_nw_x) {
						if (cell->y <= max_nw_y) {
							max_x = max_nw_x;
							max_y = max_nw_y;
							if (!nd->nd.nds.nw) {
								new_o_nd_from_nds(&nd->nd.nds.nw);
							}
							nd = nd->nd.nds.nw;
						}
						else {
							min_y = max_nw_y+1;
							max_x = max_nw_x;
							if (!nd->nd.nds.sw) {
								new_o_nd_from_nds(&nd->nd.nds.sw);
							}
							nd = nd->nd.nds.sw;
						}
					}
					else {
						if (cell->y <= max_nw_y) {
							min_x = max_nw_x+1;
							max_y = max_nw_y;
							if (!nd->nd.nds.ne) {
								new_o_nd_from_nds(&nd->nd.nds.ne);
							}
							nd = nd->nd.nds.ne;
						}
						else {
							min_x = max_nw_x+1;
							min_y = max_nw_y+1;
							if (!nd->nd.nds.se) {
								new_o_nd_from_nds(&nd->nd.nds.se);
							}
							nd = nd->nd.nds.se;
						}
					}
				}
				max_nw_x = (min_x+max_x) >> 1;
				max_nw_y = (min_y+max_y) >> 1;
				if (cell->x <= max_nw_x) {
					if (cell->y <= max_nw_y) {
						max_x = max_nw_x;
						max_y = max_nw_y;
						if (!nd->nd.nds.nw) {
							new_o_nd_from_lvs(&nd->nd.nds.nw);
						}
						nd = nd->nd.nds.nw;
					}
					else {
						min_y = max_nw_y+1;
						max_x = max_nw_x;
						if (!nd->nd.nds.sw) {
							new_o_nd_from_lvs(&nd->nd.nds.sw);
						}
						nd = nd->nd.nds.sw;
					}
				}
				else {
					if (cell->y <= max_nw_y) {
						min_x = max_nw_x+1;
						max_y = max_nw_y;
						if (!nd->nd.nds.ne) {
							new_o_nd_from_lvs(&nd->nd.nds.ne);
						}
						nd = nd->nd.nds.ne;
					}
					else {
						min_x = max_nw_x+1;
						min_y = max_nw_y+1;
						if (!nd->nd.nds.se) {
							new_o_nd_from_lvs(&nd->nd.nds.se);
						}
						nd = nd->nd.nds.se;
					}
				}
				max_nw_x = (min_x+max_x) >> 1;
				max_nw_y = (min_y+max_y) >> 1;
				if (cell->x <= max_nw_x) {
					if (cell->y <= max_nw_y) {
						max_x = max_nw_x;
						max_y = max_nw_y;
						nd->nd.lvs.nw |= (1 << ((cell->x-min_x)+((cell->y-min_y) << 2)));
					}
					else {
						min_y = max_nw_y+1;
						max_x = max_nw_x;
						nd->nd.lvs.sw |= (1 << ((cell->x-min_x)+((cell->y-max_nw_y-1) << 2)));
					}
				}
				else {
					if (cell->y <= max_nw_y) {
						min_x = max_nw_x+1;
						max_y = max_nw_y;
						nd->nd.lvs.ne |= (1 << ((cell->x-max_nw_x-1)+((cell->y-min_y) << 2)));
					}
					else {
						min_x = max_nw_x+1;
						min_y = max_nw_y+1;
						nd->nd.lvs.se |= (1 << ((cell->x-max_nw_x-1)+((cell->y-max_nw_y-1) << 2)));
					}
				}
			}
		}
	}
	free_cells();
}

int lf_from_value(int v) {
	return v >= EMPTY && v <= FULL ? v:UNDEFINED;
}

node_t *nd_from_value(int v) {
	if (v > 0) {
		if (g_tab_nds > 1) {
			int ind_tab_nds = 0, sum_nds = 0, max_nds = ALLOC_NDS;
			while (ind_tab_nds <= g_ind_tab_nds && sum_nds+max_nds < v) {
				ind_tab_nds++;
				sum_nds += max_nds;
				if (max_nds < MAX_NDS) {
					max_nds <<= 1;
				}
			}
			return ind_tab_nds < g_ind_tab_nds || (ind_tab_nds == g_ind_tab_nds && v <= sum_nds+g_ind_nds) ? g_nds[ind_tab_nds]+v-1-sum_nds:(node_t *)NULL;
		}
		else {
			return v <= g_ind_nds ? g_nds[0]+v-1:(node_t *)NULL;
		}
	}
	else {
		return (node_t *)NULL;
	}
}

void read_hlf(int *ns, int *nb, int *slash, int *max_l, node_t **c_nd) {
	int nd_key = 1, c;
	*max_l = 0;
	do {
		c = fgetc(g_file);
		if (c == 'R') {
			do {
				c = fgetc(g_file);
			}
			while (c == ' ' || c == '\t');
			read_rules(&c, 'B', ns, g_sdigits, nb, g_bdigits, slash);
		}
		else if (c == '-' || (c >= '0' && c <= '9')) {
			int l = read_value(&c), nw = read_value(&c), ne = read_value(&c), sw = read_value(&c), se = read_value(&c);
			if (l > 0) {
				node_t *nd_nw = nd_from_value(nw), *nd_ne = nd_from_value(ne), *nd_sw = nd_from_value(sw), *nd_se = nd_from_value(se);
				if (nd_nw && *(nd_nw->splk) == l-1 && nd_ne && *(nd_ne->splk) == l-1 && nd_sw && *(nd_sw->splk) == l-1 && nd_se && *(nd_se->splk) == l-1) {
					*c_nd = hash_from_nds(nd_nw, nd_ne, nd_sw, nd_se);
					new_bn(0, l, &(*c_nd)->splk);
					if (l > *max_l) {
						*max_l = l;
					}
				}
				else {
					exit_on_error("Unable to build node %i", nd_key);
				}
				nd_key++;
			}
			else if (l == 0) {
				int lf_nw = lf_from_value(nw), lf_ne = lf_from_value(ne), lf_sw = lf_from_value(sw), lf_se = lf_from_value(se);
				if (lf_nw > UNDEFINED && lf_ne > UNDEFINED && lf_sw > UNDEFINED && lf_se > UNDEFINED) {
					*c_nd = hash_from_lvs(lf_nw, lf_ne, lf_sw, lf_se);
					new_bn(0, l, &(*c_nd)->splk);
					if (l > *max_l) {
						*max_l = l;
					}
				}
				else {
					exit_on_error("Unable to build node %i", nd_key);
				}
				nd_key++;
			}
			else {
				exit_on_error("Node level must be greater than or equal to 0");
			}
		}
		for (; c != '\n' && c != EOF; c = fgetc(g_file));
	}
	while (c != EOF);
	if (*max_l < 1) {
		exit_on_error("End of file reached too soon");
	}
}

void new_hash(uinthash_t hash, node_t *nd) {
	uinthash_t modulo = hash%(uinthash_t)g_h_nds_size[g_ind_tab_nds];
	nd->link = g_h_nds[g_ind_tab_nds][modulo];
	g_h_nds[g_ind_tab_nds][modulo] = nd;
}

node_t *hash_from_nds(node_t *nw, node_t *ne, node_t *sw, node_t *se) {
	int i = g_ind_tab_nds;
	uinthash_t hash = (uinthash_t)nw+HASH_NW*((uinthash_t)ne+HASH_NE*((uinthash_t)sw+HASH_SW*(uinthash_t)se+HASH_SE));
	node_t *nd;
	do {
		for (nd = g_h_nds[i][hash%(uinthash_t)g_h_nds_size[i]]; nd && (*(nd->splk) == 0 || nd->nd.nds.nw != nw || nd->nd.nds.ne != ne || nd->nd.nds.sw != sw || nd->nd.nds.se != se); nd = nd->link);
		i--;
	}
	while (i >= 0 && !nd);
	if (!nd) {
		if (g_collect) {
			if (g_c_ind_tab_f_nds < g_ind_tab_f_nds || g_c_ind_f_nds < g_ind_f_nds) {
				nd = g_c_f_nd();
				nd_from_nds(nw, ne, sw, se, nd);
			}
			else {
				if (g_ind_tab_nds < g_tab_nds-1 || g_ind_nds < g_max_nds) {
					nd = g_nd();
					nd_from_nds(nw, ne, sw, se, nd);
				}
				else {
					if (collect_f_nds()) {
						nd = g_c_f_nd();
						nd_from_nds(nw, ne, sw, se, nd);
					}
					else {
						exit_on_error("No more nodes available");
					}
				}
			}
		}
		else {
			if (g_ind_tab_nds < g_tab_nds-1 || g_ind_nds < g_max_nds) {
				nd = g_nd();
				nd_from_nds(nw, ne, sw, se, nd);
			}
			else {
				exit_on_error("No more nodes available");
			}
		}
		new_hash(hash, nd);
	}
	if (g_collect) {
		g_r_nd(nd);
	}
	return nd;
}

node_t *hash_from_lvs(int nw, int ne, int sw, int se) {
	int i = g_ind_tab_nds;
	uinthash_t hash = (uinthash_t)nw+HASH_NW*((uinthash_t)ne+HASH_NE*((uinthash_t)sw+HASH_SW*(uinthash_t)se+HASH_SE));
	node_t *nd;
	do {
		for (nd = g_h_nds[i][hash%(uinthash_t)g_h_nds_size[i]]; nd && (*(nd->splk) > 0 || nd->nd.lvs.nw != nw || nd->nd.lvs.ne != ne || nd->nd.lvs.sw != sw || nd->nd.lvs.se != se); nd = nd->link);
		i--;
	}
	while (i >= 0 && !nd);
	if (!nd) {
		if (g_collect) {
			if (g_c_ind_tab_f_nds < g_ind_tab_f_nds || g_c_ind_f_nds < g_ind_f_nds) {
				nd = g_c_f_nd();
				nd_from_lvs(nw, ne, sw, se, nd);
			}
			else {
				if (g_ind_tab_nds < g_tab_nds-1 || g_ind_nds < g_max_nds) {
					nd = g_nd();
					nd_from_lvs(nw, ne, sw, se, nd);
				}
				else {
					if (collect_f_nds()) {
						nd = g_c_f_nd();
						nd_from_lvs(nw, ne, sw, se, nd);
					}
					else {
						exit_on_error("No more nodes available");
					}
				}
			}
		}
		else {
			if (g_ind_tab_nds < g_tab_nds-1 || g_ind_nds < g_max_nds) {
				nd = g_nd();
				nd_from_lvs(nw, ne, sw, se, nd);
			}
			else {
				exit_on_error("No more nodes available");
			}
		}
		new_hash(hash, nd);
	}
	if (g_collect) {
		g_r_nd(nd);
	}
	return nd;
}

void unmark_nd(int l, node_t *nd) {
	if (nd->splk != g_free_from_nds && nd->splk != g_free_from_lvs) {
		if (l > 0) {
			unmark_nd(l-1, nd->nd.nds.nw);
			unmark_nd(l-1, nd->nd.nds.ne);
			unmark_nd(l-1, nd->nd.nds.sw);
			unmark_nd(l-1, nd->nd.nds.se);
			nd->splk = g_free_from_nds;
		}
		else {
			nd->splk = g_free_from_lvs;
		}
	}
}

void set_sb_rules(int ns, int nb, int slash) {
	if (g_rules) {
		int i;
		for (i = 0; g_rules[i] && g_rules[i] >= '0' && g_rules[i] <= '9'; i++) {
			add_digit(g_rules[i], &ns, g_sdigits);
		}
		if (g_rules[i] == '/') {
			for (i++; g_rules[i] && g_rules[i] >= '0' && g_rules[i] <= '9'; i++) {
				add_digit(g_rules[i], &nb, g_bdigits);
			}
			slash = 1;
		}
	}
	if (ns == 1 && nb == 1 && !slash) {
		add_digit('2', &ns, g_sdigits);
		add_digit('3', &ns, g_sdigits);
		add_digit('3', &nb, g_bdigits);
	}
	g_sdigits[ns-1] = '\0';
	g_bdigits[nb-1] = '\0';
	g_survival = set_rule(g_sdigits);
	g_birth = set_rule(g_bdigits);
	fprintf(stderr, "Rules set to %s/%s\n", g_sdigits, g_bdigits);
	fflush(stderr);
}

void alloc_misc_nds(int max_l) {
	int i;
	if (g_collect) {
		g_c_max_r_nds = (int *)malloc(sizeof(int)*(size_t)max_l);
		if (!g_c_max_r_nds) {
			exit_on_error("Could not allocate memory for g_c_max_r_nds");
		}
		g_c_ind_tab_r_nds = (int *)malloc(sizeof(int)*(size_t)max_l);
		if (!g_c_ind_tab_r_nds) {
			exit_on_error("Could not allocate memory for g_c_ind_tab_r_nds");
		}
		g_c_ind_r_nds = (int *)malloc(sizeof(int)*(size_t)max_l);
		if (!g_c_ind_r_nds) {
			exit_on_error("Could not allocate memory for g_c_ind_r_nds");
		}
	}
	g_e_nds = (node_t **)malloc(sizeof(node_t *)*(size_t)(max_l+1));
	if (!g_e_nds) {
		exit_on_error("Could not allocate memory for g_e_nds");
	}
	g_e_nds[0] = hash_from_lvs(0, 0, 0, 0);
	for (i = 1; i <= max_l; i++) {
		g_e_nds[i] = hash_from_nds(g_e_nds[i-1], g_e_nds[i-1], g_e_nds[i-1], g_e_nds[i-1]);
	}
}

node_t *canonalize_nd(int l, node_t *nd) {
	return l > 0 ? hash_from_nds(!nd->nd.nds.nw ? g_e_nds[l-1]:canonalize_nd(l-1, nd->nd.nds.nw), !nd->nd.nds.ne ? g_e_nds[l-1]:canonalize_nd(l-1, nd->nd.nds.ne), !nd->nd.nds.sw ? g_e_nds[l-1]:canonalize_nd(l-1, nd->nd.nds.sw), !nd->nd.nds.se ? g_e_nds[l-1]:canonalize_nd(l-1, nd->nd.nds.se)):hash_from_lvs(nd->nd.lvs.nw, nd->nd.lvs.ne, nd->nd.lvs.sw, nd->nd.lvs.se);
}

void uproot_nd(int max_l, int *l, node_t **nd) {
	if (*l < max_l) {
		node_t **nd_ptr_ptr;
		if (g_collect) {
			int *int_ptr = (int *)realloc(g_c_max_r_nds, sizeof(int)*(size_t)max_l);
			if (!int_ptr) {
				exit_on_error("Could not reallocate memory for g_c_max_r_nds");
			}
			g_c_max_r_nds = int_ptr;
			int_ptr = (int *)realloc(g_c_ind_tab_r_nds, sizeof(int)*(size_t)max_l);
			if (!int_ptr) {
				exit_on_error("Could not reallocate memory for g_c_ind_tab_r_nds");
			}
			g_c_ind_tab_r_nds = int_ptr;
			int_ptr = (int *)realloc(g_c_ind_r_nds, sizeof(int)*(size_t)max_l);
			if (!int_ptr) {
				exit_on_error("Could not reallocate memory for g_c_ind_r_nds");
			}
			g_c_ind_r_nds = int_ptr;
		}
		nd_ptr_ptr = (node_t **)realloc(g_e_nds, sizeof(node_t *)*(size_t)(max_l+1));
		if (!nd_ptr_ptr) {
			exit_on_error("Could not reallocate memory for g_e_nds");
		}
		g_e_nds = nd_ptr_ptr;
		while (*l < max_l) {
			(*l)++;
			g_e_nds[*l] = hash_from_nds(g_e_nds[*l-1], g_e_nds[*l-1], g_e_nds[*l-1], g_e_nds[*l-1]);
			*nd = hash_from_nds(hash_from_nds(g_e_nds[*l-2], g_e_nds[*l-2], g_e_nds[*l-2], (*nd)->nd.nds.nw), hash_from_nds(g_e_nds[*l-2], g_e_nds[*l-2], (*nd)->nd.nds.ne, g_e_nds[*l-2]), hash_from_nds(g_e_nds[*l-2], (*nd)->nd.nds.sw, g_e_nds[*l-2], g_e_nds[*l-2]), hash_from_nds((*nd)->nd.nds.se, g_e_nds[*l-2], g_e_nds[*l-2], g_e_nds[*l-2]));
		}
		nd_ptr_ptr = (node_t **)realloc(g_nds_n, sizeof(node_t *)*(size_t)max_l);
		if (!nd_ptr_ptr) {
			exit_on_error("Could not reallocate memory for g_nds_n");
		}
		g_nds_n = nd_ptr_ptr;
		nd_ptr_ptr = (node_t **)realloc(g_nds_w, sizeof(node_t *)*(size_t)max_l);
		if (!nd_ptr_ptr) {
			exit_on_error("Could not reallocate memory for g_nds_w");
		}
		g_nds_w = nd_ptr_ptr;
		nd_ptr_ptr = (node_t **)realloc(g_nds_c, sizeof(node_t *)*(size_t)max_l);
		if (!nd_ptr_ptr) {
			exit_on_error("Could not reallocate memory for g_nds_c");
		}
		g_nds_c = nd_ptr_ptr;
		nd_ptr_ptr = (node_t **)realloc(g_nds_e, sizeof(node_t *)*(size_t)max_l);
		if (!nd_ptr_ptr) {
			exit_on_error("Could not reallocate memory for g_nds_e");
		}
		g_nds_e = nd_ptr_ptr;
		nd_ptr_ptr = (node_t **)realloc(g_nds_s, sizeof(node_t *)*(size_t)max_l);
		if (!nd_ptr_ptr) {
			exit_on_error("Could not reallocate memory for g_nds_s");
		}
		g_nds_s = nd_ptr_ptr;
		if (g_verbose) {
			fprintf(stderr, "Level set to %i\n", *l);
			fflush(stderr);
		}
	}
}

int update_cell(int lf, int n_cell, int n_mask) {
	int v = 0;
	if (lf & n_cell) {
		if (g_survival & (1 << bits_set(lf & n_mask))) {
			v |= n_cell;
		}
	}
	else {
		if (g_birth & (1 << bits_set(lf & n_mask))) {
			v |= n_cell;
		}
	}
	return v;
}

int next_qt(int lf) {
	if (g_next_qts[lf] == UNDEFINED) {
		g_next_qts[lf] = 0;
		g_next_qts[lf] |= update_cell(lf, N1_CELL, N1_MASK);
		g_next_qts[lf] |= update_cell(lf, N2_CELL, N2_MASK);
		g_next_qts[lf] |= update_cell(lf, N3_CELL, N3_MASK);
		g_next_qts[lf] |= update_cell(lf, N4_CELL, N4_MASK);
	}
	return g_next_qts[lf];
}

void next_qts(node_t *nd) {
	g_qt_n = next_qt(((nd->nd.lvs.nw & HALF_E) >> 2) | ((nd->nd.lvs.ne & HALF_W) << 2));
	g_qt_w = next_qt(((nd->nd.lvs.nw & HALF_S) >> 8) | ((nd->nd.lvs.sw & HALF_N) << 8));
	g_qt_c = next_qt(((nd->nd.lvs.nw & QT_SE) >> 10) | ((nd->nd.lvs.ne & QT_SW) >> 6) | ((nd->nd.lvs.sw & QT_NE) << 6) | ((nd->nd.lvs.se & QT_NW) << 10));
	g_qt_e = next_qt(((nd->nd.lvs.ne & HALF_S) >> 8) | ((nd->nd.lvs.se & HALF_N) << 8));
	g_qt_s = next_qt(((nd->nd.lvs.sw & HALF_E) >> 2) | ((nd->nd.lvs.se & HALF_W) << 2));
}

int next_lf1(node_t *nd) {
	if (nd->nd.lvs.next == UNDEFINED) {
		next_qts(nd);
		nd->nd.lvs.next = ((next_qt(nd->nd.lvs.nw) & N4_CELL) >> 10) | ((g_qt_n & N3N4_CELLS) >> 8) | ((next_qt(nd->nd.lvs.ne) & N3_CELL) >> 6) | ((g_qt_w & N2N4_CELLS) >> 2) | g_qt_c | ((g_qt_e & N1N3_CELLS) << 2) | ((next_qt(nd->nd.lvs.sw) & N2_CELL) << 6) | ((g_qt_s & N1N2_CELLS) << 8) | ((next_qt(nd->nd.lvs.se) & N1_CELL) << 10);
	}
	return nd->nd.lvs.next;
}

int next_lf2(node_t *nd) {
	if (nd->nd.lvs.next == UNDEFINED) {
		next_qts(nd);
		nd->nd.lvs.next = (next_qt((next_qt(nd->nd.lvs.nw) >> 5) | (g_qt_n >> 3) | (g_qt_w << 3) | (g_qt_c << 5)) >> 5) | (next_qt((g_qt_n >> 5) | (next_qt(nd->nd.lvs.ne) >> 3) | (g_qt_c << 3) | (g_qt_e << 5)) >> 3) | (next_qt((g_qt_w >> 5) | (g_qt_c >> 3) | (next_qt(nd->nd.lvs.sw) << 3) | (g_qt_s << 5)) << 3) | (next_qt((g_qt_c >> 5) | (g_qt_e >> 3) | (g_qt_s << 3) | (next_qt(nd->nd.lvs.se) << 5)) << 5);
	}
	return nd->nd.lvs.next;
}

void next_lfs2(node_t *nd) {
	g_lf_n = next_lf2(hash_from_lvs(nd->nd.nds.nw->nd.lvs.ne, nd->nd.nds.ne->nd.lvs.nw, nd->nd.nds.nw->nd.lvs.se, nd->nd.nds.ne->nd.lvs.sw));
	g_lf_w = next_lf2(hash_from_lvs(nd->nd.nds.nw->nd.lvs.sw, nd->nd.nds.nw->nd.lvs.se, nd->nd.nds.sw->nd.lvs.nw, nd->nd.nds.sw->nd.lvs.ne));
	g_lf_c = next_lf2(hash_from_lvs(nd->nd.nds.nw->nd.lvs.se, nd->nd.nds.ne->nd.lvs.sw, nd->nd.nds.sw->nd.lvs.ne, nd->nd.nds.se->nd.lvs.nw));
	g_lf_e = next_lf2(hash_from_lvs(nd->nd.nds.ne->nd.lvs.sw, nd->nd.nds.ne->nd.lvs.se, nd->nd.nds.se->nd.lvs.nw, nd->nd.nds.se->nd.lvs.ne));
	g_lf_s = next_lf2(hash_from_lvs(nd->nd.nds.sw->nd.lvs.ne, nd->nd.nds.se->nd.lvs.nw, nd->nd.nds.sw->nd.lvs.se, nd->nd.nds.se->nd.lvs.sw));
}

node_t *next_nd(int l, node_t *nd) {
	if (g_collect) {
		g_c_max_r_nds[l] = g_max_r_nds;
		g_c_ind_tab_r_nds[l] = g_ind_tab_r_nds;
		g_c_ind_r_nds[l] = g_ind_r_nds;
		g_r_nd((node_t *)NULL);
	}
	if (!nd->nd.nds.next) {
		if (l >= g_level-1) {
			if (l > 0) {
				g_nds_n[l] = next_nd(l-1, hash_from_nds(nd->nd.nds.nw->nd.nds.ne, nd->nd.nds.ne->nd.nds.nw, nd->nd.nds.nw->nd.nds.se, nd->nd.nds.ne->nd.nds.sw));
				g_nds_w[l] = next_nd(l-1, hash_from_nds(nd->nd.nds.nw->nd.nds.sw, nd->nd.nds.nw->nd.nds.se, nd->nd.nds.sw->nd.nds.nw, nd->nd.nds.sw->nd.nds.ne));
				g_nds_c[l] = next_nd(l-1, hash_from_nds(nd->nd.nds.nw->nd.nds.se, nd->nd.nds.ne->nd.nds.sw, nd->nd.nds.sw->nd.nds.ne, nd->nd.nds.se->nd.nds.nw));
				g_nds_e[l] = next_nd(l-1, hash_from_nds(nd->nd.nds.ne->nd.nds.sw, nd->nd.nds.ne->nd.nds.se, nd->nd.nds.se->nd.nds.nw, nd->nd.nds.se->nd.nds.ne));
				g_nds_s[l] = next_nd(l-1, hash_from_nds(nd->nd.nds.sw->nd.nds.ne, nd->nd.nds.se->nd.nds.nw, nd->nd.nds.sw->nd.nds.se, nd->nd.nds.se->nd.nds.sw));
				nd->nd.nds.next = l > 1 ? hash_from_nds(hash_from_nds(next_nd(l-1, nd->nd.nds.nw)->nd.nds.se, g_nds_n[l]->nd.nds.sw, g_nds_w[l]->nd.nds.ne, g_nds_c[l]->nd.nds.nw), hash_from_nds(g_nds_n[l]->nd.nds.se, next_nd(l-1, nd->nd.nds.ne)->nd.nds.sw, g_nds_c[l]->nd.nds.ne, g_nds_e[l]->nd.nds.nw), hash_from_nds(g_nds_w[l]->nd.nds.se, g_nds_c[l]->nd.nds.sw, next_nd(l-1, nd->nd.nds.sw)->nd.nds.ne, g_nds_s[l]->nd.nds.nw), hash_from_nds(g_nds_c[l]->nd.nds.se, g_nds_e[l]->nd.nds.sw, g_nds_s[l]->nd.nds.ne, next_nd(l-1, nd->nd.nds.se)->nd.nds.nw)):hash_from_nds(hash_from_lvs(next_nd(l-1, nd->nd.nds.nw)->nd.lvs.se, g_nds_n[l]->nd.lvs.sw, g_nds_w[l]->nd.lvs.ne, g_nds_c[l]->nd.lvs.nw), hash_from_lvs(g_nds_n[l]->nd.lvs.se, next_nd(l-1, nd->nd.nds.ne)->nd.lvs.sw, g_nds_c[l]->nd.lvs.ne, g_nds_e[l]->nd.lvs.nw), hash_from_lvs(g_nds_w[l]->nd.lvs.se, g_nds_c[l]->nd.lvs.sw, next_nd(l-1, nd->nd.nds.sw)->nd.lvs.ne, g_nds_s[l]->nd.lvs.nw), hash_from_lvs(g_nds_c[l]->nd.lvs.se, g_nds_e[l]->nd.lvs.sw, g_nds_s[l]->nd.lvs.ne, next_nd(l-1, nd->nd.nds.se)->nd.lvs.nw));
			}
			else {
				if (g_level == 0) {
					g_lf_n = next_lf1(hash_from_lvs(nd->nd.nds.nw->nd.lvs.ne, nd->nd.nds.ne->nd.lvs.nw, nd->nd.nds.nw->nd.lvs.se, nd->nd.nds.ne->nd.lvs.sw));
					g_lf_w = next_lf1(hash_from_lvs(nd->nd.nds.nw->nd.lvs.sw, nd->nd.nds.nw->nd.lvs.se, nd->nd.nds.sw->nd.lvs.nw, nd->nd.nds.sw->nd.lvs.ne));
					g_lf_c = next_lf1(hash_from_lvs(nd->nd.nds.nw->nd.lvs.se, nd->nd.nds.ne->nd.lvs.sw, nd->nd.nds.sw->nd.lvs.ne, nd->nd.nds.se->nd.lvs.nw));
					g_lf_e = next_lf1(hash_from_lvs(nd->nd.nds.ne->nd.lvs.sw, nd->nd.nds.ne->nd.lvs.se, nd->nd.nds.se->nd.lvs.nw, nd->nd.nds.se->nd.lvs.ne));
					g_lf_s = next_lf1(hash_from_lvs(nd->nd.nds.sw->nd.lvs.ne, nd->nd.nds.se->nd.lvs.nw, nd->nd.nds.sw->nd.lvs.se, nd->nd.nds.se->nd.lvs.sw));
					nd->nd.nds.next = hash_from_lvs(((next_lf1(nd->nd.nds.nw) & QT_SE) >> 10) | ((g_lf_n & QT_SW) >> 6) | ((g_lf_w & QT_NE) << 6) | ((g_lf_c & QT_NW) << 10), ((g_lf_n & QT_SE) >> 10) | ((next_lf1(nd->nd.nds.ne) & QT_SW) >> 6) | ((g_lf_c & QT_NE) << 6) | ((g_lf_e & QT_NW) << 10), ((g_lf_w & QT_SE) >> 10) | ((g_lf_c & QT_SW) >> 6) | ((next_lf1(nd->nd.nds.sw) & QT_NE) << 6) | ((g_lf_s & QT_NW) << 10), ((g_lf_c & QT_SE) >> 10) | ((g_lf_e & QT_SW) >> 6) | ((g_lf_s & QT_NE) << 6) | ((next_lf1(nd->nd.nds.se) & QT_NW) << 10));
				}
				else {
					next_lfs2(nd);
					nd->nd.nds.next = hash_from_lvs(((next_lf2(nd->nd.nds.nw) & QT_SE) >> 10) | ((g_lf_n & QT_SW) >> 6) | ((g_lf_w & QT_NE) << 6) | ((g_lf_c & QT_NW) << 10), ((g_lf_n & QT_SE) >> 10) | ((next_lf2(nd->nd.nds.ne) & QT_SW) >> 6) | ((g_lf_c & QT_NE) << 6) | ((g_lf_e & QT_NW) << 10), ((g_lf_w & QT_SE) >> 10) | ((g_lf_c & QT_SW) >> 6) | ((next_lf2(nd->nd.nds.sw) & QT_NE) << 6) | ((g_lf_s & QT_NW) << 10), ((g_lf_c & QT_SE) >> 10) | ((g_lf_e & QT_SW) >> 6) | ((g_lf_s & QT_NE) << 6) | ((next_lf2(nd->nd.nds.se) & QT_NW) << 10));
				}
			}
		}
		else {
			if (l > 0) {
				g_nds_n[l] = next_nd(l-1, hash_from_nds(nd->nd.nds.nw->nd.nds.ne, nd->nd.nds.ne->nd.nds.nw, nd->nd.nds.nw->nd.nds.se, nd->nd.nds.ne->nd.nds.sw));
				g_nds_w[l] = next_nd(l-1, hash_from_nds(nd->nd.nds.nw->nd.nds.sw, nd->nd.nds.nw->nd.nds.se, nd->nd.nds.sw->nd.nds.nw, nd->nd.nds.sw->nd.nds.ne));
				g_nds_c[l] = next_nd(l-1, hash_from_nds(nd->nd.nds.nw->nd.nds.se, nd->nd.nds.ne->nd.nds.sw, nd->nd.nds.sw->nd.nds.ne, nd->nd.nds.se->nd.nds.nw));
				g_nds_e[l] = next_nd(l-1, hash_from_nds(nd->nd.nds.ne->nd.nds.sw, nd->nd.nds.ne->nd.nds.se, nd->nd.nds.se->nd.nds.nw, nd->nd.nds.se->nd.nds.ne));
				g_nds_s[l] = next_nd(l-1, hash_from_nds(nd->nd.nds.sw->nd.nds.ne, nd->nd.nds.se->nd.nds.nw, nd->nd.nds.sw->nd.nds.se, nd->nd.nds.se->nd.nds.sw));
				nd->nd.nds.next = hash_from_nds(next_nd(l-1, hash_from_nds(next_nd(l-1, nd->nd.nds.nw), g_nds_n[l], g_nds_w[l], g_nds_c[l])), next_nd(l-1, hash_from_nds(g_nds_n[l], next_nd(l-1, nd->nd.nds.ne), g_nds_c[l], g_nds_e[l])), next_nd(l-1, hash_from_nds(g_nds_w[l], g_nds_c[l], next_nd(l-1, nd->nd.nds.sw), g_nds_s[l])), next_nd(l-1, hash_from_nds(g_nds_c[l], g_nds_e[l], g_nds_s[l], next_nd(l-1, nd->nd.nds.se))));
			}
			else {
				next_lfs2(nd);
				nd->nd.nds.next = hash_from_lvs(next_lf2(hash_from_lvs(next_lf2(nd->nd.nds.nw), g_lf_n, g_lf_w, g_lf_c)), next_lf2(hash_from_lvs(g_lf_n, next_lf2(nd->nd.nds.ne), g_lf_c, g_lf_e)), next_lf2(hash_from_lvs(g_lf_w, g_lf_c, next_lf2(nd->nd.nds.sw), g_lf_s)), next_lf2(hash_from_lvs(g_lf_c, g_lf_e, g_lf_s, next_lf2(nd->nd.nds.se))));
			}
		}
	}
	if (g_collect) {
		g_max_r_nds = g_c_max_r_nds[l];
		g_ind_tab_r_nds = g_c_ind_tab_r_nds[l];
		g_ind_r_nds = g_c_ind_r_nds[l];
		g_r_nd(nd->nd.nds.next);
	}
	return nd->nd.nds.next;
}

void pop_nd(node_t *nd) {
	if (nd->splk == g_free_from_nds) {
		int i;
		pop_nd(nd->nd.nds.nw);
		pop_nd(nd->nd.nds.ne);
		pop_nd(nd->nd.nds.sw);
		pop_nd(nd->nd.nds.se);
		g_bn_size = *(nd->nd.nds.nw->splk);
		if (g_bn_size < *(nd->nd.nds.ne->splk)) {
			g_bn_size = *(nd->nd.nds.ne->splk);
		}
		if (g_bn_size < *(nd->nd.nds.sw->splk)) {
			g_bn_size = *(nd->nd.nds.sw->splk);
		}
		if (g_bn_size < *(nd->nd.nds.se->splk)) {
			g_bn_size = *(nd->nd.nds.se->splk);
		}
		nd->splk = init_bn(g_bn_size, 0);
		for (i = 1; i < g_bn_size; i++) {
			add_bn1(i, nd->splk, nd->nd.nds.nw->splk);
			add_bn2(i, nd->splk, nd->nd.nds.ne->splk);
			add_bn2(i, nd->splk, nd->nd.nds.sw->splk);
			add_bn2(i, nd->splk, nd->nd.nds.se->splk);
		}
		add_bn1(g_bn_size, nd->splk, nd->nd.nds.nw->splk);
		add_bn3(g_bn_size, &nd->splk, nd->nd.nds.ne->splk);
		add_bn3(g_bn_size, &nd->splk, nd->nd.nds.sw->splk);
		add_bn3(g_bn_size, &nd->splk, nd->nd.nds.se->splk);
	}
	else if (nd->splk == g_free_from_lvs) {
		nd->splk = init_bn(1, g_pops[nd->nd.lvs.nw]+g_pops[nd->nd.lvs.ne]+g_pops[nd->nd.lvs.sw]+g_pops[nd->nd.lvs.se]);
	}
}

int *gen(int l) {
	if (l > 0) {
		int *r = gen(l-1);
		return add_bns(r, r);
	}
	else {
		return init_bn(1, 1);
	}
}

void process_nd(int *max_l, int **g, node_t **nd) {
	int level = g_level, uproot, *r, *s;
	node_t *s_nd;
	if (level < 0 && *g_gen == 0) {
		g_level = *max_l;
	}
	if (*max_l > 2) {
		uproot = (*nd)->nd.nds.nw->nd.nds.se->nd.nds.nw != g_e_nds[*max_l-3] || (*nd)->nd.nds.nw->nd.nds.se->nd.nds.ne != g_e_nds[*max_l-3] || (*nd)->nd.nds.nw->nd.nds.se->nd.nds.sw != g_e_nds[*max_l-3] || (*nd)->nd.nds.ne->nd.nds.sw->nd.nds.nw != g_e_nds[*max_l-3] || (*nd)->nd.nds.ne->nd.nds.sw->nd.nds.ne != g_e_nds[*max_l-3] || (*nd)->nd.nds.ne->nd.nds.sw->nd.nds.se != g_e_nds[*max_l-3] || (*nd)->nd.nds.sw->nd.nds.ne->nd.nds.nw != g_e_nds[*max_l-3] || (*nd)->nd.nds.sw->nd.nds.ne->nd.nds.sw != g_e_nds[*max_l-3] || (*nd)->nd.nds.sw->nd.nds.ne->nd.nds.se != g_e_nds[*max_l-3] || (*nd)->nd.nds.se->nd.nds.nw->nd.nds.ne != g_e_nds[*max_l-3] || (*nd)->nd.nds.se->nd.nds.nw->nd.nds.sw != g_e_nds[*max_l-3] || (*nd)->nd.nds.se->nd.nds.nw->nd.nds.se != g_e_nds[*max_l-3];
	}
	else if (*max_l == 2) {
		uproot = ((*nd)->nd.nds.nw->nd.nds.se->nd.lvs.nw & FULL) || ((*nd)->nd.nds.nw->nd.nds.se->nd.lvs.ne & FULL) || ((*nd)->nd.nds.nw->nd.nds.se->nd.lvs.sw & FULL) || ((*nd)->nd.nds.ne->nd.nds.sw->nd.lvs.nw & FULL) || ((*nd)->nd.nds.ne->nd.nds.sw->nd.lvs.ne & FULL) || ((*nd)->nd.nds.ne->nd.nds.sw->nd.lvs.se & FULL) || ((*nd)->nd.nds.sw->nd.nds.ne->nd.lvs.nw & FULL) || ((*nd)->nd.nds.sw->nd.nds.ne->nd.lvs.sw & FULL) || ((*nd)->nd.nds.sw->nd.nds.ne->nd.lvs.se & FULL) || ((*nd)->nd.nds.se->nd.nds.nw->nd.lvs.ne & FULL) || ((*nd)->nd.nds.se->nd.nds.nw->nd.lvs.sw & FULL) || ((*nd)->nd.nds.se->nd.nds.nw->nd.lvs.se & FULL);
	}
	else {
		uproot = ((*nd)->nd.nds.nw->nd.lvs.se & QT_NW) || ((*nd)->nd.nds.nw->nd.lvs.se & QT_NE) || ((*nd)->nd.nds.nw->nd.lvs.se & QT_SW) || ((*nd)->nd.nds.ne->nd.lvs.sw & QT_NW) || ((*nd)->nd.nds.ne->nd.lvs.sw & QT_NE) || ((*nd)->nd.nds.ne->nd.lvs.sw & QT_SE) || ((*nd)->nd.nds.sw->nd.lvs.ne & QT_NW) || ((*nd)->nd.nds.sw->nd.lvs.ne & QT_SW) || ((*nd)->nd.nds.sw->nd.lvs.ne & QT_SE) || ((*nd)->nd.nds.se->nd.lvs.nw & QT_NE) || ((*nd)->nd.nds.se->nd.lvs.nw & QT_SW) || ((*nd)->nd.nds.se->nd.lvs.nw & QT_SE);
	}
	if (uproot) {
		uproot_nd(*max_l+1, max_l, nd);
		if (level < 0 && *g_gen == 0) {
			g_level = *max_l;
		}
	}
	if (g_collect == 1) {
		g_collect++;
	}
	s_nd = next_nd(*max_l-1, *nd);
	if (g_collect == 2) {
		g_collect--;
	}
	*nd = *max_l > 1 ? hash_from_nds(hash_from_nds(g_e_nds[*max_l-2], g_e_nds[*max_l-2], g_e_nds[*max_l-2], s_nd->nd.nds.nw), hash_from_nds(g_e_nds[*max_l-2], g_e_nds[*max_l-2], s_nd->nd.nds.ne, g_e_nds[*max_l-2]), hash_from_nds(g_e_nds[*max_l-2], s_nd->nd.nds.sw, g_e_nds[*max_l-2], g_e_nds[*max_l-2]), hash_from_nds(s_nd->nd.nds.se, g_e_nds[*max_l-2], g_e_nds[*max_l-2], g_e_nds[*max_l-2])):hash_from_nds(hash_from_lvs(0, 0, 0, s_nd->nd.lvs.nw), hash_from_lvs(0, 0, s_nd->nd.lvs.ne, 0), hash_from_lvs(0, s_nd->nd.lvs.sw, 0, 0), hash_from_lvs(s_nd->nd.lvs.se, 0, 0, 0));
	r = gen(g_level);
	s = copy_bn(**g, *g);
	reset_bns();
	*g = add_bns(r, s);
	print_bn("Generation", *g);
	if (g_population) {
		pop_nd(*nd);
		print_bn("Population", (*nd)->splk);
		unmark_nd(*max_l, *nd);
	}
	if (level < 0 && *g_gen == 0) {
		g_level = -1;
	}
}

void clear_next(int l, node_t *nd) {
	if (nd->splk == g_free_from_nds) {
		clear_next(l-1, nd->nd.nds.nw);
		clear_next(l-1, nd->nd.nds.ne);
		clear_next(l-1, nd->nd.nds.sw);
		clear_next(l-1, nd->nd.nds.se);
		if (l >= g_level && nd->nd.nds.next) {
			nd->nd.nds.next = (node_t *)NULL;
		}
		nd->splk = g_save_from_nds;
	}
	else if (nd->splk == g_free_from_lvs) {
		if (g_level == 0 && nd->nd.lvs.next > UNDEFINED) {
			nd->nd.lvs.next = UNDEFINED;
		}
		nd->splk = g_save_from_lvs;
	}
}

void process_next_nd(int level, int *max_l, int **g, node_t **c_nd) {
	int i;
	for (i = *g_gen; i > 1; i--) {
		if (g_gen[i]%2 > 0) {
			g_gen[i-1] += BN_MAX;
		}
		g_gen[i] >>= 1;
	}
	g_gen[1] >>= 1;
	if (g_gen[1] & 1) {
		if (g_level >= 0) {
			for (i = 0; i <= g_ind_tab_nds; i++) {
				int j;
				for (j = 0; j < g_h_nds_size[i]; j++) {
					node_t *nd1;
					for (nd1 = g_h_nds[i][j]; nd1; nd1 = nd1->link) {
						if (nd1->splk == g_free_from_nds || nd1->splk == g_free_from_lvs) {
							int l = 0;
							node_t *nd2;
							for (nd2 = nd1; nd2->splk == g_free_from_nds || nd2->splk == g_save_from_nds; nd2 = nd2->nd.nds.nw) {
								l++;
							}
							clear_next(l, nd1);
						}
					}
				}
			}
			for (i = 0; i <= g_ind_tab_nds; i++) {
				int j;
				for (j = 0; j < g_h_nds_size[i]; j++) {
					node_t *nd;
					for (nd = g_h_nds[i][j]; nd; nd = nd->link) {
						update_splk(g_save_from_nds, g_save_from_lvs, g_free_from_nds, g_free_from_lvs, nd);
					}
				}
			}
		}
		g_level = level;
		uproot_nd(g_level, max_l, c_nd);
		process_nd(max_l, g, c_nd);
	}
}

void print_cells(const char *msg) {
	int cells = 0, i;
	for (i = 0; i <= g_ind_tab_cells; i++) {
		int j;
		for (j = 0; j < g_h_cells_size[i]; j++) {
			cell_t *cell;
			for (cell = g_h_cells[i][j]; cell; cell = cell->link) {
				cells++;
			}
		}
	}
	fprintf(stderr, "%s %i\n", msg, cells);
	fflush(stderr);
}

cell_t *get_hash_from_xy(int x, int y) {
	int i = g_ind_tab_cells;
	cell_t *cell;
	do {
		for (cell = g_h_cells[i][((uinthash_t)x*(uinthash_t)g_h_cells_side[i]+(uinthash_t)y)%(uinthash_t)g_h_cells_size[i]]; cell && (cell->x != x || cell->y != y); cell = cell->link);
		i--;
	}
	while (i >= 0 && !cell);
	return cell;
}

void update_sn(int x, int y) {
	cell_t *cell = get_hash_from_xy(x, y);
	if (cell) {
		cell->sn++;
	}
	else {
		if (g_c_ind_tab_f_cells < g_ind_tab_f_cells || g_c_ind_f_cells < g_ind_f_cells) {
			if (g_c_ind_f_cells == g_c_max_f_cells) {
				if (g_c_max_f_cells < MAX_CELLS) {
					g_c_max_f_cells <<= 1;
				}
				g_c_ind_tab_f_cells++;
				g_c_ind_f_cells = 0;
			}
			g_f_cells[g_c_ind_tab_f_cells][g_c_ind_f_cells]->x = x;
			g_f_cells[g_c_ind_tab_f_cells][g_c_ind_f_cells]->y = y;
			g_f_cells[g_c_ind_tab_f_cells][g_c_ind_f_cells]->sn = 1;
			new_hash_from_xy(g_f_cells[g_c_ind_tab_f_cells][g_c_ind_f_cells]);
			g_c_ind_f_cells++;
		}
		else {
			new_cell(x, y, 1);
		}
	}
}

void process_cell(int rule, int i, int j, cell_t *cell, int *total_f_cells, cell_t **l_cell) {
	if (rule & (1 << (cell->sn & (ON_CELL-1)))) {
		cell->sn = ON_CELL;
		*l_cell = cell;
	}
	else {
		if (*l_cell) {
			(*l_cell)->link = cell->link;
		}
		else {
			g_h_cells[i][j] = cell->link;
		}
		if (g_ind_tab_f_cells < TAB_SIZE-1 || g_ind_f_nds < g_max_f_nds) {
			if (g_ind_f_cells == g_max_f_cells) {
				if (g_max_f_cells < MAX_CELLS) {
					g_max_f_cells <<= 1;
				}
				g_ind_tab_f_cells++;
				alloc_f_cells();
			}
			g_f_cells[g_ind_tab_f_cells][g_ind_f_cells] = cell;
			g_ind_f_cells++;
		}
		else {
			exit_on_error("No more free cells available");
		}
		(*total_f_cells)++;
	}
}

void process_gen(void) {
	int total_f_cells = 0, i;
	for (i = 0; i <= g_ind_tab_cells; i++) {
		int j;
		for (j = 0; j < g_h_cells_size[i]; j++) {
			cell_t *cell;
			for (cell = g_h_cells[i][j]; cell; cell = cell->link) {
				if (cell->sn & ON_CELL) {
					update_sn(cell->x-1, cell->y-1);
					update_sn(cell->x-1, cell->y);
					update_sn(cell->x-1, cell->y+1);
					update_sn(cell->x, cell->y+1);
					update_sn(cell->x+1, cell->y+1);
					update_sn(cell->x+1, cell->y);
					update_sn(cell->x+1, cell->y-1);
					update_sn(cell->x, cell->y-1);
				}
			}
		}
	}
	if (g_ind_tab_f_cells > 0) {
		g_max_f_cells = ALLOC_CELLS;
		g_ind_tab_f_cells = 0;
		g_c_max_f_cells = ALLOC_CELLS;
		g_c_ind_tab_f_cells = 0;
	}
	g_ind_f_cells = 0;
	g_c_ind_f_cells = 0;
	for (i = 0; i <= g_ind_tab_cells; i++) {
		int j;
		for (j = 0; j < g_h_cells_size[i]; j++) {
			cell_t *cell, *l_cell = (cell_t *)NULL;
			for (cell = g_h_cells[i][j]; cell; cell = cell->link) {
				cell->sn & ON_CELL ? process_cell(g_survival, i, j, cell, &total_f_cells, &l_cell):process_cell(g_birth, i, j, cell, &total_f_cells, &l_cell);
			}
		}
	}
	log_data_operation(total_f_cells, "free cell", "collected");
}

void process_level(int l) {
	if (l > 0) {
		process_level(l-1);
		process_level(l-1);
	}
	else {
		process_gen();
	}
}

void process_gens_loop(int i) {
	if (i > 1) {
		int j;
		for (j = 0; j < BN_MAX; j++) {
			process_gens_loop(i-1);
		}
	}
	else {
		process_gen();
	}
}

void process_gens(int i) {
	int j;
	if (i > 1) {
		for (j = 0; j < *(g_gen+i); j++) {
			process_gens_loop(i);
		}
		process_gens(i-1);
	}
	else {
		for (j = 0; j < *(g_gen+i); j++) {
			process_gen();
		}
	}
}

void set_rc(int r1, int c1, int r4, int c4, int *r2, int *c2, int *r3, int *c3) {
	*r2 = (r1+r4) >> 1;
	*c2 = (c1+c4) >> 1;
	*r3 = *r2+1;
	*c3 = *c2+1;
}

void matrix_from_value(int r1, int c1, int r4, int c4, int v) {
	if (r1 < r4) {
		int r2, c2, r3, c3;
		set_rc(r1, c1, r4, c4, &r2, &c2, &r3, &c3);
		matrix_from_value(r1, c1, r2, c2, v & CELL_NW);
		matrix_from_value(r1, c3, r2, c4, v & CELL_NE);
		matrix_from_value(r3, c1, r4, c2, v & CELL_SW);
		matrix_from_value(r3, c3, r4, c4, v & CELL_SE);
	}
	else {
		g_matrix[r1][c1] = v ? '*':'.';
	}
}

void matrix_from_lf(int r1, int c1, int r4, int c4, int lf) {
	if (r1 < r4) {
		int r2, c2, r3, c3;
		set_rc(r1, c1, r4, c4, &r2, &c2, &r3, &c3);
		matrix_from_value(r1, c1, r2, c2, lf & QT_NW);
		matrix_from_value(r1, c3, r2, c4, (lf & QT_NE) >> 2);
		matrix_from_value(r3, c1, r4, c2, (lf & QT_SW) >> 8);
		matrix_from_value(r3, c3, r4, c4, (lf & QT_SE) >> 10);
	}
	else {
		g_matrix[r1][c1] = lf > EMPTY ? '*':'.';
	}
}

void matrix_from_nd(int l, int r1, int c1, int r4, int c4, node_t *nd) {
	if (r1 < r4) {
		int r2, c2, r3, c3;
		set_rc(r1, c1, r4, c4, &r2, &c2, &r3, &c3);
		if (l > 0) {
			matrix_from_nd(l-1, r1, c1, r2, c2, nd->nd.nds.nw);
			matrix_from_nd(l-1, r1, c3, r2, c4, nd->nd.nds.ne);
			matrix_from_nd(l-1, r3, c1, r4, c2, nd->nd.nds.sw);
			matrix_from_nd(l-1, r3, c3, r4, c4, nd->nd.nds.se);
		}
		else {
			matrix_from_lf(r1, c1, r2, c2, nd->nd.lvs.nw);
			matrix_from_lf(r1, c3, r2, c4, nd->nd.lvs.ne);
			matrix_from_lf(r3, c1, r4, c2, nd->nd.lvs.sw);
			matrix_from_lf(r3, c3, r4, c4, nd->nd.lvs.se);
		}
	}
	else {
		g_matrix[r1][c1] = nd != g_e_nds[l] ? '*':'.';
	}
}

void update_path(char *cmd, node_t *nd, int *l, node_t **m_nd, char **m_path) {
	if (*l > 1) {
		*m_nd = nd;
		strcpy(*m_path, cmd);
		(*l)--;
	}
	else {
		fprintf(stderr, "Already at lowest level\n");
		fflush(stderr);
	}
}

void write_nd(int l, node_t *nd) {
	if (nd->splk == g_free_from_nds) {
		write_nd(l-1, nd->nd.nds.nw);
		write_nd(l-1, nd->nd.nds.ne);
		write_nd(l-1, nd->nd.nds.sw);
		write_nd(l-1, nd->nd.nds.se);
		printf("%i %i %i %i %i\n", l, *(nd->nd.nds.nw->splk), *(nd->nd.nds.ne->splk), *(nd->nd.nds.sw->splk), *(nd->nd.nds.se->splk));
		new_bn(0, g_nd_key, &nd->splk);
		g_nd_key++;
	}
	else if (nd->splk == g_free_from_lvs) {
		printf("%i %i %i %i %i\n", l, nd->nd.lvs.nw, nd->nd.lvs.ne, nd->nd.lvs.sw, nd->nd.lvs.se);
		new_bn(0, g_nd_key, &nd->splk);
		g_nd_key++;
	}
}

void output_nd(int output, int argc, char *argv[], int max_l, node_t *nd) {
	if (output == OUTPUT_VIEW) {
		char **m_path, cmd[CMD_LEN+1];
		int l, i;
		node_t **m_nds = (node_t **)malloc(sizeof(node_t *)*(size_t)max_l);
		if (!m_nds) {
			exit_on_error("Could not allocate memory for m_nds");
		}
		m_nds[0] = nd;
		m_path = (char **)malloc(sizeof(char *)*(size_t)max_l);
		if (!m_path) {
			free(m_nds);
			exit_on_error("Could not allocate memory for m_path");
		}
		for (i = 0; i < max_l; i++) {
			m_path[i] = (char *)malloc(sizeof(char)*3);
			if (!m_path[i]) {
				int j;
				for (j = 0; j < i; j++) {
					free(m_path[j]);
				}
				free(m_path);
				free(m_nds);
				exit_on_error("Could not allocate memory for m_path[%i]", i);
			}
		}
		strcpy(m_path[0], "r");
		l = max_l;
		do {
			int rp, j1, j2;
			matrix_from_nd(l, 0, 0, MATRIX_SIZE-1, MATRIX_SIZE-1, m_nds[max_l-l]);
			printf("Level %i Path %s", l, m_path[0]);
			for (j1 = 1; j1 <= max_l-l; j1++) {
				printf("/%s", m_path[j1]);
			}
			puts("");
			for (j1 = 0; j1 < MATRIX_SIZE; j1++) {
				int k;
				for (k = 0; k < MATRIX_SIZE; k++) {
					putchar(g_matrix[j1][k]);
				}
				puts("");
			}
			j1 = 0;
			do {
				int c;
				printf("? ");
				fflush(stdout);
				do {
					c = getchar();
					if (j1 < CMD_LEN && c != '\n') {
						cmd[j1] = (char)c;
						j1++;
					}
				}
				while (c != '\n');
			}
			while (j1 == 0);
			cmd[j1] = '\0';
			rp = 0;
			for (j1 = 0; cmd[j1] >= '0' && cmd[j1] <= '9'; j1++) {
				rp = rp > 0 ? 10*rp+(cmd[j1]-'0'):cmd[j1]-'0';
			}
			if (j1 == 0) {
				rp = 1;
			}
			for (j2 = 0; j2 < rp; j2++) {
				if (!strcmp(cmd+j1, "nw")) {
					update_path(cmd+j1, m_nds[max_l-l]->nd.nds.nw, &l, &m_nds[max_l-l+1], &m_path[max_l-l+1]);
				}
				else if (!strcmp(cmd+j1, "ne")) {
					update_path(cmd+j1, m_nds[max_l-l]->nd.nds.ne, &l, &m_nds[max_l-l+1], &m_path[max_l-l+1]);
				}
				else if (!strcmp(cmd+j1, "sw")) {
					update_path(cmd+j1, m_nds[max_l-l]->nd.nds.sw, &l, &m_nds[max_l-l+1], &m_path[max_l-l+1]);
				}
				else if (!strcmp(cmd+j1, "se")) {
					update_path(cmd+j1, m_nds[max_l-l]->nd.nds.se, &l, &m_nds[max_l-l+1], &m_path[max_l-l+1]);
				}
				else if (!strcmp(cmd+j1, "u")) {
					if (l < max_l) {
						l++;
					}
					else {
						fprintf(stderr, "Already at highest level\n");
						fflush(stderr);
					}
				}
				else if (strcmp(cmd, "q")) {
					fprintf(stderr, "Unknown command\n");
					fflush(stderr);
				}
			}
		}
		while (strcmp(cmd, "q"));
		for (i = 0; i < max_l; i++) {
			free(m_path[i]);
		}
		free(m_path);
		free(m_nds);
	}
	else if (output == OUTPUT_FILE) {
		int i;
		g_nd_key = 1;
		printf("# migalife 1.09\n");
		printf("# Options");
		for (i = 1; i < argc; i++) {
			printf(" %s", argv[i]);
		}
		puts("");
		printf("R %s/%s\n", g_sdigits, g_bdigits);
		write_nd(max_l, nd);
		fflush(stdout);
		fprintf(stderr, "Finished writing output\n");
		fflush(stderr);
	}
}

int main(int argc, char *argv[]) {
	char *filename;
	int input, output, ns, nb, slash, max_l, *g, i;
	node_t *r_nd, *c_nd;
	fprintf(stderr, "migalife 1.09\n");
	fprintf(stderr, "Copyright 2007-2019 miga Software\n");
	fflush(stderr);
	g_hashlife = 0;
	g_level = -1;
	g_gen = (int *)malloc(sizeof(int));
	if (!g_gen) {
		fprintf(stderr, "Could not allocate memory for g_gen\n");
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
	*g_gen = 0;
	g_max_nds = 0;
	g_collect = 0;
	g_population = 0;
	g_rules = (char *)NULL;
	input = INPUT_RLE;
	filename = (char *)NULL;
	output = OUTPUT_NONE;
	g_verbose = 0;
	i = 1;
	while (i < argc) {
		if (!strcmp(argv[i], "-h")) {
			g_hashlife = 1;
			i++;
		}
		else if (!strcmp(argv[i], "-l") && i < argc-1) {
			g_level = atoi(argv[i+1]);
			*g_gen = 0;
			i += 2;
		}
		else if (!strcmp(argv[i], "-g") && i < argc-1) {
			int j1, j2;
			g_level = -1;
			for (j1 = argv[i+1][0] != '-' ? 0:1; argv[i+1][j1] == '0'; j1++);
			for (j2 = 0; argv[i+1][j1+j2] >= '0' && argv[i+1][j1+j2] <= '9'; j2++);
			if (j2 > 0) {
				if (argv[i+1][0] != '-') {
					char *argg;
					int *int_ptr = (int *)realloc(g_gen, sizeof(int)*((size_t)(j2-1)/BN_DIGITS+2)), k1, k2;
					if (!int_ptr) {
						fprintf(stderr, "Could not reallocate memory for g_gen\n");
						fflush(stderr);
						free(g_gen);
						exit(EXIT_FAILURE);
					}
					g_gen = int_ptr;
					*g_gen = (j2-1)/BN_DIGITS+1;
					argg = (char *)malloc(sizeof(char)*(BN_DIGITS+1)*(size_t)(*g_gen));
					if (!argg) {
						fprintf(stderr, "Could not allocate memory for argg\n");
						fflush(stderr);
						free(g_gen);
						exit(EXIT_FAILURE);
					}
					k1 = (j2-1)%BN_DIGITS+1;
					strncpy(argg, argv[i+1]+j1, (size_t)k1);
					argg[k1] = '\0';
					for (k2 = 1; k2 < *g_gen; k2++) {
						strncpy(argg+(BN_DIGITS+1)*k2, argv[i+1]+j1+k1+BN_DIGITS*(k2-1), BN_DIGITS);
						argg[(BN_DIGITS+1)*(k2+1)-1] = '\0';
					}
					for (k2 = 0; k2 < *g_gen; k2++) {
						g_gen[*g_gen-k2] = atoi(argg+(BN_DIGITS+1)*k2);
					}
					free(argg);
				}
			}
			else {
				int *int_ptr = (int *)realloc(g_gen, sizeof(int)*2);
				if (!int_ptr) {
					fprintf(stderr, "Could not reallocate memory for g_gen\n");
					fflush(stderr);
					free(g_gen);
					exit(EXIT_FAILURE);
				}
				g_gen = int_ptr;
				*g_gen = 1;
				g_gen[1] = 0;
			}
			i += 2;
		}
		else if (!strcmp(argv[i], "-n") && i < argc-1) {
			g_max_nds = atoi(argv[i+1]);
			i += 2;
		}
		else if (!strcmp(argv[i], "-c")) {
			g_collect = 1;
			i++;
		}
		else if (!strcmp(argv[i], "-p")) {
			g_population = 1;
			i++;
		}
		else if (!strcmp(argv[i], "-r") && i < argc-1) {
			g_rules = argv[i+1];
			i += 2;
		}
		else if (!strcmp(argv[i], "-i") && i < argc-1) {
			input = atoi(argv[i+1]);
			i += 2;
		}
		else if (!strcmp(argv[i], "-f") && i < argc-1) {
			filename = argv[i+1];
			i += 2;
		}
		else if (!strcmp(argv[i], "-o") && i < argc-1) {
			output = atoi(argv[i+1]);
			i += 2;
		}
		else if (!strcmp(argv[i], "-v")) {
			g_verbose = 1;
			i++;
		}
		else {
			usage(argv[0]);
			fprintf(stderr, "Unknown option %s or missing option value\n", argv[i]);
			fflush(stderr);
			free(g_gen);
			return EXIT_FAILURE;
		}
	}
	if (g_max_nds < 0 || g_max_nds > MAX_NDS) {
		usage(argv[0]);
		fprintf(stderr, "nodes must be within 0 and %i\nnodes > 0 static allocation for nodes\nnodes = 0 dynamic allocation for nodes\n", MAX_NDS);
		fflush(stderr);
		free(g_gen);
		return EXIT_FAILURE;
	}
	if (input != INPUT_RLE && input != INPUT_LIF && input != INPUT_HLF) {
		usage(argv[0]);
		fprintf(stderr, "input must be %i for rle/%i for lif/%i for hlf\n", INPUT_RLE, INPUT_LIF, INPUT_HLF);
		fflush(stderr);
		free(g_gen);
		return EXIT_FAILURE;
	}
	if (filename) {
		g_file = fopen(filename, "r");
		if (!g_file) {
			fprintf(stderr, "Cannot open input file %s\n", filename);
			fflush(stderr);
			free(g_gen);
			return EXIT_FAILURE;
		}
	}
	else {
		g_file = stdin;
	}
	if (output != OUTPUT_NONE && output != OUTPUT_VIEW && output != OUTPUT_FILE) {
		usage(argv[0]);
		fprintf(stderr, "output must be %i for none/%i for view/%i for file\n", OUTPUT_NONE, OUTPUT_VIEW, OUTPUT_FILE);
		fflush(stderr);
		if (g_file != stdin) {
			fclose(g_file);
		}
		free(g_gen);
		return EXIT_FAILURE;
	}
	g_bns = (int **)NULL;
	if (g_collect) {
		g_r_nds = (node_t ***)NULL;
		g_c_max_r_nds = (int *)NULL;
		g_c_ind_tab_r_nds = (int *)NULL;
		g_c_ind_r_nds = (int *)NULL;
		g_f_nds = (node_t ***)NULL;
	}
	g_h_nds_size = (int *)NULL;
	g_h_nds = (node_t ***)NULL;
	g_nds = (node_t **)NULL;
	g_e_nds = (node_t **)NULL;
	g_nds_n = (node_t **)NULL;
	g_nds_w = (node_t **)NULL;
	g_nds_c = (node_t **)NULL;
	g_nds_e = (node_t **)NULL;
	g_nds_s = (node_t **)NULL;
	if (!g_hashlife) {
		g_f_cells = (cell_t ***)NULL;
	}
	g_h_cells_size = (int *)NULL;
	g_h_cells = (cell_t ***)NULL;
	g_h_cells_side = (int *)NULL;
	g_cells = (cell_t **)NULL;
	g_o_nds = (node_t **)NULL;
	g_max_bns = ALLOC_BNS;
	g_ind_tab_bns = 0;
	g_bns = (int **)malloc(sizeof(int *)*TAB_SIZE);
	if (!g_bns) {
		exit_on_error("Could not allocate memory for g_bns");
	}
	for (i = 0; i < TAB_SIZE; i++) {
		g_bns[i] = (int *)NULL;
	}
	alloc_bns();
	if (g_collect) {
		g_max_r_nds = ALLOC_NDS;
		g_ind_tab_r_nds = 0;
		g_r_nds = (node_t ***)malloc(sizeof(node_t **)*TAB_SIZE);
		if (!g_r_nds) {
			exit_on_error("Could not allocate memory for g_r_nds");
		}
		for (i = 0; i < TAB_SIZE; i++) {
			g_r_nds[i] = (node_t **)NULL;
		}
		alloc_r_nds();
		g_max_f_nds = ALLOC_NDS;
		g_ind_tab_f_nds = 0;
		g_f_nds = (node_t ***)malloc(sizeof(node_t **)*TAB_SIZE);
		if (!g_f_nds) {
			exit_on_error("Could not allocate memory for g_f_nds");
		}
		for (i = 0; i < TAB_SIZE; i++) {
			g_f_nds[i] = (node_t **)NULL;
		}
		alloc_f_nds();
		g_c_max_f_nds = ALLOC_NDS;
		g_c_ind_tab_f_nds = 0;
		g_c_ind_f_nds = 0;
	}
	if (g_max_nds > 0) {
		g_tab_nds = 1;
	}
	else {
		g_tab_nds = TAB_SIZE;
		g_max_nds = ALLOC_NDS;
	}
	g_ind_tab_nds = 0;
	g_h_nds_size = (int *)malloc(sizeof(int)*(size_t)g_tab_nds);
	if (!g_h_nds_size) {
		exit_on_error("Could not allocate memory for g_h_nds_size");
	}
	g_h_nds = (node_t ***)malloc(sizeof(node_t **)*(size_t)g_tab_nds);
	if (!g_h_nds) {
		exit_on_error("Could not allocate memory for g_h_nds");
	}
	g_nds = (node_t **)malloc(sizeof(node_t *)*(size_t)g_tab_nds);
	if (!g_nds) {
		exit_on_error("Could not allocate memory for g_nds");
	}
	alloc_nds();
	new_bn(0, 1, &g_free_from_nds);
	new_bn(0, 0, &g_free_from_lvs);
	new_bn(0, 1, &g_save_from_nds);
	new_bn(0, 0, &g_save_from_lvs);
	r_nd = (node_t *)NULL;
	c_nd = (node_t *)NULL;
	ns = 1;
	nb = 1;
	slash = 0;
	if (g_hashlife) {
		for (i = 0; i < LVS_SIZE; i++) {
			g_pops[i] = bits_set(i);
			g_next_qts[i] = UNDEFINED;
		}
		if (input == INPUT_RLE || input == INPUT_LIF) {
			init_cells();
			alloc_cells();
			input == INPUT_RLE ? read_rle(&ns, &nb, &slash):read_lif(&ns, &nb, &slash);
			nd_from_cells(&max_l, &r_nd);
		}
		else {
			read_hlf(&ns, &nb, &slash, &max_l, &c_nd);
			unmark_nd(max_l, c_nd);
		}
		set_sb_rules(ns, nb, slash);
		alloc_misc_nds(max_l);
		g_nds_n = (node_t **)malloc(sizeof(node_t *)*(size_t)max_l);
		if (!g_nds_n) {
			exit_on_error("Could not allocate memory for g_nds_n");
		}
		g_nds_w = (node_t **)malloc(sizeof(node_t *)*(size_t)max_l);
		if (!g_nds_w) {
			exit_on_error("Could not allocate memory for g_nds_w");
		}
		g_nds_c = (node_t **)malloc(sizeof(node_t *)*(size_t)max_l);
		if (!g_nds_c) {
			exit_on_error("Could not allocate memory for g_nds_c");
		}
		g_nds_e = (node_t **)malloc(sizeof(node_t *)*(size_t)max_l);
		if (!g_nds_e) {
			exit_on_error("Could not allocate memory for g_nds_e");
		}
		g_nds_s = (node_t **)malloc(sizeof(node_t *)*(size_t)max_l);
		if (!g_nds_s) {
			exit_on_error("Could not allocate memory for g_nds_s");
		}
		if (r_nd) {
			c_nd = canonalize_nd(max_l, r_nd);
			free_o_nds();
		}
		reset_bns();
		g = init_bn(1, 0);
		pop_nd(c_nd);
		print_bn("Start population", c_nd->splk);
		unmark_nd(max_l, c_nd);
		if (g_level >= 0) {
			uproot_nd(g_level, &max_l, &c_nd);
			process_nd(&max_l, &g, &c_nd);
		}
		else {
			if (*g_gen > 0) {
				int level = 0;
				if (g_gen[1] & 1) {
					g_level = level;
					uproot_nd(g_level, &max_l, &c_nd);
					process_nd(&max_l, &g, &c_nd);
				}
				do {
					level++;
					process_next_nd(level, &max_l, &g, &c_nd);
					if (*g_gen > 1 && g_gen[*g_gen] == 0) {
						(*g_gen)--;
					}
				}
				while (*g_gen > 1 || g_gen[1] > 0);
			}
			else {
				while (g_level < 0) {
					process_nd(&max_l, &g, &c_nd);
				}
			}
		}
		output_nd(output, argc, argv, max_l, c_nd);
	}
	else {
		g_max_f_cells = ALLOC_CELLS;
		g_ind_tab_f_cells = 0;
		g_f_cells = (cell_t ***)malloc(sizeof(cell_t **)*TAB_SIZE);
		if (!g_f_cells) {
			exit_on_error("Could not allocate memory for g_f_cells");
		}
		for (i = 0; i < TAB_SIZE; i++) {
			g_f_cells[i] = (cell_t **)NULL;
		}
		alloc_f_cells();
		g_c_max_f_cells = ALLOC_CELLS;
		g_c_ind_tab_f_cells = 0;
		g_c_ind_f_cells = 0;
		init_cells();
		alloc_cells();
		if (input == INPUT_RLE) {
			read_rle(&ns, &nb, &slash);
		}
		else if (input == INPUT_LIF) {
			read_lif(&ns, &nb, &slash);
		}
		else {
			exit_on_error("hlf input not enabled in conventional mode");
		}
		set_sb_rules(ns, nb, slash);
		print_cells("Start population");
		if (g_level >= 0) {
			process_level(g_level);
			g = gen(g_level);
			print_bn("Generation", g);
			print_cells("Population");
		}
		else {
			if (*g_gen > 0) {
				if (*g_gen > 1 || g_gen[1] > 0) {
					process_gens(*g_gen);
					g = copy_bn(*g_gen, g_gen);
					print_bn("Generation", g);
					print_cells("Population");
				}
			}
			else {
				while (g_level < 0) {
					process_gen();
				}
			}
		}
		if (output == OUTPUT_VIEW || output == OUTPUT_FILE) {
			nd_from_cells(&max_l, &r_nd);
			alloc_misc_nds(max_l);
			c_nd = canonalize_nd(max_l, r_nd);
			free_o_nds();
			output_nd(output, argc, argv, max_l, c_nd);
		}
	}
	free_globals();
	return EXIT_SUCCESS;
}
