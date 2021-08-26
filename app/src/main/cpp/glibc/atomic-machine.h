#ifndef _NIOS2_ATOMIC_MACHINE_H
#define _NIOS2_ATOMIC_MACHINE_H 1

#include <stdint.h>

#ifndef LOCK_PREFIX
# ifdef UP
#  define LOCK_PREFIX        /* nothing */
# else
#  define LOCK_PREFIX "lock;"
# endif
#endif

#ifdef __x86_64__
# define __HAVE_64B_ATOMICS                1
# define SP_REG                                "rsp"
# define SEG_REG                        "fs"
# define BR_CONSTRAINT                        "q"
# define IBR_CONSTRAINT                        "iq"
#else
# define __HAVE_64B_ATOMICS                0
# define SP_REG                                "esp"
# define SEG_REG                        "gs"
# define BR_CONSTRAINT                        "r"
# define IBR_CONSTRAINT                        "ir"
#endif
#define ATOMIC_EXCHANGE_USES_CAS        0

typedef int32_t atomic32_t;
typedef uint32_t uatomic32_t;
typedef int_fast32_t atomic_fast32_t;
typedef uint_fast32_t uatomic_fast32_t;

typedef intptr_t atomicptr_t;
typedef uintptr_t uatomicptr_t;
typedef intmax_t atomic_max_t;
typedef uintmax_t uatomic_max_t;

#define __HAVE_64B_ATOMICS 0
#define USE_ATOMIC_COMPILER_BUILTINS 0

void __arm_link_error (void);

/* XXX Is this actually correct?  */
#define ATOMIC_EXCHANGE_USES_CAS 1

#define __arch_compare_and_exchange_val_8_acq(mem, newval, oldval)	\
  (abort (), (__typeof (*mem)) 0)
#define __arch_compare_and_exchange_val_16_acq(mem, newval, oldval)	\
  (abort (), (__typeof (*mem)) 0)
#define __arch_compare_and_exchange_val_64_acq(mem, newval, oldval)	\
  (abort (), (__typeof (*mem)) 0)

#define __arch_compare_and_exchange_bool_8_acq(mem, newval, oldval)	\
  (abort (), 0)
#define __arch_compare_and_exchange_bool_16_acq(mem, newval, oldval)	\
  (abort (), 0)
#define __arch_compare_and_exchange_bool_64_acq(mem, newval, oldval)	\
  (abort (), 0)


# define atomic_exchange_acq(mem, value)                                \
  __atomic_val_bysize (__arch_exchange, int, mem, value, __ATOMIC_ACQUIRE)
# define atomic_exchange_rel(mem, value)                                \
  __atomic_val_bysize (__arch_exchange, int, mem, value, __ATOMIC_RELEASE)
/* Atomic exchange (without compare).  */
# define __arch_exchange_8_int(mem, newval, model)      \
  (__arm_link_error (), (typeof (*mem)) 0)
# define __arch_exchange_16_int(mem, newval, model)     \
  (__arm_link_error (), (typeof (*mem)) 0)
# define __arch_exchange_32_int(mem, newval, model)     \
  __atomic_exchange_n (mem, newval, model)
# define __arch_exchange_64_int(mem, newval, model)     \
  (__arm_link_error (), (typeof (*mem)) 0)
/* Compare and exchange with "acquire" semantics, ie barrier after.  */
# define atomic_compare_and_exchange_bool_acq(mem, new, old)    \
  __atomic_bool_bysize (__arch_compare_and_exchange_bool, int,  \
                        mem, new, old, __ATOMIC_ACQUIRE)
# define atomic_compare_and_exchange_val_acq(mem, _new, old)     \
  __atomic_val_bysize (__arch_compare_and_exchange_val, int,    \
                       mem, _new, old, __ATOMIC_ACQUIRE)
/* Compare and exchange with "release" semantics, ie barrier before.  */
# define atomic_compare_and_exchange_val_rel(mem, _new, old)      \
  __atomic_val_bysize (__arch_compare_and_exchange_val, int,    \
                       mem, _new, old, __ATOMIC_RELEASE)
/* Compare and exchange.
   For all "bool" routines, we return FALSE if exchange succesful.  */
