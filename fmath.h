#define pi (3.14159265358979323846264338327)
#define pi2 (6.283185307179586476925286766559)
#define pi_inv (0.318309886183790671537767526745)
#define pi2_inv (0.159154943091895335768883763372)

// pi/180
#define deg_to_rad(angle) \
((angle) * 0.017453292519943295769236907684886)
// 180/pi
#define rad_to_deg(angle) \
((angle) * 57.295779513082320876798154814105)

double sqrt(double x);
double fmod(double dividend, double divisor);
double atan2(double y, double x);
double cos(double angle);
double sin(double angle);
