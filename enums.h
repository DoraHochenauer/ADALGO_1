//
// Created by dorish25 on 12.11.2026.
//

#ifndef ADALGO_ENUMS_H
#define ADALGO_ENUMS_H


enum class pointcloud_mode {
    random_points, line, circle, performance, file
};


enum class program_mode {
    visual, performance
};

enum class algorithm {
    quickqull, gift_wrapping
};


enum class giftwrapp_state {
    find_first_favourite,    find_best_favourite, add_favourite, done
};

enum class quickhull_state {
    choose_segment, find_farthest, split, done
};

#endif //ADALGO_ENUMS_H
