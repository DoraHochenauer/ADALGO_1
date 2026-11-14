//
// Created by dorish25 on 14.11.2026.
//

#include "GiftWrapping.h"

#include "PointCloud.h"

#include <unordered_set>

void GiftWrapping::StartAlgorithmVisual(PointCloud &point_cloud) {
    //INIT
    std::unordered_set<size_t> pointsAddedIndex;

    size_t current = point_cloud.GetMinXIndex();
    pointsAddedIndex.insert(current);
    pointsAddedIndex.insert(current);
    size_t end = point_cloud.GetMaxXIndex();

    float currentCp = 0;
    size_t favourite;


    while (current != end) {
        //find first favourite, not visited
        for (int i = 0; i < point_cloud.GetCountPoints(); i++) {
            favourite = i;
            if (!pointsAddedIndex.contains(i))
                break;
        }

        //besten favouriten finden
        for (int i = 0; i < point_cloud.GetCountPoints(); i++) {

            if (pointsAddedIndex.contains(i))
                continue;

            float cp = CrossProduct(point_cloud.GetPoint(current).GetPoint(),
                                    point_cloud.GetPoint(favourite).GetPoint(),
                                    point_cloud.GetPointCloud()[i].GetPoint());

            if (cp > 0) {
                favourite = i;
            }

            //currentCp = cp;
        }

        AddPoint(point_cloud.GetPoint(favourite));
        point_cloud.GetPoint(current).SetColor(GREEN);

        current = favourite;
        pointsAddedIndex.insert(current);

    }
}

void GiftWrapping::StartAlgorithmPerformance(PointCloud &point_cloud) {
}
