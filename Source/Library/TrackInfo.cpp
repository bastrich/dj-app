#include "TrackInfo.h"

#include <utility>

TrackInfo::TrackInfo(const string name, const string filePath, const long durationSeconds)
        : name(name),
          filePath(filePath),
          durationSeconds(durationSeconds) {
}