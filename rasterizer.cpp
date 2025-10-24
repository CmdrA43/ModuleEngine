bool boundCheck(obj camera, vector3 point){
  vector3 dir = camera.rotation.normalize();
  vector3 lBound = arctan(camera.rotation.y/camera.rotation.x) + 0.785398163;
  vector3 rBound = arctan(camera.rotation.y/camera.rotation.x) - 0.785398163;
}
