#include "motion_detection.h"

int run()
{
    SensorPoint sp;
    urg_t urg;
    vector<vector<SensorPoint::Centroid>> history;

    if (open_urg_sensor(&urg) < 0) {
        return 1;
    }

    sp.run_motion_detection(&urg, history, &SensorPoint::print_data, &SensorPoint::print_xy_coordinates);

    //sp.print_xy_coordinates();


#if defined(URG_MSC)
    getchar();
#endif

    urg_close(&urg);

    return 0;
}


int main() 
{
    run();
}