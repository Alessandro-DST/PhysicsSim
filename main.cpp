#include <iostream>
#include "Simulation.h"

int main() {
    Simulation sim(0.01);

    Body b1(1.0, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,0,0));
    sim.addBody(b1);

    for (int i = 0; i < 100; ++i){
        sim.step();
        std::cout << "Step" << i << ": " << sim.bodies[0].position.transpose() << "\n";
    }

    return 0;
}