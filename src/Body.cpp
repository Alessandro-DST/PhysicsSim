#include "Body.h"

// Default Constructor
Body::Body(double m, const Eigen::Vector3d& pos, const Eigen::Vector3d& vel)
    : mass(m), position(pos), velocity(vel), acceleration(Eigen::Vector3d::Zero()) {};

// Overloaded Constructor to set initial Acceleration
Body::Body(
    double m,
    const Eigen::Vector3d& pos,
    const Eigen::Vector3d& vel,
    const Eigen::Vector3d& acc
)
    : mass(m), position(pos), velocity(vel), acceleration(acc) {};
