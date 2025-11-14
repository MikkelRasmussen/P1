#include "parking-spot.h"
#include "../project/project.h"

void add_parking_spot(Project *project, Vector2 position, float width, float height, char zone) {
    if (project->parking_spots_capacity == 0) {
        project->parking_spots_capacity = 10;
        project->parking_spots = (ParkingSpot*)malloc(sizeof(ParkingSpot) * project->parking_spots_capacity);
        if (!project->parking_spots) return;
    }

    if (project->parking_spots_count >= project->parking_spots_capacity) {
        project->parking_spots_capacity *= 2;
        ParkingSpot *new_array = (ParkingSpot*)realloc(project->parking_spots,
                                         sizeof(ParkingSpot) * project->parking_spots_capacity);
        if (!new_array) return;
        project->parking_spots = new_array;
    }

    ParkingSpot spot = {
        .position = position,
        .width = width,
        .height = height,
        .spot_id = project->parking_spots_count + 1,
        .occupied = false,
        .floor = project->active_floor,
        .zone = zone,
        .type = Default
    };

    project->parking_spots[project->parking_spots_count++] = spot;
}
