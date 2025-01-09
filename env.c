#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"env.h"
#ifdef __cplusplus
extern "C" {
#endif

float values[NUM_STATES];
action_t policy[NUM_STATES];
float p[2][NUM+1][NUM+1]; /* 各地数目变动概率分布 */
float r[2][NUM+1]; /* 各状态奖励期望 */

void init(void) {
    const int lambda[] = {3,3,4,2};
    int t, i, j, m, n;
    uint64_t l;
    long double k;
    float _s;
    float (*_p)[NUM+1] = (float(*)[NUM+1])malloc(sizeof(float)*4*(NUM+1));
    for (i=0; i<4; i++) {
        t = lambda[i];
        _s = 0.;
        k = l = 1;
        for (j=0; j<=NUM; l*=++j,k*=t)
            _s += (_p[i][j] = k/l);
        for (j=0; j<=NUM; j++) /* 归一化 */
            _p[i][j] /= _s;
    }
    for (i=0; i<4; i+=2) {
        for (j=0; j<=NUM; j++) { /* 现有数量j */
            for (_s=t=0; t<=NUM; t++) { /* 求租数量t */
                m = j<t?j:t; /* 最大可租数量j */
                _s += _p[i][t]*m;
                m = j - m;
                for (n=0; n<=NUM; n++) { /* 归还数量n */
                    p[i>>1][j][MIN(m+n, NUM)] += _p[i][t]*_p[i|1][n];
                }
            }
            r[i>>1][j] = REW * _s;
        }
    }
    free(_p);
}

float step(state_t state, action_t act) {
    float _sum, _s;
    int8_t fr = FR(state), to = TO(state), _t;
    int16_t i, j;
    if (act > fr)
        act = fr;
    else if (act < (_t=-to))
        act = _t;
    fr = MIN(fr-act, NUM);
    to = MIN(to+act, NUM);
    for (_sum=i=0; i<NUM_STATES; i+=NUM+1) {
        for (_s=j=0; j<=NUM; j++)
            _s += values[i+j] * p[1][to][j];
        _sum += _s * p[0][fr][FR(i)];
    }
    return r[0][fr]+r[1][to] - ABS(act)*COST + GAMMA*_sum;
}

void export_policy(const char name[], int i) {
    char tp[11];
    int j;
    sprintf(tp, "%s%d.pgm", name, i);
    FILE *f = fopen(tp, "wb");
    fputs("P5\n21 21\n10\n", f);
    for (i=NUM*(NUM+1); i>=0; i-=NUM+1)
        for (j=0; j<=NUM; j++)
            fputc(5+policy[i+j], f);
    fclose(f);
}

void export_values(const char name[]) {
    float mx, mi;
    int i, j;
    mx = mi = values[0];
    for (i=1; i<NUM_STATES; i++) {
        if (values[i]>mx)
            mx = values[i];
        else if (values[i]<mi)
            mi = values[i];
    }
    //printf("max: %f, min: %f\n", mx, mi);
    mx = 255.1/(mx-mi);
    FILE *f = fopen(name, "wb");
    fputs("P5\n21 21\n255\n", f);
    for (i=NUM*(NUM+1); i>=0; i-=NUM+1)
        for (j=0; j<=NUM; j++)
            fputc(mx*(values[i+j]-mi), f);
    fclose(f);
}

float max_value(state_t s, action_t *ma) {
    action_t a = MAX(-5, FR(s)-NUM), t = -TO(s);
    float v, _r;
    if (t>a)
        a = t;
    t = MIN(NUM+t, 5);
    t = MIN(t, FR(s));
    if (ma) *ma = a;
    for (v=-INFINITY; a<=t; a++) {
        _r = step(s, a);
        if (_r > v) {
            if (ma) *ma = a;
            v = _r;
        }
    }
    return v;
}
#ifdef __cplusplus
}
#endif
