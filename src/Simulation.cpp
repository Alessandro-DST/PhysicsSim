#include "Simulation.h"
#include <Eigen/Dense>

Simulation::Simulation(double dt) : timeStep(dt) {}

// Add a new body
void Simulation::addBody(const Body& body) {
    bodies.push_back(body);
}

//Set G value
void Simulation::setG(double input) {
    G = input;
}

// Compute forces acting on bodies
void Simulation::computeForces() {
    // Reset all accelerations
    for (auto& b : bodies) {
        b.acceleration = Eigen::Vector3d::Zero();
    }
    computeGravity();
}

void Simulation::computeGravity(){
    for (size_t i = 0; i < bodies.size(); ++i){
        for (size_t j = i; j < bodies.size(); ++j){
            Eigen::Vector3d r_ij = bodies[j].position - bodies[i].position;
            double distance = r_ij.norm() + 1e-10; // Avoid div by zero for r_ij = 0
            Eigen::Vector3d forceDir = r_ij.normalized();
            
            bodies[i].acceleration += G * bodies[j].mass / (distance * distance) * forceDir;
            bodies[j].acceleration += -G * bodies[i].mass / (distance * distance) * forceDir;
        }
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