//
// Created by dorish25 on 14.11.2026.
//

#ifndef ADALGO_GIFTWRAPPING_H
#define ADALGO_GIFTWRAPPING_H
#include <unordered_set>

#include "ConvexHull.h"
#include "enums.h"


class GiftWrapping : public ConvexHull {
private:
    PointCloud* _motherPointCloud = nullptr;

    std::vector<Point> pointsInHull;

    size_t _current = 0;
    size_t _favourite = 0;
    size_t _start = 0;   // Startpunkt (minX); wieder dort angekommen = Hülle geschlossen
    std::unordered_set<size_t> _pointsAddedIndex;

    giftwrapp_state _state = giftwrapp_state::find_favourite;
    int _index_bestfavourite = 0;

public:
    void StartAlgorithmVisual(PointCloud &point_cloud) override;

    void StartAlgorithmPerformance(PointCloud &point_cloud) override;

    void Init(PointCloud &point_cloud) override;
    void StepFavourite();
    void StepBestFavourite();
    void StepAddFavourite();

    void Step() override;

    // für die Visualisierung
    size_t GetCurrent() const { return _current; }
    size_t GetFavourite() const { return _favourite; }
    int GetFavourtiteCandidate() const { return _index_bestfavourite; }
    giftwrapp_state GetState() const { return _state; }
    const char* GetStateName() const;
};


#endif //ADALGO_GIFTWRAPPING_H
