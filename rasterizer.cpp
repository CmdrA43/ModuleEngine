bool boundCheck(obj camera, vector3 point){
  vector3 dir = camera.rotation.normalize();
  double leftBound = arctan(camera.rotation.y/camera.rotation.x) + 0.785398163;
  double rightBound = arctan(camera.rotation.y/camera.rotation.x) - 0.785398163;
  double upBound = (camera.rotation.y + camera.rotation.x) * 0.588002604;
  double downBound = (camera.rotation.y + camera.rotation.x) * (0-0.588002604);
}
