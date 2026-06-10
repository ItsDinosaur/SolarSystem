# 🪐 Solar System Simulation

Wizualizacja i symulacja Układu Słonecznego napisana w języku C++ z wykorzystaniem biblioteki OpenGL (GLUT). Projekt został stworzony w ramach zajęć ze **Wstępu do Fizyki**.

## 🚀 O projekcie
Aplikacja symuluje ruch planet Układu Słonecznego korzystając z numerycznego rozwiązywania równań wynikających z Prawa Powszechnego Ciążenia Newtona oraz Drugiej Zasady Dynamiki. System jednostek został zoptymalizowany dla ciał niebieskich (jednostki astronomiczne, masy Słońca i czas w dniach).

## ✨ Główne funkcje
* **Prawdziwa fizyka:** Obliczenia grawitacyjne w czasie rzeczywistym dla wielu ciał.
* **Interaktywny HUD:** Pływające okienko wyświetlające na żywo statystyki (nazwa, masa, parametry orbitalne) wybranego ciała.
* **Sterowanie czasem:** Interaktywny suwak pozwala na płynną zmianę kroku czasowego (odtwarzanie przyspieszone lub zwolnione) za pomocą kursora myszy.
* **Wizualizacja orbit:** Generowanie i rysowanie krzywych orbitalnych poszczególnych planet dla lepszej czytelności trajektorii.
* **Podąrzania za planetami** Możliwość podąrzania za planetami po kliknięciu przycisków {1,2...8}. Klawisz '0' skupia kamęrę ponowinie na Słońcu.

## 🎮 Sterowanie
* **Kamera:** Możliwość obracania i przybliżania (implementowane poprzez zdarzenia myszy/klawiatury).
* **Interfejs (HUD):** Użyj lewego przycisku myszy na suwaku w lewym dolnym rogu (Time step), aby dynamicznie zmienić prędkość upływu czasu w symulacji (DT).

## 🛠️ Technologie i Zależności
* **Język:** C++
* **Grafika:** OpenGL / GLUT

## ⚙️ Kompilacja
Aby uruchomić projekt, upewnij się, że posiadasz zainstalowane biblioteki OpenGL i GLUT. Przykładowa kompilacja (g++):

```bash
g++ main.cpp body.cpp physics.cpp -o SolarSystem -lGL -lGLU -lglut
./SolarSystem
