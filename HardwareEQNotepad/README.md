# Hardware EQ Notepad — v0.2.0

Een VST3/AU-plugin die **geen audio verwerkt**. Het is een digitaal
geheugensteuntje voor de instellingen van je externe hardware-eq
(4-bands: LO / LO-MID / HI-MID / HI, met shelving op LO en HI, plus een
Range-schakelaar x0.25 / x0.5 / x1). De standen komen exact overeen met
de gedetenteerde schakelaars op de hardware.

Je vult de waarden in die op je hardware staan ingesteld, de plugin
onthoudt ze via je DAW-sessie, en bij een volgende sessie zie je precies
hoe je de hardware weer moet instellen.

## Parameters

| Band     | Freq-standen                                                        | Gain            | Extra         |
|----------|----------------------------------------------------------------------|------------------|---------------|
| LO       | 30, 40, 50, 100, 200, 300, 400 Hz                                    | -12..+12 dB, 2dB-stappen | Shelf aan/uit |
| LO-MID   | 75, 150, 180, 240, 500, 700, 1k Hz                                   | -12..+12 dB, 2dB-stappen | —             |
| HI-MID   | 800, 1.5k, 3k, 5k, 8k, 10k, 12.5k Hz                                 | -12..+12 dB, 2dB-stappen | —             |
| HI       | 2.5k, 5k, 7k, 10k, 12.5k, 15k, 20k Hz                                | -12..+12 dB, 2dB-stappen | Shelf aan/uit |
| Range    | —                                                                    | —                | x0.25 / x0.5 / x1 |

## Kant-en-klare plugin krijgen (geen code nodig)

Ik kan hier geen macOS-plugin compileren (deze omgeving is Linux, en
Apple's build-tools werken alleen op een Mac). In plaats daarvan bevat dit
project een **GitHub Actions-workflow** die dat automatisch voor je doet op
een gratis Apple-machine in de cloud. Zo gebruik je die, zonder ooit code
te schrijven:

1. Maak een gratis account op [github.com](https://github.com) (indien je die nog niet hebt).
2. Klik rechtsboven op **"+" → "New repository"**. Geef een naam (bv. `hardware-eq-notepad`), laat 'm **Private** of Public, en klik **Create repository**.
3. Op de nieuwe, lege repository-pagina klik je op **"uploading an existing file"** (link in het midden van de pagina).
4. Sleep de hele inhoud van deze map (`HardwareEQNotepad/`, inclusief de verborgen map `.github`) naar het upload-vak, en klik **Commit changes**.
   - Let op: sommige besturingssystemen verbergen mappen die met een punt beginnen (`.github`). Zorg dat je die apart meeneemt als je "sleep de map" gebruikt — gebruik anders GitHub Desktop of `git push` als je daar bekend mee bent.
5. Ga naar het tabblad **"Actions"** bovenin de repository. Je ziet een workflow **"Build macOS plugin"** automatisch starten (duurt ongeveer 5-10 minuten).
6. Als hij groen is afgerond, klik je erop en scroll je naar beneden naar **"Artifacts"**. Daar staan twee zip-bestanden:
   - `HardwareEQNotepad-VST3` → bevat het `.vst3`-bestand
   - `HardwareEQNotepad-AU` → bevat het `.component`-bestand (Audio Unit, handig voor Logic Pro)
7. Download de zip, pak 'm uit, en verplaats:
   - het `.vst3`-bestand naar `~/Library/Audio/Plug-Ins/VST3/`
   - het `.component`-bestand naar `~/Library/Audio/Plug-Ins/Components/`
8. Herscan plugins in je DAW. Klaar!

Wil je later een aanpassing (andere frequenties, extra band, etc.)? Dan
pas ik de code aan, jij zet de nieuwe bestanden weer in dezelfde
GitHub-repository (stap 3-4), en de Actions-workflow bouwt automatisch
een nieuwe versie.

## Zelf bouwen op je eigen Mac (optioneel, alternatief voor bovenstaande)

Alleen nodig als je liever lokaal bouwt i.p.v. via GitHub:

1. Installeer Xcode (met command line tools) en CMake (`brew install cmake`).
2. Clone JUCE naast dit project:
   ```
   jouw-map/
     HardwareEQNotepad/
     JUCE/
   ```
   ```bash
   git clone --branch 8.0.4 --depth 1 https://github.com/juce-framework/JUCE.git
   ```
3. Configureer en bouw:
   ```bash
   cd HardwareEQNotepad
   cmake -B build -G Xcode
   cmake --build build --config Release
   ```
4. De plugins verschijnen automatisch op de standaardlocaties dankzij
   `COPY_PLUGIN_AFTER_BUILD TRUE` in `CMakeLists.txt`.

## Versienummer aanpassen

Pas `PLUGIN_VERSION` bovenaan `CMakeLists.txt` aan. Dat nummer verschijnt
automatisch rechtsboven in de plugin-GUI. Zie `CHANGELOG.md` voor de
wijzigingsgeschiedenis.

## Bekende beperkingen (v0.2.0)

- Geen los presets-systeem — alles wordt bewaard via de DAW-sessie.
- Geen audioverwerking, puur "notitieblok"-functionaliteit.
