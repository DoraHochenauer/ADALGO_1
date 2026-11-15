#include "QuickHull.h"

#include "PointCloud.h"

#include <utility>

void QuickHull::Init(PointCloud &point_cloud) {
    _motherPointCloud = &point_cloud;

    // falls Init mehrmals aufgerufen wird: alte Segmente weg, sonst wird alles doppelt gerechnet
    _stack.clear();

    // minX und maxX liegen sicher auf der Hülle
    size_t left = point_cloud.GetMinXIndex();
    size_t right = point_cloud.GetMaxXIndex();

    // alle Punkte haben dasselbe x (senkrechte Linie) -> minX == maxX, keine Trennlinie möglich
    // -> stattdessen minY und maxY nehmen
    if (left == right) {
        left = point_cloud.GetMinYIndex();
        right = point_cloud.GetMaxYIndex();
    }
    _start = left;

    _discarded.assign(point_cloud.GetCountPoints(), false);

    Vector2 l = point_cloud.GetPoint(left).GetPoint();
    Vector2 r = point_cloud.GetPoint(right).GetPoint();

    // Linie left -> right teilt alle Punkte in zwei Hälften.
    // Wie PointsOutside, nur beide Seiten in einem Durchlauf (statt 2x über alle Punkte)
    Segment upper{left, right, {}};
    Segment lower{right, left, {}};
    for (int i = 0; i < point_cloud.GetCountPoints(); i++) {
        float cp = CrossProduct(l, r, point_cloud.GetPoint(i).GetPoint());
        if (cp > 0)
            upper.outside.push_back(i);
        else if (cp < 0)
            lower.outside.push_back(i);   // cp < 0 bei left -> right  ==  cp > 0 bei right -> left
        else if (i != left && i != right)
            _discarded[i] = true;         // liegt genau auf der Linie -> kein Eckpunkt
    }

    _stack.push_back(std::move(lower));
    _stack.push_back(std::move(upper));   // liegt oben am Stack -> wird zuerst bearbeitet

    _state = quickhull_state::choose_segment;
}


void QuickHull::Step() {
    switch (_state) {
        case quickhull_state::choose_segment:
            StepChooseSegment();
            break;
        case quickhull_state::find_farthest:
            StepFindFarthest();
            break;
        case quickhull_state::split:
            StepSplit();
            break;
        case quickhull_state::done:
            break;
    }
}

const char *QuickHull::GetStateName() const {
    switch (_state) {
        case quickhull_state::choose_segment: return "Naechstes Segment waehlen";
        case quickhull_state::find_farthest: return "Weitesten Punkt suchen";
        case quickhull_state::split: return "Segment aufteilen";
        case quickhull_state::done: return "Fertig";
    }
    return "";
}

//--------------------------------------------------------------------------
// GEMEINSAME BAUSTEINE (von Visual und Performance benutzt)
//--------------------------------------------------------------------------

std::vector<size_t> QuickHull::PointsOutside(size_t a, size_t b, const std::vector<size_t> &candidates) const {
    Vector2 pa = _motherPointCloud->GetPoint(a).GetPoint();
    Vector2 pb = _motherPointCloud->GetPoint(b).GetPoint();

    std::vector<size_t> result;
    for (size_t i : candidates) {
        // strikt > 0: Punkte genau auf der Linie a -> b sind keine Eckpunkte und fallen raus
        // (Vorzeichen ist egal, solange es überall gleich ist -> bestimmt nur die Umlaufrichtung)
        if (CrossProduct(pa, pb, _motherPointCloud->GetPoint(i).GetPoint()) > 0)
            result.push_back(i);
    }
    return result;
}

void QuickHull::ChooseSegment() {
    _segment = std::move(_stack.back());
    _stack.pop_back();
}

void QuickHull::AddEdge() {
    // nur a hinzufügen: b kommt als a vom nächsten Segment (bzw. ist am Ende der Startpunkt)
    AddPoint(_motherPointCloud->GetPoint(_segment.a));   // färbt auch grün
}

void QuickHull::ChooseFirstFarthest() {
    _farthest = _segment.outside[0];
    _farthestDistance = -1.0f;
    _index_candidate = 0;
}

void QuickHull::CheckCandidate(size_t i) {
    Vector2 a = _motherPointCloud->GetPoint(_segment.a).GetPoint();
    Vector2 b = _motherPointCloud->GetPoint(_segment.b).GetPoint();
    Vector2 p = _motherPointCloud->GetPoint(i).GetPoint();

    // Kreuzprodukt = |ab| * Abstand von p zur Linie, |ab| ist fürs ganze Segment gleich
    // -> zum Vergleichen reicht das Kreuzprodukt, keine Wurzel nötig
    // (alle Punkte in outside haben cp > 0, darum kein abs)
    float distance = CrossProduct(a, b, p);

    if (distance > _farthestDistance) {
        _farthestDistance = distance;
        _farthest = i;
    }
}

void QuickHull::SplitSegment() {
    // nur Punkte außerhalb von a -> f bzw. f -> b bleiben, die im Dreieck a-f-b fallen raus
    std::vector<size_t> right = PointsOutside(_farthest, _segment.b, _segment.outside);
    std::vector<size_t> left = PointsOutside(_segment.a, _farthest, _segment.outside);

    _stack.push_back(Segment{_farthest, _segment.b, std::move(right)});
    _stack.push_back(Segment{_segment.a, _farthest, std::move(left)});   // links zuerst -> Hülle in Reihenfolge
}

void QuickHull::Finish() {
    //ENDE! Startpunkt nochmal, damit die Hülle geschlossen gezeichnet wird (wie bei GiftWrapping)
    AddPoint(_motherPointCloud->GetPoint(_start));
    _state = quickhull_state::done;
}

//--------------------------------------------------------------------------
// VISUAL: ein Teilschritt pro Step()
//--------------------------------------------------------------------------

void QuickHull::StepChooseSegment() {
    if (_stack.empty()) {
        Finish();
        return;
    }

    ChooseSegment();

    if (_segment.outside.empty()) {
        AddEdge();
        if (_stack.empty())
            Finish();
        return;   // bleibt in choose_segment
    }

    ChooseFirstFarthest();
    _state = quickhull_state::find_farthest;
}

void QuickHull::StepFindFarthest() {
    if (HasCandidate()) {
        CheckCandidate(_segment.outside[_index_candidate]);
    }

    _index_candidate++;

    if (!HasCandidate())
        _state = quickhull_state::split;
}

void QuickHull::StepSplit() {
    SplitSegment();

    // nur für die Anzeige: alles aus dem Segment ausgrauen, außer f und den Punkten,
    // die in den zwei neuen Segmenten noch Kandidaten sind
    for (size_t i : _segment.outside)
        _discarded[i] = true;
    _discarded[_farthest] = false;
    for (size_t s = _stack.size() - 2; s < _stack.size(); s++)
        for (size_t i : _stack[s].outside)
            _discarded[i] = false;

    _state = quickhull_state::choose_segment;
}

//--------------------------------------------------------------------------
// PERFORMANCE: dieselben Bausteine, nur ohne Pausen dazwischen
//--------------------------------------------------------------------------

void QuickHull::StartAlgorithmPerformance(PointCloud &point_cloud) {
    while (!_stack.empty()) {
        ChooseSegment();

        if (_segment.outside.empty()) {
            AddEdge();
            continue;
        }

        ChooseFirstFarthest();
        for (size_t i : _segment.outside)
            CheckCandidate(i);

        SplitSegment();
    }

    Finish();
}
