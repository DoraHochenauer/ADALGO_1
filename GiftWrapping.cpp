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

void GiftWrapping::StepBestFavourite() {
    if (_index_bestfavourite < _motherPointCloud->GetCountPoints()) {
        float cp = CrossProduct(_motherPointCloud->GetPoint(_current).GetPoint(),
                                _motherPointCloud->GetPoint(_favourite).GetPoint(),
                                _motherPointCloud->GetPointCloud()[_index_bestfavourite].GetPoint());


        if (cp > 0 || (cp == 0 && Distance(_motherPointCloud->GetPointCloud()[_current].GetPoint(),
                                           _motherPointCloud->GetPointCloud()[_index_bestfavourite].GetPoint()) >
                       Distance(_motherPointCloud->GetPointCloud()[_current].GetPoint(),
                                _motherPointCloud->GetPointCloud()[_index_bestfavourite].GetPoint()))) {
            _favourite = _index_bestfavourite;
        }
    }
    _index_bestfavourite++;

    // gleich über besuchte Punkte springen, damit der nächste Kandidat (graue Linie) nie ein Hüllpunkt ist
    while (_index_bestfavourite < _motherPointCloud->GetCountPoints() && _pointsAddedIndex.
           contains(_index_bestfavourite))
        _index_bestfavourite++;

    if (_index_bestfavourite >= _motherPointCloud->GetCountPoints())
        _state = giftwrapp_state::add_favourite;
}

void GiftWrapping::StepAddFavourite() {
    AddPoint(_motherPointCloud->GetPoint(_favourite));
    _motherPointCloud->GetPoint(_current).SetColor(GREEN);

    _current = _favourite;
    _pointsAddedIndex.insert(_current);

    //ENDE! wieder beim Start
    _state = (_current == _start)
                 ? giftwrapp_state::done
                 : giftwrapp_state::find_first_favourite;
}

void GiftWrapping::StepFirstFavourite() {
    //find first _favourite, not visited ,und nicht current =es hört auf
    for (int i = 0; i < _motherPointCloud->GetCountPoints(); i++) {
        _favourite = i;
        if (i != _current && !_pointsAddedIndex.contains(i))
            break;
    }

    _index_bestfavourite = 0;
    // falls Punkt 0 schon in der Hülle ist -> zum ersten unbesuchten springen
    while (_index_bestfavourite < _motherPointCloud->GetCountPoints() && _pointsAddedIndex.
           contains(_index_bestfavourite))
        _index_bestfavourite++;
    _state = giftwrapp_state::find_best_favourite;
}


//PERFORMANCE
void GiftWrapping::StartAlgorithmPerformance(PointCloud &point_cloud) {
    //FÜR EDGE CASE LINIE WENN ALLE cp 0 = ALLE AUF LINIE SIND
    float sumcp = 0;
    float maxDist = 0;
    float currentDist = 0;
    size_t indexMaxDist;

    do {
        //find first _favourite, not visited (und nicht current selbst)
        for (int i = 0; i < _motherPointCloud->GetCountPoints(); i++) {
            _favourite = i;
            if (i != _current && !_pointsAddedIndex.contains(i))
                break;
        }


        //besten _favouriten finden
        for (int i = 0; i < _motherPointCloud->GetCountPoints(); i++) {
            if (_pointsAddedIndex.contains(i))
                continue;

            float cp = CrossProduct(_motherPointCloud->GetPoint(_current).GetPoint(),
                                    _motherPointCloud->GetPoint(_favourite).GetPoint(),
                                    _motherPointCloud->GetPointCloud()[i].GetPoint());
            sumcp += cp;

            if (cp > 0) {
                _favourite = i;
            }

            if (cp == 0) {
                currentDist = ConvexHull::Distance(_motherPointCloud->GetPoint(_current).GetPoint(),
                                                   _motherPointCloud->GetPoint(_favourite).GetPoint());

                if (currentDist > maxDist) {
                    maxDist = currentDist;
                    indexMaxDist = i;
                }
            }
        }

        // edge case Line
        if (sumcp == 0) {
            _favourite = indexMaxDist;
            AddPoint(_motherPointCloud->GetPoint(_favourite));

            _state = giftwrapp_state::done;
        }

        AddPoint(_motherPointCloud->GetPoint(_favourite));

        _current = _favourite;
        _pointsAddedIndex.insert(_current);
    } while (_current != _start);
}
