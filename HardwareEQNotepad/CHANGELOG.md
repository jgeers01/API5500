# Changelog

## v0.2.0 — 22 juli 2026
- Frequenties zijn nu discrete standen (matchend met de gedetenteerde
  schakelaars op de hardware) i.p.v. vrij instelbare knoppen:
  - LO: 30, 40, 50, 100, 200, 300, 400 Hz
  - LO-MID: 75, 150, 180, 240, 500, 700, 1 kHz
  - HI-MID: 800, 1.5k, 3k, 5k, 8k, 10k, 12.5 kHz
  - HI: 2.5k, 5k, 7k, 10k, 12.5k, 15k, 20 kHz
- Gain loopt nu in stappen van 2 dB, van -12 tot +12 dB.
- GitHub Actions-workflow toegevoegd (`.github/workflows/build.yml`) die
  automatisch een kant-en-klare VST3 + AU compileert op macOS, zonder dat
  je zelf Xcode of CMake hoeft te gebruiken.

## v0.1.0 — 22 juli 2026
- Eerste versie.
- 4 banden (LO, LO-MID, HI-MID, HI) met Freq + Gain per band.
- Shelf-toggle op LO en HI.
- Range-schakelaar met x0.25 / x0.5 / x1.
- Opslag van alle instellingen via de DAW-sessie (APVTS state).
- Geen audioverwerking (pure pass-through).
