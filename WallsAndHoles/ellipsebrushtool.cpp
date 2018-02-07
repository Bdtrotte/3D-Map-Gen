#include <QtMath>


#include "ellipsebrushtool.h"


EllipseBrushTool::EllipseBrushTool(MapView *mapView, TileMap *tileMap, SharedTileTemplate drawWith)
    : AbstractShapeBrushTool(mapView, tileMap, drawWith)
{

}


static double getXTheta(double cs, double bestTheta, double minTheta) {
    double t1 = acos(cs);

    if (t1 > minTheta)
        return std::min(bestTheta, t1);

    double t2 = 2*M_PI - t1;

    if (t2 > minTheta)
        return std::min(bestTheta, t2);

    return bestTheta;
}

static double getYTheta(double sn, double bestTheta, double minTheta) {
    double t1 = asin(sn);
    double t2 = M_PI - t1;
    if (t1 < 0) {
        t1 = t1 + 2 * M_PI;

        if (t2 > minTheta)
            return std::min(bestTheta, t2);

        if (t1 > minTheta)
            return std::min(bestTheta, t1);

        return bestTheta;
    } else {
        if (t1 > minTheta)
            return std::min(bestTheta, t1);

        if (t2 > minTheta)
            return std::min(bestTheta, t2);

        return bestTheta;
    }
}


QVector<QPoint> EllipseBrushTool::getShape(int dx, int dy) const {

    /*
     * Currently, this method works by intersecting an ellipse with a grid.
     * The while loop sweeps through angles between 0 and 2pi in as few iterations
     * as possible while hitting every grid square.
     *
     *
     * There is a much easier and faster way of drawing an ellipse:
     *  1) Only draw one quadrant, then reflect.
     *  2) To draw the quadrant, just intersect the ellipse with lines.
     * If this function becomes buggy, reimplement using this process.
     * */

    QVector<QPoint> points;

    if (dx == 0) {
        for (int y = 0; y <= abs(dy); ++y)
            points.push_back(QPoint(0, dy < 0 ? -y : y));
        return points;
    }

    if (dy == 0) {
        for (int x = 0; x <= abs(dx); ++x)
            points.push_back(QPoint(dx < 0 ? -x : x, 0));
        return points;
    }

    double w = abs(dx);
    double h = abs(dy);

    if (dx >= 0) ++w;
    if (dy >= 0) ++h;

    // Center coordinates.
    double cx = w * 0.5 * (dx < 0 ? -1 : 1);
    double cy = h * 0.5 * (dy < 0 ? -1 : 1);

    // Theta will sweep from 0 to 2 pi.
    double theta = 0;


    // x and y "radii"
    double rx = .5 * w - 0.8;
    double ry = .5 * h - 0.8;

    double x = cx + rx * cos(theta);
    double y = cy + ry * sin(theta);

    // "Small angle" that is small enough not to skip too many tiles, but is large
    // enough so that the while() loop doesn't iterate too many times.
    double smallAngle = 0.02 * std::min(fabs(atan(rx / ry)), fabs(atan(ry / rx)));

    while (theta <= 2 * M_PI) {
        points.push_back(QPoint(floor(x), floor(y)));


        double newTheta = 2*M_PI;

        if (x == round(x)) {
            double nx1 = x - 1;
            double nx2 = x + 1;

            if (nx1 <= cx + rx && nx1 >= cx - rx)
                newTheta = getXTheta( (nx1 - cx) / rx, newTheta, theta );

            if (nx2 <= cx + rx && nx2 >= cx - rx)
                newTheta = getXTheta( (nx2 - cx) / rx, newTheta, theta );



            double tx = 2*M_PI - acos( (x - cx) / rx );
            if (tx > theta)
                newTheta = std::min(newTheta, tx);

        } else {
            double nx1 = floor(x);
            double nx2 = ceil(x);

            if (nx1 <= cx + rx && nx1 >= cx - rx)
                newTheta = getXTheta( (nx1 - cx) / rx, newTheta, theta );

            if (nx2 <= cx + rx && nx2 >= cx - rx)
                newTheta = getXTheta( (nx2 - cx) / rx, newTheta, theta );
        }

        if (y == round(y)) {
            double ny1 = y - 1;
            double ny2 = y + 1;

            if (ny1 <= cy + ry && ny1 >= cy - ry)
                newTheta = getYTheta( (ny1 - cy) / ry, newTheta, theta );

            if (ny2 <= cy + ry && ny2 >= cy - ry)
                newTheta = getYTheta( (ny2 - cy) / ry, newTheta, theta );


            // This angle should always be less than or equal to theta.
            double ty = asin( (y - cy) / ry );

            if (ty < 0) {
                double t = 2*M_PI + ty;
                if (t > theta)
                    newTheta = std::min(t, newTheta);
            } else {
                double t = M_PI - ty;
                if (t > theta)
                    newTheta = std::min(t, newTheta);
            }

        } else {
            double ny1 = floor(y);
            double ny2 = ceil(y);

            if (ny1 <= cy + ry && ny1 >= cy - ry)
                newTheta = getYTheta( (ny1 - cy) / ry, newTheta, theta );

            if (ny2 <= cy + ry && ny2 >= cy - ry)
                newTheta = getYTheta( (ny2 - cy) / ry, newTheta, theta );
        }

        if (newTheta < theta)
            break;

        // Should almost never happen (but if it does, we're safe!)
        if (newTheta == theta)
            newTheta += smallAngle;


        theta = newTheta + smallAngle;

        x = cx + rx * cos(theta);
        y = cy + ry * sin(theta);
    }

    return points;
}
