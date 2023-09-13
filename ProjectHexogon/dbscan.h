#pragma once

#include <vector>

struct DBPoint {
    double x, y;

    bool operator==(const DBPoint& other) const {
        return x == other.x && y == other.y;
    }
};

std::vector<DBPoint> region_query(const std::vector<DBPoint>& points, const DBPoint& p, double eps);
std::vector<std::vector<DBPoint>> dbscan(std::vector<DBPoint> points, double eps, int min_pts);
std::vector<DBPoint> get_centroids(std::vector<std::vector<DBPoint>> clusters);
void expand_cluster(std::vector<DBPoint>& points, int p_idx, std::vector<int>& cluster, int cluster_id, double eps, int min_pts);