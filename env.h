#ifndef _ENV_H
#define _ENV_H

#ifdef __cplusplus
#ifndef __auto_type
#define __auto_type auto
#endif
extern "C" {
#endif
#include<stdint.h>
#define GAMMA 0.9f
#define THETA 1e-1f
#define NUM 20
#define REW 10
#define COST 2
#define NUM_ACTS 11
#define NUM_STATES (NUM+1)*(NUM+1)
#define MAX(a, b) ({__auto_type _m=(a);__auto_type _n=(b);_m>_n?_m:_n;})
#define MIN(a, b) ({__auto_type _m=(a);__auto_type _n=(b);_m<_n?_m:_n;})
#define ABS(a) ({__auto_type _t=(a);_t>0?_t:-_t;})
#define FR(s) ((s)/(NUM+1))
#define TO(s) ((s)%(NUM+1))
#define ARGMAX(arr, n) ({\
    int i, m;\
    for (m=i=0; i<n; i++)\
        if (arr[i]>arr[m])\
            m = i;\
    m;\
})

typedef uint16_t state_t;
typedef int8_t action_t;

extern float values[];
extern action_t policy[]; //[NUM_ACTS];

#endif
void init(void);
float step(state_t, action_t);
void export_policy(const char[], const int);
void export_values(const char[]);
float max_value(state_t, action_t*);
#ifdef __cplusplus
}
#endif
