#ifndef HUD_CONTEXT_H
#define HUD_CONTEXT_H

struct hud_context;
struct cso_context;
struct pipe_context;
struct pipe_resource;
struct util_queue_monitoring;
struct st_context_iface;

struct hud_context *
hud_create(struct cso_context *cso, struct st_context_iface *st,
           struct hud_context *share);

void
hud_destroy(struct hud_context *hud, struct cso_context *cso);

void
hud_run(struct hud_context *hud, struct cso_context *cso,
        struct pipe_resource *tex);

void
hud_record_only(struct hud_context *hud, struct pipe_context *pipe);

void
hud_add_queue_for_monitoring(struct hud_context *hud,
                             struct util_queue_monitoring *queue_info);

#endif