# define __arch_compare_and_exchange_bool_8_int(mem, newval, oldval, model) \
  ({__arm_link_error (); 0; })
# define __arch_compare_and_exchange_bool_16_int(mem, newval, oldval, model) \
  ({__arm_link_error (); 0; })
# define __arch_compare_and_exchange_bool_32_int(mem, newval, oldval, model) \
  ({                                                                    \
    typeof (*mem) __oldval = (oldval);                                  \
    !__atomic_compare_exchange_n (mem, (void *) &__oldval, newval, 0,   \
                                  model, __ATOMIC_RELAXED);             \
  })
# define __arch_compare_and_exchange_bool_64_int(mem, newval, oldval, model) \
  ({__arm_link_error (); 0; })
# define __arch_compare_and_exchange_val_8_int(mem, newval, oldval, model) \
  ({__arm_link_error (); oldval; })
# define __arch_compare_and_exchange_val_16_int(mem, newval, oldval, model) \
  ({__arm_link_error (); oldval; })
# define __arch_compare_and_exchange_val_32_int(mem, newval, oldval, model) \
  ({                                                                    \
    typeof (*mem) __oldval = (oldval);                                  \
    __atomic_compare_exchange_n (mem, (void *) &__oldval, newval, 0,    \
                                 model, __ATOMIC_RELAXED);              \
    __oldval;                                                           \
  })
# define __arch_compare_and_exchange_val_64_int(mem, newval, oldval, model) \
  ({__arm_link_error (); oldval; })


# ifdef __arch_compare_and_exchange_bool_32_acq
#  define atomic_compare_and_exchange_bool_acq(mem, newval, oldval) \
  __atomic_bool_bysize (__arch_compare_and_exchange_bool,acq,		      \
		        mem, newval, oldval)
# else
#  define atomic_compare_and_exchange_bool_acq(mem, newval, oldval) \
  ({ /* Cannot use __oldval here, because macros later in this file might     \
	call this macro with __oldval argument.	 */			      \
     __typeof (oldval) __atg3_old = (oldval);				      \
     atomic_compare_and_exchange_val_acq (mem, newval, __atg3_old)	      \
       != __atg3_old;							      \
  })
# endif

//#define __arch_compare_and_exchange_val_32_acq(mem, newval, oldval)	\
//  ({									\
//     register int r2 asm ("r2");					\
//     register int* r4 asm ("r4") = (int*)(mem);				\
//     register int r5 asm ("r5");					\
//     register int r6 asm ("r6") = (int)(newval);			\
//     int retval, orig_oldval = (int)(oldval);				\
//     long kernel_cmpxchg = 0x1004;					\
//     while (1)								\
//       {								\
//         r5 = *r4;							\
//	 if (r5 != orig_oldval)						\
//	   {								\
//	     retval = r5;						\
//	     break;							\
//	   }								\
//	 asm volatile ("callr %1\n"					\
//		       : "=r" (r2)					\
//		       : "r" (kernel_cmpxchg), "r" (r4), "r" (r5), "r" (r6));				\
//	 if (!r2) { retval = orig_oldval; break; }			\
//       }								\
//     (__typeof (*(mem))) retval;					\
//  })

# define __arch_compare_and_exchange_val_32_acq(mem, newval, oldval) \
  __arm_assisted_compare_and_exchange_val_32_acq ((mem), (newval), (oldval))

#ifdef __thumb2__
/* Thumb-2 has ldrex/strex.  However it does not have barrier instructions,
   so we still need to use the kernel helper.  */
