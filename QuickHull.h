#ifndef ADALGO_QUICKHULL_H
#define ADALGO_QUICKHULL_H
#include <vector>

#include "ConvexHull.h"
#include "enums.h"


// Ein offenes Teilproblem: Kante a -> b und alle Punkte, die außerhalb davon liegen
struct Segment {
    size_t a = 0;
    size_t b = 0;
    std::vector<size_t> outside;   // Indizes in der Punktwolke
};


class QuickHull : public ConvexHull {
private:
    PointCloud* _motherPointCloud = nullptr;

    size_t _start = 0;   // Startpunkt (minX); am Ende nochmal hinzufügen = Hülle geschlossen

    // Stack statt Rekursion: so kann Step() nach jedem Teilschritt anhalten
    std::vector<Segment> _stack;
    Segment _segment;              // gerade bearbeitetes Segment

    size_t _farthest = 0;          // bisher weitester Punkt vom Segment
    float _farthestDistance = -1.0f;
    size_t _index_candidate = 0;   // Position in _segment.outside (nicht in der Punktwolke!)

    std::vector<bool> _discarded;  // nur Visual: Punkte im Dreieck, die rausgefallen sind

    quickhull_state _state = quickhull_state::choose_segment;

    // Gemeinsame Bausteine: Visual (Step) und Performance benutzen genau diese,
    // damit beide Modi garantiert denselben Algorithmus ausführen
    std::vector<size_t> PointsOutside(size_t a, size_t b, const std::vector<size_t>& candidates) const;
    void ChooseSegment();          // oberstes Segment vom Stack holen
    void AddEdge();                // Segment ohne Punkte außen: a -> b ist Hüllkante
    void ChooseFirstFarthest();    // erster Punkt außerhalb ist erstmal der weiteste
    void CheckCandidate(size_t i); // Punkt i wird weitester, wenn er weiter weg ist
    void SplitSegment();           // a -> f und f -> b auf den Stack, Dreieck fällt raus
    void Finish();                 // Startpunkt nochmal hinzufügen

public:

    void StartAlgorithmPerformance(PointCloud &point_cloud) override;

    void Init(PointCloud &point_cloud) override;
    void StepChooseSegment();
    void StepFindFarthest();
    void StepSplit();

    void Step() override;

    // für Visualisierung
    const Segment& GetSegment() const { return _segment; }
    const std::vector<Segment>& GetOpenSegments() const { return _stack; }
    size_t GetFarthest() const { return _farthest; }
    // Punkt, der gerade geprüft wird (oder ungültig, wenn alle geprüft sind)
    bool HasCandidate() const { return _index_candidate < _segment.outside.size(); }
    size_t GetCandidate() const { return _segment.outside[_index_candidate]; }
    bool IsDiscarded(size_t i) const { return i < _discarded.size() && _discarded[i]; }
    quickhull_state GetState() const { return _state; }
    const char* GetStateName() const;
};


#endif //ADALGO_QUICKHULL_H
