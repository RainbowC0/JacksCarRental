#include<stdio.h>
#include"env.h"
#define STRA "poli"

int main(void) {
    state_t s;
    action_t ma;
    float delta, v, _r;
    int flag, itr = 0, cnt = 0;
    init();
    do { /* 策略迭代 */
        do { /* 策略评估 */
            delta = 0.;
            for (s=0; s<NUM_STATES; s++) {
                v = step(s, policy[s]);
                if ((_r=ABS(v-values[s])) > delta)
                    delta = _r;
                values[s] = v;
            }
            printf("max change: %f\n", delta);
            cnt++;
        } while (delta > THETA);
        /* 策略提升 */
        for (flag=0, s=0; s<NUM_STATES; s++) {
            max_value(s, &ma);
            if (ma != policy[s]) {
                flag = 1;
                policy[s] = ma;
            }
        }
        printf("\niter: %d\n", ++itr);
        export_policy(STRA, itr);
    } while (flag);
    printf("counts: %d\n", cnt);
    export_values(STRA"_v.pgm");
    return 0;
}

