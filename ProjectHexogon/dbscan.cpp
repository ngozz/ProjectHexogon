#include "dbscan.h"

#include <cmath>
#include <vector>

std::vector<DBPoint> region_query(const std::vector<DBPoint>& points, const DBPoint& p, double eps) {
    std::vector<DBPoint> neighbors;
    for (const DBPoint& q : points) {
        double dist = std::sqrt(std::pow(p.x - q.x, 2) + std::pow(p.y - q.y, 2));
        if (dist <= eps) {
            neighbors.push_back(q);
        }
    }
    return neighbors;
}

void expand_cluster(std::vector<DBPoint>& points, int p_idx, std::vector<int>& cluster, int cluster_id, double eps, int min_pts) {
    std::vector<DBPoint> seeds = region_query(points, points[p_idx], eps);
    if (seeds.size() < min_pts) {
        cluster[p_idx] = -1;
    }
    else {
        cluster[p_idx] = cluster_id;
        for (int i = 0; i < seeds.size(); i++) {
            int q_idx = find(points.begin(), points.end(), seeds[i]) - points.begin();
            if (cluster[q_idx] == 0) {
                cluster[q_idx] = cluster_id;
                expand_cluster(points, q_idx, cluster, cluster_id, eps, min_pts);
            }
        }
    }
}

std::vector<std::vector<DBPoint>> dbscan(std::vector<DBPoint> points, double eps, int min_pts) {
    int n = points.size();
    std::vector<int> cluster(n, 0);
    int cluster_id = 0;
    for (int i = 0; i < n; i++) {
        if (cluster[i] == 0) {
            std::vector<DBPoint> neighbors = region_query(points, points[i], eps);
            if (neighbors.size() < min_pts) {
                cluster[i] = -1;
            }
            else {
                cluster_id++;
                expand_cluster(points, i, cluster, cluster_id, eps, min_pts);
            }
        }
    }

    std::vector<std::vector<DBPoint>> clusters(cluster_id);
    for (int i = 0; i < n; i++) {
        if (cluster[i] > 0) {
            clusters[cluster[i] - 1].push_back(points[i]);
        }
    }

    return clusters;
}

std::vector<DBPoint> get_centroids(std::vector<std::vector<DBPoint>> clusters) {
    std::vector<DBPoint> centroids;
    for (auto cluster : clusters) {
        if (cluster.size() <= 1) {
            continue;
        }
        double sum_x = 0, sum_y = 0;
        for (auto point : cluster) {
            sum_x += point.x;
            sum_y += point.y;
        }
        DBPoint centroid = { sum_x / cluster.size(), sum_y / cluster.size() };
        centroids.push_back(centroid);
    }
    return centroids;
}
