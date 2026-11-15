# Testprotokoll Assignment 1 – Convex Hull

## 1. Aufgabenstellung

Laut Angabe sollen wir 2 Convex-Hull-Algorithmen für 2D implementieren, visualisieren und ihre Performance vergleichen. Zur Auswahl standen Andrew’s algorithm, Gift wrapping, Quickhull und Divide & Conquer. **Wir haben Gift Wrapping und Quickhull gewählt.**

Anforderungen:

- Eingabe: Random generated test data und File import (1. Zeile n, danach pro Zeile x,y)
- Performance optimized mode: Ergebnis und Zeit für beide Algorithmen
- Visual mode: Hülle Schritt für Schritt zeichnen
- Test protocol mit Zeitmessungen (größere Datenmengen) und Big-O-Vergleich

## 2. Unsere Umsetzung

Wir verwenden C++ (in der Angabe empfohlen) und raylib zum Zeichnen. Visual und Performance Mode nutzen dieselben Funktionen.

| Teil | Umsetzung |
| --- | --- |
| Gruppe | Offene Frage: Namen der beiden Gruppenmitglieder eintragen |
| Algorithmen | `GiftWrapping` und `QuickHull` (mit Stack statt Rekursion, damit der Visual Mode nach jedem Schritt anhalten kann) |
| Gemeinsame Basis | `ConvexHull` mit `Init`, `Step` und `StartAlgorithmPerformance` |
| Eingabedaten | Zufall, Kreis, Linie, Datei |
| Visual Mode | Pfeil rechts = nächster Schritt |
| Performance Mode | Zeit und Hüllpunkte beider Algorithmen |

## 3. Unsere ersten Annahmen

Vor dem Testen haben wir aufgeschrieben, was wir erwarten (`zusammenschrift.txt`):

| Algorithmus | Fall | Unsere Annahme |
| --- | --- | --- |
| Gift Wrapping | Best Case | alle Punkte auf einem Kreis |
| Gift Wrapping | Crash Case | Linie |
| Gift Wrapping | Worst Case | noch offen |
| QuickHull | Best Case | Linie |
| QuickHull | Worst Case | alle Punkte auf einem Kreis |

## 4. Best/Worst Case mit Big O

n = Anzahl aller Punkte, h = Anzahl der Hüllpunkte.

| Algorithmus | Best Case | Worst Case |
| --- | --- | --- |
| Gift Wrapping | O(n): wenige Hüllpunkte (Linie, h = 2) | O(n²): alle Punkte auf der Hülle (Kreis, h = n) |
| QuickHull | O(n): Linie, nichts aufzuteilen | O(n²): jede Aufteilung trennt nur einen Punkt ab |

**Gift Wrapping:** Pro Hüllpunkt einmal alle n Punkte durchgehen, also O(n · h).

**QuickHull:** Jedes Segment wird in zwei Teile geteilt. Halbieren sich die Teile, ist es O(n log n) wie bei Quicksort, sonst bis O(n²).

## 5. Testfälle

Punktformen aus unserem Programm und kleine Importdateien für Sonderfälle:

| Testfall | Wie erzeugt | Was wir erwarten |
| --- | --- | --- |
| Zufallspunkte | `pointcloud_mode::random_points` | wenige Hüllpunkte, beide Algorithmen schnell |
| Kreis | `pointcloud_mode::circle` | alle Punkte auf der Hülle |
| Linie | `pointcloud_mode::line` | nur 2 Hüllpunkte |
| Datei `importPoints` | `pointcloud_mode::file` | 2 Punkte, Hülle = beide Punkte |
| Gleichstand bei minX | Importdatei mit `0,5` `0,0` `0,10` `10,5` | Hülle (0,0), (10,5), (0,10) |
| Senkrechte Linie | Importdatei mit `1,4` `1,0` `1,2` | Hülle (1,0), (1,4) |

Gemessen mit `std::chrono::steady_clock` im Release-Build, nur der Algorithmus (ohne Erzeugen, Einlesen, Zeichnen).

## 6. Annahmen vs. was wir beim Testen gelernt haben

Zwei unserer Annahmen waren falsch: Der Kreis ist der Worst Case von Gift Wrapping, aber nicht von QuickHull.

