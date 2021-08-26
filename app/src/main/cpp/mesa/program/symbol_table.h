#ifndef MESA_SYMBOL_TABLE_H
#define MESA_SYMBOL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

struct _mesa_symbol_table;

extern void _mesa_symbol_table_push_scope(struct _mesa_symbol_table *table);

extern void _mesa_symbol_table_pop_scope(struct _mesa_symbol_table *table);

extern int _mesa_symbol_table_add_symbol(struct _mesa_symbol_table *symtab,
                                         const char *name, void *declaration);

extern int _mesa_symbol_table_replace_symbol(struct _mesa_symbol_table *table,
                                             const char *name,
                                             void *declaration);

extern int
_mesa_symbol_table_add_global_symbol(struct _mesa_symbol_table *symtab,
                                     const char *name,
                                     void *declaration);

extern int _mesa_symbol_table_symbol_scope(struct _mesa_symbol_table *table,
                                           const char *name);

extern void *_mesa_symbol_table_find_symbol(struct _mesa_symbol_table *symtab,
                                            const char *name);

extern struct _mesa_symbol_table *_mesa_symbol_table_ctor(void);

extern void _mesa_symbol_table_dtor(struct _mesa_symbol_table *);

#ifdef __cplusplus
}
#endif

#endif /* MESA_SYMBOL_TABLE_H */
