
# Ziele fuer MemoryClass

- Bestehende Sample Packs laden

- Einzelne Samples laden/ zu Sample Pack hinzufuegen

- Sample Pack Einstellungen uebernehmen (nur SD-Karte)

- Sample Pack Einstellungen speichern (nur SD-Karte)

- Infos zu Samples/Sample Packs abfragen (SD-Karte)

- Globale / Session Einstellungen speichern/laden (SD-Karte)

- 

# Ideen

- Sample Packs sind vorkonfiguriert ueber setting.json
- Aenderungen an Sample Packs (Mix aus verschiedenen Sample Packs) werden in den Session Settings gespeichert



# Aufgaben

1. Dateistruktur zum Laden von Sample Packs entwerfen

SD-Karte
/
|
--/samples
|	|
|	|
|	./SamplePack01
|		|
|		./settings.txt
|		./Sample01.wav
|		./Sample02.wav
|		...
|
--/sessions
	|
	|
	./session01.txt
	...

2. grober Entwurf MemoryClass API

// load Sample Pack from SD to Flash
boolean: loadSamplePack(String path)

// load additional Samples
boolean: loadSample(String path)

// replace existing Sample in Flash with new Sample
boolean: replaceSample(String oldPath, String newPath)

// triggerSample by returning wav (to Audio Class)
wav: triggerSample(String path)

3. Serial Flash und SD.h ansprechen/API angucken

## Serial Flash

SerialFlash.opendir() - iterator ueber globale Dateiliste
SerialFlash.readdir() - aktuellen Dateinamen auslesen

char buffer[256];
Flash -> text.txt (256 Byte)


