#ifndef glub_h
#define glub_h


int glub_unproject(double winx, double winy, double winz,
                   const double modelMatrix[16],
                   const double projMatrix[16],
                   const int viewport[4],
                   double *objx, double *objy, double *objz);

#endif
