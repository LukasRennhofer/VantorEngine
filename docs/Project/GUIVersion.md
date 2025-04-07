# Entwicklung einer GUI-Version des CHIFEngine Dev Environments

## Zielsetzung:

Die Aufgabe besteht darin, eine grafische Benutzeroberfläche zu erstellen, die es Nutzern ermöglicht, das interne Build-System für die CHIFEngine zu steuern. Die GUI soll mit ```customtkinter ``` entwickelt werden und ein modernes, ansprechendes Design bieten. Der Benutzer soll in der Lage sein, Bibliotheken und Beispielprojekte zu erstellen, das Build-Verzeichnis zu bereinigen und zu prüfen, ob alle erforderlichen Tools wie cmake installiert sind – alles ohne die Notwendigkeit, die Kommandozeile zu benutzen.
Überblick:

Das bestehende Build-System (**CHIFEngineInternalBuildSystem**) wurde für das Terminal entwickelt, aber es soll jetzt eine benutzerfreundliche Oberfläche geschaffen werden, die es ermöglicht, diese Aufgaben intuitiv auszuführen.

Die GUI wird das zentrale Modul **chifDev.py** ([in diesem Repo](https://github.com/LukasRennhofer/CHIFEngine/blob/main/Tools/chifDev.py)) verwenden, das die Logik des Build-Systems enthält. Die GUI wird Funktionen wie das Starten des Builds, das Anzeigen von Statusinformationen, das Bereinigen von Build-Dateien und das Überprüfen von Fehlern unterstützen.

## Funktionen der GUI

### 1. Plattform-Auswahl

        Ermöglicht dem Benutzer, das Zielbetriebssystem (Windows oder Linux) auszuwählen. Dies wird für die Anpassung des Builds auf das jeweilige Zielsystem benötigt.

### 2. Hauptfunktionen

        Build-Bibliothek: Diese Funktion startet den Build-Prozess der CHIFEngine-Bibliothek für das ausgewählte Zielsystem.

        Build Beispiel-Projekt: Der Benutzer kann ein Beispielprojekt (wie ein Framework oder eine Testumgebung) bauen.

        CMake Überprüfung: Ein Button, um zu überprüfen, ob cmake auf dem System installiert ist.

        Bereinigen des Build-Verzeichnisses: Mit dieser Funktion kann das Build-Verzeichnis gelöscht und neu erstellt werden, um sicherzustellen, dass keine alten Artefakte den neuen Build-Prozess beeinflussen.

### 3. Fehlerbehandlung und Feedback

        Ladeanzeigen: Während des Build-Prozesses sollte eine visuelle Rückmeldung (z. B. ein Ladeindikator oder Spinner) angezeigt werden.

        Erfolgs-/Fehlermeldungen: Zeigt dem Benutzer nach jedem Build-Prozess an, ob der Vorgang erfolgreich war oder nicht (z. B. durch grüne Häkchen für Erfolge und rote Kreuze für Fehler).

### 4. Modernes Design
    Was soll ich da noch dazu sagen

## Wenn fertig:
- Baue es zu einer Exe und schreibe eine README.md Datei, wo die Commands und beschreibung drinnen steht und schreibe hier auch den build command für das Bauen hinein
- Lade es auf Github auf ein Github Repository Name wäre angemessen zu wählen und schicke mir dann den Link