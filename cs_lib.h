#ifndef CS_LIB_H_
#define CS_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*EnterCS)(void const *Handle);
typedef void (*LeaveCS)(void const *Handle);
typedef struct S_CS_lib CS_lib_t;

struct S_CS_lib
{
    void const *Handle;
    EnterCS Enter;
    LeaveCS Leave;
};

#ifdef __cplusplus
}
#endif

#endif /* CS_LIB_H_ */
