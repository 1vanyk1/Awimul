#ifdef __cplusplus

// some enum helpers
#define MESA_DEFINE_CPP_ENUM_BINARY_OPERATOR(Enum, op) \
extern "C++" {                                         \
static inline                                          \
Enum operator op (Enum a, Enum b)                      \
{                                                      \
   uint64_t ua = a, ub = b;                            \
   return static_cast<Enum>(ua op ub);                 \
}                                                      \
                                                       \
static inline                                          \
Enum& operator op##= (Enum &a, Enum b)                 \
{                                                      \
   uint64_t ua = a, ub = b;                            \
   ua op##= ub;                                        \
   a = static_cast<Enum>(ua);                          \
   return a;                                           \
}                                                      \
}

#define MESA_DEFINE_CPP_ENUM_UNARY_OPERATOR(Enum, op) \
extern "C++" {                                        \
static inline                                         \
Enum operator op (Enum a)                             \
{                                                     \
   uint64_t ua = a;                                   \
   return static_cast<Enum>(op ua);                   \
}                                                     \
}

#define MESA_DEFINE_CPP_ENUM_BITFIELD_OPERATORS(Enum) \
MESA_DEFINE_CPP_ENUM_BINARY_OPERATOR(Enum, |)         \
MESA_DEFINE_CPP_ENUM_BINARY_OPERATOR(Enum, &)         \
MESA_DEFINE_CPP_ENUM_BINARY_OPERATOR(Enum, ^)         \
MESA_DEFINE_CPP_ENUM_UNARY_OPERATOR(Enum, ~)

#else

#define MESA_DEFINE_CPP_ENUM_BITFIELD_OPERATORS(Enum)

#endif
