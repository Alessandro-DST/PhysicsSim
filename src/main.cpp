#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Body.h"
#include "Simulation.h"

// Window size
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Simple render function -> render body as a point
void drawBody(const Body& b){
}


int main() {
    Simulation sim(0.01);

    Body b1(1e10, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,0,0));
    Body b2(1e10, Eigen::Vector3d(10,10,0), Eigen::Vector3d(0,0,0));
    sim.addBody(b1);
    sim.addBody(b2);

    for (int i = 0; i < 100; ++i){
        sim.step();
        std::cout << "Step" << i << ": " << "\n"<< "Body[0]: "<< sim.bodies[0].position.transpose() << "\n"<< "Body[1]: "<< sim.bodies[1].position.transpose() << "\n\n";
    }

    return 0;
}