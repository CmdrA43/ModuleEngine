//camera rotation should be the sin and cos of the unit vector for x and y in radians

bool vertBoundChecks(obj camera, vector3 point){
  vector3 dir = camera.rotation.normalize();
  if((((point.y * dir.y) + (point.x * dir.x)) * 0.588002604) > point.z && (((point.y * dir.y) + (point.x * dir.x)) * -0.588002604) < point.z){
    return true;
  }
  return false;
}

bool boundsCheck(obj camera, vector3 point){
  vector3 dir = camera.rotation.normalize();
  double leftBound = tan(arctan(dir.y/dir.x) + 0.785398163);
  double rightBound = tan(arctan(dir.y/dir.x) - 0.785398163);
  if(vertBoundChecks(camera, point) && (leftBound * point.x) > point.y && (rightBound * point.x) < point.y){
    return true;
  }
  return false;
}
