/*
#The Facade pattern is a structural design pattern that provides a unified, simplified interface to
 a set of interfaces in a subsystem. It hides the complexity of the underlying system, making it
 easier for clients to interact with it without knowing the details. This is useful when dealing
 with legacy code, third-party libraries, or complex APIs.

 #Key characteristics:
-- Acts as a "front door" to a subsystem.
-- Reduces dependencies between clients and subsystem classes.
-- Promotes loose coupling.

#Small C++ Example: Facade for a Home Theater System
--Here, the facade simplifies controlling multiple components (e.g., TV, DVD player, lights).

*/

#include <iostream>

// Subsystem classes (complex internals)
class TV {
public:
    void on() { std::cout << "TV is on\n"; }
    void off() { std::cout << "TV is off\n"; }
};

class DVDPlayer {
public:
    void play() { std::cout << "DVD is playing\n"; }
    void stop() { std::cout << "DVD stopped\n"; }
};

class Lights {
public:
    void dim() { std::cout << "Lights dimmed\n"; }
    void brighten() { std::cout << "Lights brightened\n"; }
};

// Facade: Simplified interface
class HomeTheaterFacade {
private:
    TV tv;
    DVDPlayer dvd;
    Lights lights;

public:
    void watchMovie() {
        lights.dim();
        tv.on();
        dvd.play();
    }

    void endMovie() {
        dvd.stop();
        tv.off();
        lights.brighten();
    }
};

int main() {
    HomeTheaterFacade theater;
    theater.watchMovie();  // Client uses simple method
    theater.endMovie();
    
    return EXIT_SUCCESS;
}
