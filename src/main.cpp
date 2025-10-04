#include <iostream>
#include "Body.h"
#include "Simulation.h"
#include "Renderer.h"

int main() {
    std::cout << "Starting Simulation!" << "\n";
    Simulation sim(1);
    sim.addBody(Body(1e3, Eigen::Vector3d(0.80,-0.5,0), Eigen::Vector3d(0,0,0)));
    sim.addBody(Body(1e3, Eigen::Vector3d(-0.80,0.5,0), Eigen::Vector3d(0,0,0)));
    sim.addBody(Body(1, Eigen::Vector3d(0,0,0), Eigen::Vector3d(-0.0001,0.0001,0)));

    Renderer renderer(1280, 720, "PhysicsSim");

    while (!renderer.shouldClose()) {
        sim.step();

       // for(size_t i = 0; i < sim.bodies.size(); i++){
       //     std::cout << "B" << i <<":\n"
       //               << sim.bodies[i].position.x() << ","
       //               << sim.bodies[i].position.y() << ","
       //               << sim.bodies[i].position.z() << "\n";
       // }
        renderer.render(sim.bodies); // Draw current state
        renderer.swapBuffers();      // Show newly drawn frame
        renderer.pollEvents();       // Handle user input or window events
    }

    return 0;
}