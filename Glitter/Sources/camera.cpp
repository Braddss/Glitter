#include "camera.hpp"


Camera* mCam;

Camera* Camera::getCamera()
{
    return mCam;
}

void Camera::setCamera(Camera& camera)
{
    mCam = &camera;
}