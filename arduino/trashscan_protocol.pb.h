#ifndef PB_TRASHSCAN_PROTOCOL_PB_H_INCLUDED
#define PB_TRASHSCAN_PROTOCOL_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _BIN_STATUS {
    float SENSOR_1;
    float SENSOR_2;
    float SENSOR_3;
    float SENSOR_4;
} BIN_STATUS;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define BIN_STATUS_init_default                  {0, 0, 0, 0}
#define BIN_STATUS_init_zero                     {0, 0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define BIN_STATUS_SENSOR_1_tag                  1
#define BIN_STATUS_SENSOR_2_tag                  2
#define BIN_STATUS_SENSOR_3_tag                  3
#define BIN_STATUS_SENSOR_4_tag                  4

/* Struct field encoding specification for nanopb */
#define BIN_STATUS_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, FLOAT,    SENSOR_1,          1) \
X(a, STATIC,   SINGULAR, FLOAT,    SENSOR_2,          2) \
X(a, STATIC,   SINGULAR, FLOAT,    SENSOR_3,          3) \
X(a, STATIC,   SINGULAR, FLOAT,    SENSOR_4,          4)
#define BIN_STATUS_CALLBACK NULL
#define BIN_STATUS_DEFAULT NULL

extern const pb_msgdesc_t BIN_STATUS_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define BIN_STATUS_fields &BIN_STATUS_msg

/* Maximum encoded size of messages (where known) */
#define BIN_STATUS_size                          20
#define TRASHSCAN_PROTOCOL_PB_H_MAX_SIZE         BIN_STATUS_size

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
