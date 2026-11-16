//
// Created by dorish25 on 14.11.2026.
//

#ifndef ADALGO_GIFTWRAPPING_H
#define ADALGO_GIFTWRAPPING_H
#include <vector>

#include "ConvexHull.h"
#include "enums.h"


class GiftWrapping : public ConvexHull {
private:
    PointCloud* _motherPointCloud = nullptr;

    size_t _current = 0;
    size_t _favourite = 0;
    size_t _start = 0;   // Startpunkt (minX); wieder dort angekommen = Hülle geschlossen
    // ein Eintrag pro Punkt (1 = schon in der Hülle), Zugriff über den Index
    // ist viel schneller als ein unordered_set (kein Hash, kein Suchen)
    std::vector<char> _visited;

    giftwrapp_state _state = giftwrapp_state::find_first_favourite;
    int _index_bestfavourite = 0;

    // Gemeinsame Bausteine: Visual (Step) und Performance benutzen genau diese,
    // damit beide Modi garantiert denselben Algorithmus ausführen
    bool IsVisited(size_t i) const { return _visited[i]; }
    void ChooseFirstFavourite();        // erster unbesuchter Punkt != current
    void CheckCandidate(size_t i);      // Punkt i wird Favorit, wenn er "besser" ist
    void AddFavourite();                // Favorit in die Hülle, wird zum neuen current
    void SkipVisitedCandidates();       // _index_bestfavourite über besuchte Punkte schieben

public:

    void StartAlgorithmPerformance(PointCloud &point_cloud) override;

    void Init(PointCloud &point_cloud) override;
    void StepFirstFavourite();
    void StepBestFavourite();
    void StepAddFavourite();

    void Step() override;

    // für Visualisierung
    size_t GetCurrent() const { return _current; }
    size_t GetFavourite() const { return _favourite; }
    int GetBestFavourtite() const { return _index_bestfavourite; }
    giftwrapp_state GetState() const { return _state; }
    const char* GetStateName() const;
};


#endif //ADALGO_GIFTWRAPPING_H