| Algorithmus | Fall | Unsere Annahme | Was wir jetzt wissen |
| --- | --- | --- | --- |
| Gift Wrapping | Best Case | Kreis | **Falsch.** Der Kreis ist der Worst Case (h = n). Best Case ist die Linie (h = 2). |
| Gift Wrapping | Worst Case | noch offen | **Kreis**, allgemein: alle Punkte auf der Hülle, O(n²). |
| Gift Wrapping | Crash Case | Linie | **Behoben.** Bei Punkten auf einer Linie wird der weiter entfernte genommen. |
| QuickHull | Best Case | Linie | Alle Punkte liegen auf der Trennlinie, nichts aufzuteilen. |
| QuickHull | Worst Case | Kreis | **Falsch.** Beim Kreis wird jedes Mal halbiert, also O(n log n). |

### Der echte Worst Case von QuickHull

Alle Punkte auf der Hülle reicht nicht, es kommt auf die Aufteilung an:

- **Kreis:** Der weiteste Punkt liegt in der Bogenmitte, die Punkte werden halbiert: 16 → 8 + 8 → 4 + 4 + 4 + 4 → … = O(n log n).
- **Worst Case:** Der weiteste Punkt liegt immer am Rand, nur ein Punkt wird abgetrennt: 16 → 1 + 15 → 1 + 14 → … = O(n²). Z.B. Punkte auf einem Bogen, die zu einem Ende hin immer enger werden.

Wie bei Quicksort: Pivot in der Mitte ist schnell, Pivot am Rand langsam. Mit vielen Punkten können wir das nicht testen, weil `float` die engen Punkte nicht mehr unterscheiden kann.

### Probleme, die uns beim Testen aufgefallen sind

- **Gift Wrapping, Gleichstand bei minX:** Haben mehrere Punkte das kleinste x, kann eine falsche Hülle entstehen, z.B. bei (0,5), (0,0), (0,10), (10,5). Mögliche Lösung: bei gleichem x den Punkt mit kleinerem y als Start nehmen.
- **QuickHull, senkrechte Linie:** Haben alle Punkte dasselbe x, gibt es keine Trennlinie. Behoben: QuickHull nimmt dann minY und maxY.
- **Rechengenauigkeit mit `float`:** Beim Kreis mit vielen Punkten liegen Nachbarpunkte so eng, dass das Kreuzprodukt 0 wird. Deshalb werden weniger als n Hüllpunkte gezählt (siehe Messtabelle).

## 7. Zeitmessungen

Gemessen im Performance Mode (Release-Build) auf AMD Ryzen 9 9900X, 64 GB RAM, Windows 11 Pro, GCC 15.2. Jede Messung 3-mal, eingetragen ist der Median. Eingestellt in `main.cpp` über `programMode`, `selectedMode` und `numberPoints`.

| Testfall | n | Hüllpunkte GW | Hüllpunkte QH | Gift Wrapping (ms) | QuickHull (ms) |
| --- | --- | --- | --- | --- | --- |
| Kreis | 1.000 | 1.000 | 1.000 | 3,37 | 0,207 |
| Kreis | 50.000 | 18.652 | 18.655 | 7.534 | 7,51 |
| Kreis | 100.000 | 21.457 | 21.461 | 20.809 | 14,0 |
| Kreis | 1.000.000 | 31.711 | 31.716 | 330.141 (1 Lauf) | 113 (1 Lauf) |
| Zufall | 1.000 | 17 bis 24 | 17 bis 24 | 0,112 | 0,044 |
| Zufall | 50.000 | 26 bis 34 | 26 bis 34 | 8,39 | 1,23 |
| Zufall | 100.000 | 30 bis 39 | 30 bis 39 | 18,9 | 2,43 |
| Zufall | 1.000.000 | 35 bis 39 | 35 bis 39 | 188 | 21,7 |
| Linie | 1.000 | 2 | 2 | 0,031 | 0,008 |
| Linie | 50.000 | 2 | 2 | 1,28 | 0,109 |
| Linie | 100.000 | 2 | 2 | 2,57 | 0,22 |
| Linie | 1.000.000 | 2 | 2 | 25,5 | 2,11 |
| Datei `importPoints` | 2 | 2 | 2 | 0,008 | 0,009 |

Bei Zufall sind die Punkte bei jedem Lauf andere, deshalb schwankt die Anzahl der Hüllpunkte.

### Alle Messläufe

Die einzelnen Läufe zu den Medianen oben (`cmake-build-release\ADALGO.exe`).

