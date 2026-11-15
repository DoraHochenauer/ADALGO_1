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
        case giftwrapp_state::find_favourite:
            StepFavourite();
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

const char* GiftWrapping::GetStateName() const {
    switch (_state) {
        case giftwrapp_state::find_favourite:      return "Startfavorit aussuchen";
        case giftwrapp_state::find_best_favourite: return "Besten Favoriten suchen";
        case giftwrapp_state::add_favourite:       return "Favorit zur Huelle hinzufuegen";
        case giftwrapp_state::done:                return "Fertig";
    }
    return "";
}

void GiftWrapping::StepBestFavourite() {
    if (_index_bestfavourite < _motherPointCloud->GetCountPoints() && !(_pointsAddedIndex.
            contains(_index_bestfavourite))) {
        float cp = CrossProduct(_motherPointCloud->GetPoint(_current).GetPoint(),
                                _motherPointCloud->GetPoint(_favourite).GetPoint(),
                                _motherPointCloud->GetPointCloud()[_index_bestfavourite].GetPoint());

        if (cp > 0) {
            _favourite = _index_bestfavourite;
        }
    }
    _index_bestfavourite++;

    if (_index_bestfavourite == _motherPointCloud->GetCountPoints())
        _state = giftwrapp_state::add_favourite;
}

void GiftWrapping::StepAddFavourite() {
    AddPoint(_motherPointCloud->GetPoint(_favourite));
    _motherPointCloud->GetPoint(_current).SetColor(GREEN);

    _current = _favourite;
    _pointsAddedIndex.insert(_current);

    //ENDE! wieder beim Start
    _state = (_current == _start) ? giftwrapp_state::done
                                  : giftwrapp_state::find_favourite;
}

void GiftWrapping::StepFavourite() {
    //find first _favourite, not visited ,und nicht current = Kreuzprodukt wäre 0 und es hört auf
    for (int i = 0; i < _motherPointCloud->GetCountPoints(); i++) {
        _favourite = i;
        if (i != _current && !_pointsAddedIndex.contains(i))
            break;
    }

    _index_bestfavourite = 0;
    _state = giftwrapp_state::find_best_favourite;
}


void GiftWrapping::StartAlgorithmVisual(PointCloud &point_cloud) {
}

//PERFORMANCE
void GiftWrapping::StartAlgorithmPerformance(PointCloud &point_cloud) {

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

            if (cp > 0) {
                _favourite = i;
            }

            if (cp == 0) {
                //EDGE CASE LINIE WENN ALLE 0 SIND
            }
        }

        AddPoint(_motherPointCloud->GetPoint(_favourite));

        _current = _favourite;
        _pointsAddedIndex.insert(_current);
    } while (_current != _start);
}
