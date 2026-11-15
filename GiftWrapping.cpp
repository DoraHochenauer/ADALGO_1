//
// Created by dorish25 on 14.11.2026.
//

#include "GiftWrapping.h"

#include "PointCloud.h"

#include <unordered_set>

void GiftWrapping::Init(PointCloud &point_cloud) {
    _motherPointCloud = &point_cloud;
    _start = _current = point_cloud.GetMinXIndex();
    AddPoint(point_cloud.GetPoint(_current));
}


void GiftWrapping::Step() {
    switch (_state) {
        case giftwrapp_state::find_first_favourite:
            StepFirstFavourite();
            break;

        case giftwrapp_state::find_best_favourite:
            StepBestFavourite();
            break;
        case giftwrapp_state::add_favourite:
            StepAddFavourite();
            break;
        case giftwrapp_state::done:
            break;
    }
}

const char *GiftWrapping::GetStateName() const {
    switch (_state) {
        case giftwrapp_state::find_first_favourite: return "Ersten Favorit aussuchen";
        case giftwrapp_state::find_best_favourite: return "Besten Favoriten suchen";
        case giftwrapp_state::add_favourite: return "Favorit zur Huelle hinzufuegen";
        case giftwrapp_state::done: return "Fertig";
    }
    return "";
}

//--------------------------------------------------------------------------
// GEMEINSAME BAUSTEINE (von Visual und Performance benutzt)
//--------------------------------------------------------------------------

void GiftWrapping::ChooseFirstFavourite() {
    // erster unbesuchter Punkt, aber nicht current selbst (sonst wäre jedes Kreuzprodukt 0)
    for (int i = 0; i < _motherPointCloud->GetCountPoints(); i++) {
        _favourite = i;
        if (i != _current && !IsVisited(i))
            break;
    }
}

void GiftWrapping::CheckCandidate(size_t i) {
    Vector2 cur = _motherPointCloud->GetPoint(_current).GetPoint();
    Vector2 fav = _motherPointCloud->GetPoint(_favourite).GetPoint();
    Vector2 p   = _motherPointCloud->GetPoint(i).GetPoint();

    float cp = CrossProduct(cur, fav, p);

    // cp > 0: p liegt auf der falschen Seite von cur -> fav
    // cp == 0: auf einer Linie (z.B. Modus line) -> den weiter entfernten nehmen
    if (cp > 0 || (cp == 0 && Distance(cur, p) > Distance(cur, fav))) {
        _favourite = i;
    }
}

void GiftWrapping::AddFavourite() {
    AddPoint(_motherPointCloud->GetPoint(_favourite));   // färbt auch grün
    _current = _favourite;
    _pointsAddedIndex.insert(_current);
}

void GiftWrapping::SkipVisitedCandidates() {
    // damit der nächste Kandidat (graue Linie) nie ein Hüllpunkt ist
    while (_index_bestfavourite < _motherPointCloud->GetCountPoints() && IsVisited(_index_bestfavourite))
        _index_bestfavourite++;
}

//--------------------------------------------------------------------------
// VISUAL: ein Teilschritt pro Step()
//--------------------------------------------------------------------------

void GiftWrapping::StepFirstFavourite() {
    ChooseFirstFavourite();

    _index_bestfavourite = 0;
    SkipVisitedCandidates();
    _state = giftwrapp_state::find_best_favourite;
}

void GiftWrapping::StepBestFavourite() {
    if (_index_bestfavourite < _motherPointCloud->GetCountPoints()) {
        CheckCandidate(_index_bestfavourite);
    }

    _index_bestfavourite++;
    SkipVisitedCandidates();

    if (_index_bestfavourite >= _motherPointCloud->GetCountPoints())
        _state = giftwrapp_state::add_favourite;
}

void GiftWrapping::StepAddFavourite() {
    AddFavourite();

    //ENDE! wieder beim Start
    _state = (_current == _start)
                 ? giftwrapp_state::done
                 : giftwrapp_state::find_first_favourite;
}

//--------------------------------------------------------------------------
// PERFORMANCE: dieselben Bausteine, nur ohne Pausen dazwischen
//--------------------------------------------------------------------------

void GiftWrapping::StartAlgorithmPerformance(PointCloud &point_cloud) {
    do {
        ChooseFirstFavourite();

        for (int i = 0; i < _motherPointCloud->GetCountPoints(); i++) {
            if (IsVisited(i))
                continue;
            CheckCandidate(i);
        }

        AddFavourite();
    } while (_current != _start);

    _state = giftwrapp_state::done;
}
