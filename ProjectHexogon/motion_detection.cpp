#include "motion_detection.h"


void bubbleSort(vector<Centroid>& centroids) {
    for (int i = 0; i < centroids.size() - 1; i++) {
        for (int j = 0; j < centroids.size() - i - 1; j++) {
            if (centroids[j].x > centroids[j + 1].x) {
                swap(centroids[j], centroids[j + 1]);
            }
        }
    }
}

void print_data(urg_t* urg, long data[], int data_n, long time_stamp, vector<vector<Centroid>>& history)
{
    (void)time_stamp;

    int i;
    long min_distance;
    long max_distance;

    // Set the number of clusters
    int K = 1;

    // Create a vector of Point objects
    vector<Point> all_points;
    int pointId = 0;

    // Prints the X-Y coordinates for all the measurement points
    urg_distance_min_max(urg, &min_distance, &max_distance);
    for (i = 0; i < data_n; ++i) {
        long l = data[i];
        double radian;
        long x;
        long y;

        if ((l <= min_distance) || (l >= max_distance)) {
            continue;
        }
        radian = urg_index2rad(urg, i);
        x = (long)(l * sin(radian));
        y = (long)(l * cos(radian));
        stringstream ss;
        ss << x << " " << y;
        Point point(pointId, ss.str());
        all_points.push_back(point);
        pointId++;
    }

    // Set a fixed seed (starting point) for the random number generator
    srand(2222);

    // Run the KMeans clustering algorithm
    int iters = 100;
    KMeans kmeans(K, iters);
    kmeans.run(all_points);

    // Print the final cluster assignments for each point
    /*for (int i = 0; i < all_points.size(); i++)
    {
        cout << "Point " << i << ": (" << all_points[i].getVal(0) << ", " << all_points[i].getVal(1) << ") -> Cluster " << all_points[i].getCluster() << endl;
    }*/

    // Create a vector to store the coordinates of the centroids at this time step
    vector<Centroid> current_centroids(K);
    const vector<Cluster>& clusters = kmeans.getClusters();

    for (int i = 0; i < K; i++)
    {
        double x = clusters[i].getCentroidByPos(0);
        double y = clusters[i].getCentroidByPos(1);

        // Store the coordinates of the centroid in the current_centroids vector
        current_centroids[i] = { x, y };
    }

    bubbleSort(current_centroids); //Sort the current_centroids vector

    // Print the sorted current_centroids vector
    for (int i = 0; i < current_centroids.size(); i++) {
        cout << "Centroid " << i + 1 << ": (" << current_centroids[i].x << ", " << current_centroids[i].y << ")" << endl;
    }

    // Add the current_centroids vector to the history vector
    history.push_back(current_centroids);

    // Check if there is enough data in the history vector to determine movement
    if (history.size() >= 2)
    {
        // Get the previous and current centroids from the history vector
        const vector<Centroid>& prev_centroids = history[history.size() - 2];
        const vector<Centroid>& curr_centroids = history[history.size() - 1];

        // Calculate the movement vectors for each cluster
        for (int i = 0; i < K; i++)
        {
            double dx = curr_centroids[i].x - prev_centroids[i].x;
            double dy = curr_centroids[i].y - prev_centroids[i].y;
            double n = 10;
            double m = 5;

            // Calculate the distance between the current and previous positions
            double dist = sqrt(dx * dx + dy * dy);

            // Check if the distance is greater than the threshold value n
            if (dist > n)
            {
                // Determine the direction of movement based on the movement vector
                string direction;
                if (dx > m)
                    direction = "left";
                else if (dx < -m)
                    direction = "right";
                /*if (abs(dx) > abs(dy))
                {
                    if (dx > 0)
                        direction = "up";
                    else
                        direction = "down";
                }
                else
                {
                    cout << dx << endl;
                    if (dy > 0)
                        direction = "left";
                    else
                        direction = "right";
                }*/
                else
                    break;
                cout << "Cluster " << i + 1 << " is moving: " << direction << endl;
            }
            else
            {
                /*cout << "Cluster " << i + 1 << " is not moving" << endl;*/
            }
        }
    }
}

void run_motion_detection(urg_t* urg, vector<vector<Centroid>>& history)
{
    enum {
        CAPTURE_TIMES = 9999,
    };
    long* data = NULL;
    long time_stamp;
    int n;
    int i;

    data = (long*)malloc(urg_max_data_size(urg) * sizeof(data[0]));
    if (!data) {
        perror("urg_max_index()");
        return;
    }

    // Gets measurement data
#if 1
    // Case where the measurement range (start/end steps) is defined
    urg_set_scanning_parameter(urg,
        urg_deg2step(urg, -30),
        urg_deg2step(urg, +30), 0);
#endif

    urg_start_measurement(urg, URG_DISTANCE, URG_SCAN_INFINITY, 0, 1);
    for (i = 0; i < CAPTURE_TIMES; ++i) {
        n = urg_get_distance(urg, data, &time_stamp);
        if (n <= 0) {
            printf("urg_get_distance: %s\n", urg_error(urg));
            free(data);
            return;
        }
        print_data(urg, data, n, time_stamp, history);
    }

    // Disconnects
    free(data);
}
