#include <at/core.h>

/// @cond IGNORE
#define AT_DEFINE_ARRAY_OP(op)                    \
op(u8 ,U8 ); op(u16,U16); op(u32,U32); op(u64,U64);\
op(i8 ,I8 ); op(i16,I16); op(i32,I32); op(i64,I64);

#define AT_DEFINE_ARRAY_OP3(op)                                                    \
op(u8 ,U8 ,uint8_t );op(u16,U16,uint16_t);op(u32,U32,uint32_t);op(u64,U64,uint64_t);\
op(i8 ,I8 , int8_t );op(i16,I16, int16_t);op(i32,I32, int32_t);op(i64,I64, int64_t);\


#define AT_DEFINE_ARRAY_JACCARD(lower,UPPER)     \
double                                            \
at_array##lower##_jaccard(AtArray##UPPER* a1, AtArray##UPPER* a2){ \
  uint64_t un=0,in=0, i;                          \
  for(i = 0; i < a1->h.num_elements;i++){         \
    un += a1->data[i] | a2->data[i];              \
    in += a1->data[i] & a2->data[i];              \
  }                                               \
  return (double)in/(double)un;                   \
}

#define AT_DEFINE_ARRAY_DICE(lower,UPPER)         \
double                                            \
at_array##lower##_dice(AtArray##UPPER* a1, AtArray##UPPER* a2){ \
  uint64_t un=0,in=0, i;                          \
  for(i = 0; i < a1->h.num_elements;i++){         \
    un += a1->data[i] + a2->data[i];              \
    in += a1->data[i] & a2->data[i];              \
  }                                               \
  return (double)(in << 1)/(double)un;            \
}

AT_DEFINE_ARRAY_OP(AT_DEFINE_ARRAY_JACCARD)
AT_DEFINE_ARRAY_OP(AT_DEFINE_ARRAY_DICE)

#undef AT_DEFINE_ARRAY_JACCARD
#undef AT_DEFINE_ARRAY_DICE

#undef AT_DEFINE_ARRAY_OP
#undef AT_DEFINE_ARRAY_OP3

/// @endcond
