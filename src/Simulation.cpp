#include "Simulation.h"
#include <Eigen/Dense>

Simulation::Simulation(double dt) : timeStep(dt) {}

// Add a new body
void Simulation::addBody(const Body& body) {
    bodies.push_back(body);
}

// Compute forces acting on bodies
void Simulation::computeForces() {
    // for now empty
    for (auto& b : bodies) {
        b.acceleration = Eigen::Vector3d::Zero();
    }
}

// Perform a Verlet integration step for Newtonian Forces
void Simulation::step() {
    computeForces(); // Update accelerations

    Eigen::Vector3d old_acc;
    for (auto& b : bodies){
        b.position += timeStep * b.velocity + 0.5 * timeStep * timeStep * b.acceleration;
        old_acc = std::move(b.acceleration);
        computeForces();
        b.velocity += 0.5 * timeStep * (b.acceleration + old_acc);
    }
}