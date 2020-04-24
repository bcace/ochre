#ifndef ocdescriptor_h
#define ocdescriptor_h

#include "enums.h"
#include "ocstring.h"
#include "runtime/memmanagement.h"
#include <vector>

#define OCN(ocf_name) void ocf_name(void **)
#define OCF(ocf_name) void ocf_name(void **v)
#define OCP(ocp_type,ocp_index) (*((ocp_type *)v[ocp_index]))
#define OCR(ocp_type,ocp_index) ((ocp_type *)v[ocp_index])

typedef void (*EXEC)(void **);

#endif
