#include<stdio.h>
#include"env.h"
#define STRA "valu"

int main(void) {
    state_t s;
    float delta, _r, _v;
    int cnt = 0;
    init();
    do { /* 价值迭代 */
       delta = 0.f;
       for (s=0; s<NUM_STATES; s++) {
           _v = max_value(s, NULL);
           if ((_r = ABS(_v - values[s])) > delta)
               delta = _r;
           values[s] = _v;
       }
       printf("max change: %f\n", delta);
       cnt++;
    } while (delta > THETA);
    for (s=0; s<NUM_STATES; s++)
        max_value(s, policy+s);
    printf("counts: %d\n", cnt);
    export_policy(STRA, 0);
    export_values(STRA"_v.pgm");
    return 0;
}

