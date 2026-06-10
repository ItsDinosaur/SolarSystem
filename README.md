# 🪐 Solar System Simulation

Wizualizacja i symulacja Układu Słonecznego napisana w języku C++ z wykorzystaniem biblioteki OpenGL (GLUT). Projekt został stworzony w ramach zajęć ze **Wstępu do Fizyki**.

## 🚀 O projekcie
Aplikacja symuluje ruch planet Układu Słonecznego korzystając z numerycznego rozwiązywania równań wynikających z Prawa Powszechnego Ciążenia Newtona oraz Drugiej Zasady Dynamiki. System jednostek został zoptymalizowany dla ciał niebieskich (jednostki astronomiczne, masy Słońca i czas w dniach).

## ✨ Główne funkcje
* **Prawdziwa fizyka:** Obliczenia grawitacyjne w czasie rzeczywistym dla wielu ciał.
* **Interaktywny HUD:** Pływające okienko wyświetlające na żywo statystyki (nazwa, masa, parametry orbitalne) wybranego ciała.
* **Sterowanie czasem:** Interaktywny suwak pozwala na płynną zmianę kroku czasowego (odtwarzanie przyspieszone lub zwolnione) za pomocą kursora myszy.
* **Wizualizacja orbit:** Generowanie i rysowanie krzywych orbitalnych poszczególnych planet dla lepszej czytelności trajektorii.
* **Podążania za planetami** Możliwość podążania za planetami po kliknięciu przycisków {1,2...8}. Klawisz '0' skupia kamerę ponownie na Słońcu.

## 🎮 Sterowanie
* **Kamera:** Możliwość obracania i przybliżania (implementowane poprzez zdarzenia myszy/klawiatury).
* **Interfejs (HUD):** Możliwość użycia suwaku w lewym dolnym rogu (Time step), aby dynamicznie zmienić prędkość upływu czasu w symulacji (DT).

## 🛠️ Technologie i Zależności
* **Język:** C++
* **Grafika:** OpenGL / freeGLUT

## ⚙️ Kompilacja
Aby uruchomić projekt, upewnij się, że posiadasz zainstalowane biblioteki OpenGL i freeGLUT. Przykładowa kompilacja (g++):

```bash
g++ main.cpp body.cpp physics.cpp -o SolarSystem -lGL -lGLU -lglut
./SolarSystem
```
### Linux
##### 0. NixOS
Zerowy krok tylko dla użytkowników NixOS. Istnieje zdefiniowany nix-shell zawierający wszystkie potrzebne biblioteki i zależności.
```bash
nix-shell .
```
##### 1. Użycie Makefile
```bash
make            # kompilacja
./solar_system  # uruchomienie
```
