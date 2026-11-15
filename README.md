# ADALGO Übung 1 - Konvexe Hülle

## Was man braucht

- CMake (mind. 4.3, steht so in der CMakeLists)
- Compiler mit C++20
- Internet beim ersten Build, raylib wird über FetchContent geladen
- für den Release Build noch Ninja

Getestet unter Windows mit CLion.
Unter Linux braucht raylib noch die X11/OpenGL dev Pakete.

## Bauen

Debug:
```bash
cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug
```

Release (für die Zeitmessung nehmen, ist viel schneller):
```bash
cmake --preset release
cmake --build --preset release
```

Die exe liegt dann im jeweiligen cmake-build Ordner.

## Einstellungen

Oben in der main.cpp unter "HIER KONFIGURIEREN":
- `pointcloud_mode`: random_points, line, circle, arc oder file
- `programMode`: visual oder performance (im performance Modus laufen immer beide Algorithmen)
- `algorithm`: gift_wrapping oder quickqull, welcher Algorithmus im visual Modus gezeigt wird
- `numberPoints`: wie viele Punkte (bei file egal)
- `importFile`: welche Datei geladen wird

Danach neu bauen. Im visual Modus geht man mit Pfeil rechts weiter.

## Punkte aus Datei

In der ersten Zeile steht die Anzahl, danach ein Punkt pro Zeile mit Komma getrennt, z.B.
```
2
26.755,5.201
4.752,301.021
```
Es gibt schon Dateien mit 100, 10000 und 100000 Punkten (importPoints100 usw.).

Achtung: der Pfad zum Projektordner wird beim Bauen fix eingetragen, wenn man den Ordner verschiebt muss man neu bauen.
