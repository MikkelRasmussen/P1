#include "parking-spot.h"

ParkingSpot init_parking_spot(int floor, char zone, int spot, SpotType type) {
  return (ParkingSpot){floor, zone, spot, type};
}