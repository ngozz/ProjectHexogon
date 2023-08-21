#include "motion_detection.h"

int main()
{
    urg_t urg;
    vector<vector<Centroid>> history;

    if (open_urg_sensor(&urg) < 0) {
        return 1;
    }

    run_motion_detection(&urg, history);

#if defined(URG_MSC)
    getchar();
#endif

    urg_close(&urg);

    return 0;
}
