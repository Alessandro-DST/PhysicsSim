#pragma once
#include <Eigen/Dense>

class Body{
    public:
        double mass;
        Eigen::Vector3d position;
        Eigen::Vector3d velocity;
        Eigen::Vector3d acceleration;

        Body(double m, const Eigen::Vector3d& pos, const Eigen::Vector3d& vel);

        // Overloaded Definition to also set Acceleration of the body
        Body(
            double m,
            const Eigen::Vector3d& pos,
            const Eigen::Vector3d& vel,
            const Eigen::Vector3d& acc
        );

        // Default Constructor
        Body() : 
                mass(1.0), 
                position(Eigen::Vector3d::Zero()),
                velocity(Eigen::Vector3d::Zero()),
                acceleration(Eigen::Vector3d::Zero())
            {};

    private:
};