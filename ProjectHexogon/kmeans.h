#pragma once

#include <omp.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Point
{
private:
    int pointId, clusterId;
    int dimensions;
    vector<double> values;

public:
    Point(int id, string line);

    int getDimensions();

    int getCluster();

    int getID();

    void setCluster(int val);

    double getVal(int pos);
};

class Cluster
{
private:
    int clusterId;
    vector<double> centroid;
    vector<Point> points;

public:
    Cluster(int clusterId, Point centroid);

    void addPoint(Point p);

    bool removePoint(int pointId);

    void removeAllPoints();

    int getId();

    Point getPoint(int pos);

    int getSize();

    double getCentroidByPos(int pos) const;

    void setCentroidByPos(int pos, double val);
};

class KMeans
{
private:
    int K, iters, dimensions, total_points;
    std::vector<Cluster> clusters;

    void clearClusters();
    int getNearestClusterId(Point point);

public:
    KMeans(int K, int iterations);

    void run(vector<Point>& all_points);

    const vector<Cluster>& getClusters() const;
};
