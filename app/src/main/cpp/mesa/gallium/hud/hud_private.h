#ifndef HUD_PRIVATE_H
#define HUD_PRIVATE_H

#include "../../include/pipe/p_context.h"
#include "../../include/pipe/p_state.h"
#include "../../util/list.h"
#include "font.h"
#include "../cso_cache/cso_context.h"

enum hud_counter {
    HUD_COUNTER_OFFLOADED,
    HUD_COUNTER_DIRECT,
    HUD_COUNTER_SYNCS,
};

struct hud_context {
    int refcount;
    bool simple;

    /* Context where queries are executed. */
    struct pipe_context *record_pipe;

    /* Context where the HUD is drawn: */
    struct pipe_context *pipe;
    struct cso_context *cso;
    struct st_context_iface *st;

    struct hud_batch_query_context *batch_query;
    struct list_head pane_list;

    struct util_queue_monitoring *monitored_queue;

    /* states */
    struct pipe_blend_state no_blend, alpha_blend;
    struct pipe_depth_stencil_alpha_state dsa;
    void *fs_color, *fs_text;
    struct pipe_rasterizer_state rasterizer, rasterizer_aa_lines;
    void *vs;
    struct cso_velems_state velems;

    /* font */
    struct util_font font;
    struct pipe_sampler_view *font_sampler_view;
    struct pipe_sampler_state font_sampler_state;

    /* VS constant buffer */
    struct {
        float color[4];
        float two_div_fb_width;
        float two_div_fb_height;
        float translate[2];
        float scale[2];
        float padding[2];
    } constants;
    struct pipe_constant_buffer constbuf;

    unsigned fb_width, fb_height;

    /* vertices for text and background drawing are accumulated here and then
     * drawn all at once */
    struct vertex_queue {
        float *vertices;
        struct pipe_vertex_buffer vbuf;
        unsigned max_num_vertices;
        unsigned num_vertices;
        unsigned buffer_size;
    } text, bg, whitelines;

    bool has_srgb;
};

struct hud_graph {
    /* initialized by common code */
    struct list_head head;
    struct hud_pane *pane;
    float color[3];
    float *vertices; /* ring buffer of vertices */

    /* name and query */
    char name[128];
    void *query_data;
    void (*begin_query)(struct hud_graph *gr, struct pipe_context *pipe);
    void (*query_new_value)(struct hud_graph *gr, struct pipe_context *pipe);
    /* use this instead of ordinary free() */
    void (*free_query_data)(void *ptr, struct pipe_context *pipe);

    /* mutable variables */
    unsigned num_vertices;
    unsigned index; /* vertex index being updated */
    double current_value;
    FILE *fd;
};

struct hud_pane {
    struct list_head head;
    struct hud_context *hud;
    unsigned x1, y1, x2, y2, y_simple;
    unsigned inner_x1;
    unsigned inner_y1;
    unsigned inner_x2;
    unsigned inner_y2;
    unsigned inner_width;
    unsigned inner_height;
    float yscale;
    unsigned max_num_vertices;
    unsigned last_line; /* index of the last describing line in the graph */
    uint64_t max_value;
    uint64_t initial_max_value;
    uint64_t ceiling;
    unsigned dyn_ceil_last_ran;
    boolean dyn_ceiling;
    boolean sort_items;
    enum pipe_driver_query_type type;
    uint64_t period; /* in microseconds */

    struct list_head graph_list;
    unsigned num_graphs;
    unsigned next_color;
};


/* core */
void hud_pane_add_graph(struct hud_pane *pane, struct hud_graph *gr);
void hud_pane_set_max_value(struct hud_pane *pane, uint64_t value);
void hud_graph_add_value(struct hud_graph *gr, double value);

/* graphs/queries */
struct hud_batch_query_context;

#define ALL_CPUS ~0 /* optionally set as cpu_index */

int hud_get_num_cpus(void);

void hud_fps_graph_install(struct hud_pane *pane);
void hud_frametime_graph_install(struct hud_pane *pane);
void hud_cpu_graph_install(struct hud_pane *pane, unsigned cpu_index);
void hud_thread_busy_install(struct hud_pane *pane, const char *name, bool main);
void hud_thread_counter_install(struct hud_pane *pane, const char *name,
                                enum hud_counter counter);
void hud_pipe_query_install(struct hud_batch_query_context **pbq,
                            struct hud_pane *pane,
                            const char *name,
                            enum pipe_query_type query_type,
                            unsigned result_index,
                            uint64_t max_value,
                            enum pipe_driver_query_type type,
                            enum pipe_driver_query_result_type result_type,
                            unsigned flags);
boolean hud_driver_query_install(struct hud_batch_query_context **pbq,
                                 struct hud_pane *pane,
                                 struct pipe_screen *screen, const char *name);
void hud_batch_query_begin(struct hud_batch_query_context *bq,
                           struct pipe_context *pipe);
void hud_batch_query_update(struct hud_batch_query_context *bq,
                            struct pipe_context *pipe);
void hud_batch_query_cleanup(struct hud_batch_query_context **pbq,
                             struct pipe_context *pipe);

#ifdef HAVE_GALLIUM_EXTRA_HUD
int hud_get_num_nics(bool displayhelp);
#define NIC_DIRECTION_RX 1
#define NIC_DIRECTION_TX 2
#define NIC_RSSI_DBM     3
void hud_nic_graph_install(struct hud_pane *pane, const char *nic_index,
                           unsigned int mode);

int hud_get_num_disks(bool displayhelp);
#define DISKSTAT_RD 1
#define DISKSTAT_WR 2
void hud_diskstat_graph_install(struct hud_pane *pane, const char *dev_name,
                                unsigned int mode);

int hud_get_num_cpufreq(bool displayhelp);
#define CPUFREQ_MINIMUM     1
#define CPUFREQ_CURRENT     2
#define CPUFREQ_MAXIMUM     3
void hud_cpufreq_graph_install(struct hud_pane *pane, int cpu_index, unsigned int mode);
#endif

#ifdef HAVE_LIBSENSORS
int hud_get_num_sensors(bool displayhelp);
#define SENSORS_TEMP_CURRENT     1
#define SENSORS_TEMP_CRITICAL    2
#define SENSORS_VOLTAGE_CURRENT  3
#define SENSORS_CURRENT_CURRENT  4
#define SENSORS_POWER_CURRENT    5
void hud_sensors_temp_graph_install(struct hud_pane *pane, const char *dev_name,
                                    unsigned int mode);
#endif

#endif
