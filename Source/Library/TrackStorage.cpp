#include <fstream>
#include "TrackStorage.h"

using std::exception;
using std::endl;
using std::vector;
using std::unique_ptr;
using std::ifstream;
using std::getline;
using std::copy_if;
using std::back_inserter;
using std::ofstream;

TrackStorage::TrackStorage(string storageFilePath, AudioFormatManager &formatManager) :
        storageFilePath(storageFilePath),
        formatManager(formatManager) {
    if (formatManager.getNumKnownFormats() == 0) {
        formatManager.registerBasicFormats();
    }
    readTracks();
}

void TrackStorage::addTrack(File &file) {

    unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader) {
        auto durationSeconds = static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
        tracks.emplace_back(
                file.getFileName().toStdString(),
                file.getFullPathName().toStdString(),
                durationSeconds
        );
    } else {
        AlertWindow::showMessageBoxAsync(
                MessageBoxIconType::WarningIcon,
                "ERROR!",
                "Wrong audio file",
                "OK"
        );
    }

    saveTracks();
}

void TrackStorage::deleteTrack(string trackFilePath) {
    vector<TrackInfo>::iterator trackInfo = find_if(tracks.begin(), tracks.end(),
                                                    [&trackFilePath](const TrackInfo &trackInfo) {
                                                        return trackInfo.filePath.compare(trackFilePath) == 0;
                                                    });

    if (trackInfo != tracks.end()) {
        tracks.erase(trackInfo);
        saveTracks();
    }
}

vector<TrackInfo> TrackStorage::getTracks(const string &nameFilter) {
    if (nameFilter.empty()) {
        return tracks;
    }

    vector<TrackInfo> result;

    copy_if(tracks.begin(), tracks.end(), back_inserter(result), [&nameFilter, this](const TrackInfo &trackInfo) {
        return to_lower(trackInfo.name).find(to_lower(nameFilter)) != string::npos;
    });

    return result;
}

void TrackStorage::readTracks() {
    tracks = vector<TrackInfo>{};
    ifstream storageFile{storageFilePath};

    if (!storageFile.good()) {
        return;
    }

    string line;
    if (storageFile.is_open()) {
        while (getline(storageFile, line)) {
            try {
                if (line.empty()) {
                    continue;
                }
                string delimiter = "\1";
                int delimiterPos = line.find(delimiter);
                string name = line.substr(0, delimiterPos);
                string filePath = line.substr(delimiterPos + 1);
                int durationSeconds = 0;

                ifstream audioFile{filePath};
                unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(File{filePath}));
                if (reader) {
                    durationSeconds = static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
                } else {
                    AlertWindow::showMessageBoxAsync(
                            MessageBoxIconType::WarningIcon,
                            "ERROR!",
                            "Wrong audio file",
                            "OK"
                    );
                }

                tracks.emplace_back(name, filePath, durationSeconds);
            } catch (const exception &e) {
                DBG(e.what());
            }
        }

        storageFile.close();
    }

}

void TrackStorage::saveTracks() {
    ofstream storageFile{storageFilePath};
    for (TrackInfo const &track: tracks) {
        storageFile << track.name << "\1" << track.filePath << endl;
    }
    storageFile.close();
}

string TrackStorage::to_lower(string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}