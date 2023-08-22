#pragma once
#include "urg_sensor.h"
#include "urg_utils.h"
#include "open_urg_sensor.h"
#include <stdlib.h>
#include <stdio.h>
#include <corecrt_math.h>
#include <sstream>
#include "kmeans.h"

using namespace std;

class SensorPoint {
public:
    struct Centroid
    {
        double x;
        double y;
    };

    void bubbleSort(vector<Centroid>& centroids);
    void print_data(urg_t* urg, long data[], int data_n, long time_stamp, vector<vector<Centroid>>& history);
    void run_motion_detection(urg_t* urg, vector<vector<Centroid>>& history, void (SensorPoint::* data_function)(urg_t*, long[], int, long, vector<vector<Centroid>>&));
    void print_xy_coordinates(vector<Point>& all_points);
    vector<Point> get_xy_coordinates(urg_t* urg, long data[], int data_n);
};