| Testfall | n | Lauf | Gift Wrapping (ms) | QuickHull (ms) | Hüllpunkte GW | Hüllpunkte QH |
| --- | --- | --- | --- | --- | --- | --- |
| Kreis | 1.000 | 1 | 3,275 | 0,210 | 1.000 | 1.000 |
| Kreis | 1.000 | 2 | 3,379 | 0,183 | 1.000 | 1.000 |
| Kreis | 1.000 | 3 | 3,372 | 0,207 | 1.000 | 1.000 |
| Kreis | 50.000 | 1 | 7.534,1 | 7,494 | 18.652 | 18.655 |
| Kreis | 50.000 | 2 | 7.525,6 | 7,508 | 18.652 | 18.655 |
| Kreis | 50.000 | 3 | 7.575,0 | 7,539 | 18.652 | 18.655 |
| Kreis | 100.000 | 1 | 20.808,6 | 14,034 | 21.457 | 21.461 |
| Kreis | 100.000 | 2 | 20.461,1 | 14,445 | 21.457 | 21.461 |
| Kreis | 100.000 | 3 | 21.048,5 | 13,913 | 21.457 | 21.461 |
| Kreis | 1.000.000 | 1 | 330.141 | 113,225 | 31.711 | 31.716 |
| Zufall | 1.000 | 1 | 0,157 | 0,049 | 24 | 24 |
| Zufall | 1.000 | 2 | 0,112 | 0,044 | 17 | 17 |
| Zufall | 1.000 | 3 | 0,110 | 0,040 | 18 | 18 |
| Zufall | 50.000 | 1 | 8,392 | 1,235 | 31 | 31 |
| Zufall | 50.000 | 2 | 6,830 | 1,148 | 26 | 26 |
| Zufall | 50.000 | 3 | 9,196 | 1,283 | 34 | 34 |
| Zufall | 100.000 | 1 | 18,880 | 2,307 | 36 | 36 |
| Zufall | 100.000 | 2 | 20,839 | 2,432 | 39 | 39 |
| Zufall | 100.000 | 3 | 13,377 | 2,498 | 30 | 30 |
| Zufall | 1.000.000 | 1 | 194,157 | 22,000 | 39 | 39 |
| Zufall | 1.000.000 | 2 | 179,613 | 18,396 | 37 | 37 |
| Zufall | 1.000.000 | 3 | 187,571 | 21,664 | 35 | 35 |
| Linie | 1.000 | 1 | 0,032 | 0,011 | 2 | 2 |
| Linie | 1.000 | 2 | 0,031 | 0,008 | 2 | 2 |
| Linie | 1.000 | 3 | 0,031 | 0,008 | 2 | 2 |
| Linie | 50.000 | 1 | 1,261 | 0,113 | 2 | 2 |
| Linie | 50.000 | 2 | 1,277 | 0,107 | 2 | 2 |
| Linie | 50.000 | 3 | 1,284 | 0,109 | 2 | 2 |
| Linie | 100.000 | 1 | 2,568 | 0,220 | 2 | 2 |
| Linie | 100.000 | 2 | 2,612 | 0,217 | 2 | 2 |
| Linie | 100.000 | 3 | 2,539 | 0,216 | 2 | 2 |
| Linie | 1.000.000 | 1 | 25,515 | 2,121 | 2 | 2 |
| Linie | 1.000.000 | 2 | 25,193 | 2,106 | 2 | 2 |
| Linie | 1.000.000 | 3 | 25,708 | 2,115 | 2 | 2 |
| Datei `importPoints` | 2 | 1 | 0,0079 | 0,0092 | 2 | 2 |
| Datei `importPoints` | 2 | 2 | 0,0079 | 0,0100 | 2 | 2 |
| Datei `importPoints` | 2 | 3 | 0,0054 | 0,0062 | 2 | 2 |

Kreis mit 1.000.000 Punkten nur 1 Lauf, weil Gift Wrapping dafür 5,5 Minuten braucht.

Erwartung laut Big O: Beim Kreis wächst Gift Wrapping quadratisch, QuickHull mit n log n. Bei Zufall und Linie wachsen beide ungefähr linear.

## 8. Fazit

Entscheidend ist, wie viele Punkte auf der Hülle liegen:

- **Wenige (Zufall, Linie):** Beide fast linear.
- **Viele (Kreis):** Gift Wrapping quadratisch, QuickHull O(n log n). QuickHull gewinnt klar.
- **QuickHull-Worst-Case:** nur bei speziell angeordneten Punkten (Kapitel 6).

Gift Wrapping ist einfacher zu verstehen und zu zeichnen, QuickHull bei vielen Hüllpunkten viel schneller.

Offene Frage: Fazit nach den Zeitmessungen mit unseren echten Zahlen ergänzen.
