#include <fstream>
#include "TrackStorage.h"

using std::exception;
using std::endl;

using std::vector;

TrackStorage::TrackStorage(string storageFilePath, AudioFormatManager &formatManager) :
        storageFilePath(storageFilePath),
        formatManager(formatManager) {
    if (formatManager.getNumKnownFormats() == 0) {
        formatManager.registerBasicFormats();
    }
    readTracks();
}

void TrackStorage::addTrack(File& file) {

    std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));
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
    vector<TrackInfo>::iterator trackInfo = find_if(tracks.begin(), tracks.end(), [&trackFilePath](const TrackInfo &trackInfo) {
        return trackInfo.filePath == trackFilePath;
    });

    if (trackInfo != tracks.end()) {
        tracks.erase(trackInfo);
        saveTracks();
    }
}

vector<TrackInfo> TrackStorage::getTracks() {
    return tracks;
}

void TrackStorage::readTracks() {
    tracks = vector<TrackInfo>{};
    std::ifstream storageFile{storageFilePath};

    if (!storageFile.good()) {
        return;
    }

    string line;
    if (storageFile.is_open()) {
        while (std::getline(storageFile, line)) {
            try {
                if (line.empty()) {
                    continue;
                }
                string delimiter = "\1";
                int delimiterPos = line.find(delimiter);
                string name = line.substr(0, delimiterPos);
                string filePath = line.substr(delimiterPos + 1);
                int durationSeconds = 0;

                std::ifstream audioFile{filePath};
                std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(File{filePath}));
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
//                cout << "CSVReader::readCSV bad data" << endl;
            }
        } // end of while

        storageFile.close();
    }

}

void TrackStorage::saveTracks() {
    std::ofstream storageFile{storageFilePath};
    for (TrackInfo const &track: tracks) {
        storageFile << track.name << "\1" << track.filePath << endl;
    }
    storageFile.close();
}