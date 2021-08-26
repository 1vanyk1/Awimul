#ifndef GLSL_IR_RVALUE_VISITOR_H
#define GLSL_IR_RVALUE_VISITOR_H

class ir_rvalue_base_visitor : public ir_hierarchical_visitor {
public:
    ir_visitor_status rvalue_visit(ir_assignment *);
    ir_visitor_status rvalue_visit(ir_call *);
    ir_visitor_status rvalue_visit(ir_dereference_array *);
    ir_visitor_status rvalue_visit(ir_dereference_record *);
    ir_visitor_status rvalue_visit(ir_discard *);
    ir_visitor_status rvalue_visit(ir_expression *);
    ir_visitor_status rvalue_visit(ir_if *);
    ir_visitor_status rvalue_visit(ir_return *);
    ir_visitor_status rvalue_visit(ir_swizzle *);
    ir_visitor_status rvalue_visit(ir_texture *);
    ir_visitor_status rvalue_visit(ir_emit_vertex *);
    ir_visitor_status rvalue_visit(ir_end_primitive *);

    virtual void handle_rvalue(ir_rvalue **rvalue) = 0;
};

class ir_rvalue_visitor : public ir_rvalue_base_visitor {
public:

    virtual ir_visitor_status visit_leave(ir_assignment *);
    virtual ir_visitor_status visit_leave(ir_call *);
    virtual ir_visitor_status visit_leave(ir_dereference_array *);
    virtual ir_visitor_status visit_leave(ir_dereference_record *);
    virtual ir_visitor_status visit_leave(ir_discard *);
    virtual ir_visitor_status visit_leave(ir_expression *);
    virtual ir_visitor_status visit_leave(ir_if *);
    virtual ir_visitor_status visit_leave(ir_return *);
    virtual ir_visitor_status visit_leave(ir_swizzle *);
    virtual ir_visitor_status visit_leave(ir_texture *);
    virtual ir_visitor_status visit_leave(ir_emit_vertex *);
    virtual ir_visitor_status visit_leave(ir_end_primitive *);
};

class ir_rvalue_enter_visitor : public ir_rvalue_base_visitor {
public:

    virtual ir_visitor_status visit_enter(ir_assignment *);
    virtual ir_visitor_status visit_enter(ir_call *);
    virtual ir_visitor_status visit_enter(ir_dereference_array *);
    virtual ir_visitor_status visit_enter(ir_dereference_record *);
    virtual ir_visitor_status visit_enter(ir_discard *);
    virtual ir_visitor_status visit_enter(ir_expression *);
    virtual ir_visitor_status visit_enter(ir_if *);
    virtual ir_visitor_status visit_enter(ir_return *);
    virtual ir_visitor_status visit_enter(ir_swizzle *);
    virtual ir_visitor_status visit_enter(ir_texture *);
    virtual ir_visitor_status visit_enter(ir_emit_vertex *);
    virtual ir_visitor_status visit_enter(ir_end_primitive *);
};

#endif /* GLSL_IR_RVALUE_VISITOR_H */