# define __arm_assisted_compare_and_exchange_val_32_acq(mem, newval, oldval) \
  ({ union { __typeof (oldval) a; uint32_t v; } oldval_arg = { .a = (oldval) };\
     union { __typeof (newval) a; uint32_t v; } newval_arg = { .a = (newval) };\
     register uint32_t a_oldval asm ("r0");				      \
     register uint32_t a_newval asm ("r1") = newval_arg.v;		      \
     register __typeof (mem) a_ptr asm ("r2") = (mem);			      \
     register uint32_t a_tmp asm ("r3");				      \
     register uint32_t a_oldval2 asm ("r4") = oldval_arg.v;		      \
     __asm__ __volatile__						      \
	     ("0:\tldr\t%[tmp],[%[ptr]]\n\t"				      \
	      "cmp\t%[tmp], %[old2]\n\t"				      \
	      "bne\t1f\n\t"						      \
	      "mov\t%[old], %[old2]\n\t"				      \
	      "movw\t%[tmp], #0x0fc0\n\t"				      \
	      "movt\t%[tmp], #0xffff\n\t"				      \
	      "blx\t%[tmp]\n\t"						      \
	      "bcc\t0b\n\t"						      \
	      "mov\t%[tmp], %[old2]\n\t"				      \
	      "1:"							      \
	      : [old] "=&r" (a_oldval), [tmp] "=&r" (a_tmp)		      \
	      : [new] "r" (a_newval), [ptr] "r" (a_ptr),		      \
		[old2] "r" (a_oldval2)					      \
	      : "ip", "lr", "cc", "memory");				      \
     (__typeof (oldval)) a_tmp; })
#else
# define __arm_assisted_compare_and_exchange_val_32_acq(mem, newval, oldval) \
  ({ union { __typeof (oldval) a; uint32_t v; } oldval_arg = { .a = (oldval) };\
     union { __typeof (newval) a; uint32_t v; } newval_arg = { .a = (newval) };\
     register uint32_t a_oldval asm ("r0");				      \
     register uint32_t a_newval asm ("r1") = newval_arg.v;		      \
     register __typeof (mem) a_ptr asm ("r2") = (mem);			      \
     register uint32_t a_tmp asm ("r3");				      \
     register uint32_t a_oldval2 asm ("r4") = oldval_arg.v;		      \
     __asm__ __volatile__						      \
	     ("0:\tldr\t%[tmp],[%[ptr]]\n\t"				      \
	      "cmp\t%[tmp], %[old2]\n\t"				      \
	      "bne\t1f\n\t"						      \
	      "mov\t%[old], %[old2]\n\t"				      \
	      "mov\t%[tmp], #0xffff0fff\n\t"				      \
	      "mov\tlr, pc\n\t"						      \
	      "add\tpc, %[tmp], #(0xffff0fc0 - 0xffff0fff)\n\t"		      \
	      "bcc\t0b\n\t"						      \
	      "mov\t%[tmp], %[old2]\n\t"				      \
	      "1:"							      \
	      : [old] "=&r" (a_oldval), [tmp] "=&r" (a_tmp)		      \
	      : [new] "r" (a_newval), [ptr] "r" (a_ptr),		      \
		[old2] "r" (a_oldval2)					      \
	      : "ip", "lr", "cc", "memory");				      \
     (__typeof (oldval)) a_tmp; })
#endif

#define __arch_compare_and_exchange_bool_32_acq(mem, newval, oldval)	\
  ({									\
     register int r2 asm ("r2");					\
     register int *r4 asm ("r4") = (int*)(mem);				\
     register int r5 asm ("r5") = (int)(oldval);			\
     register int r6 asm ("r6") = (int)(newval);			\
     long kernel_cmpxchg = 0x1004;					\
     asm volatile ("callr %1\n"						\
		   : "=r" (r2)						\
		   : "r" (kernel_cmpxchg), "r" (r4), "r" (r5), "r" (r6));					\
     r2;								\
  })


# define atomic_bit_test_set(mem, bit) \
  ({ __typeof (*(mem)) __atg14_old;					      \
     __typeof (mem) __atg14_memp = (mem);				      \
     __typeof (*(mem)) __atg14_mask = ((__typeof (*(mem))) 1 << (bit));	      \
									      \
     do									      \
       __atg14_old = (*__atg14_memp);					      \
     while (__builtin_expect						      \
	    (atomic_compare_and_exchange_bool_acq (__atg14_memp,	      \
						   __atg14_old | __atg14_mask,\
						   __atg14_old), 0));	      \
									      \
     __atg14_old & __atg14_mask; })


#define atomic_full_barrier()  ({ asm volatile ("sync"); })

#endif /* _NIOS2_ATOMIC_MACHINE_H */