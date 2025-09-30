#pragma once
#include <vector>
#include "Body.h"

class Simulation {
public:
    double timeStep;                  // Δt
    std::vector<Body> bodies;         // All bodies in the simulation

    Simulation(double dt);

    // Add a new body to the simulation
    void addBody(const Body& body);

    // Perform a single integration step (Verlet)
    void step();

private:
    // Private helper functions for force calculation, etc.
    void computeForces(); 
};
