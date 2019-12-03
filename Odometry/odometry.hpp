#include <cmath>

/**
  Class that makes use and implementation of odometry shorter and simpler

  Takes in distance from turning circumference - wheel to center of rotation and enables use of `get_pose` functions to calculate and retrieve current x, y, theta pose of robot relative to starting position

  @param l Distance from turning circumference - wheel to center of rotation
  */
class Odometry{
    protected:
        float delta_x=0, delta_y=0, delta_theta=0;
        float x=0, y=0, theta=0, L=0;
        float delta_l=0, delta_r=0;
        float previous_l=0, previous_r=0, previous_theta=0;

        float const PI = 3.1415926535897323846264338297;

        float calculate_delta_x(float sl, float sr){
            float output = (sr+sl)/2;
            output *= std::cos(theta + (sr-sl)/(4*L));

            delta_x = output;
            return(output);
        }

        float calculate_delta_y(float sl, float sr){
            float output = (sr+sl)/2;
            output *= std::sin(theta + (sr-sl)/(4*L));

            delta_y = output;
            return(output);
        }
        float calculate_delta_theta(float sl, float sr){
            float output = (sr-sl)/(2*L);
            delta_theta = output;
            return(output);
        }

    public:
        Odometry(float l){
            L = l/2;
        }


        /**
          Update robot x, y and theta pose

          If no updates want to be made, use `get_x()`, `get_y()` and `get_theta()` functions instead to retrieve pose

          @param sl Left side displacement in ft, in, m, cm, etc. SINCE LAST ITERATION
          @param sr Right side displacement in ft, in, m, cm, etc. SINCE LAST ITERATION
          @return x, y and theta pose relative to 0,0,0 (starting point and initial heading)
          */
        void calculate_pose(float sl, float sr){
            delta_l = sl - previous_l;
            delta_r = sr - previous_r;
            previous_l = sl; previous_r = sr;

            float output[3] = {
                calculate_delta_x(delta_l, delta_r),
                calculate_delta_y(delta_l, delta_r),
                calculate_delta_theta(delta_l, delta_r)
            };

            x += output[0];
            y += output[1];
            theta += output[2]/2;
            // fmod(theta, 2*PI);
            // std::cout<<theta<<std::endl;
        }

        // Getters and setters
        void set_x(float _x){ x = _x; }
        float get_x(){ return(x); }

        void set_y(float _y){ y = _y; }
        float get_y(){ return(y); }

        void set_theta(float _theta, bool degrees = false){
            theta = degrees ? (PI*180.0)*_theta : _theta;
        }
        float get_theta(bool degrees = false){
            return(degrees ? (180.0/PI)*theta : theta);
        }
};


/**
  Class that makes use and implementation of odometry FOR MECANUM DRIVES shorter and simpler

  Takes in drive width (from wheel to wheel), length (from wheel to wheel), and wheel radius. Enables use of `get_pose` functions to calculate and retrieve current x, y, theta pose of robot relative to starting position

  @param w Distance from turning circumference - wheel to center of rotation
  @param l Distance from turning circumference - wheel to center of rotation
  @param r Distance from turning circumference - wheel to center of rotation
  */
class MecanumOdometry{
    protected:
        float lx, ly, lr, radius;
        float prev_s[4];
        float current_t, dt=0, prev_t;
        float dx_r=0, dy_r=0;
        float delta_x=0, delta_y=0, delta_theta=0;
        float x=0, y=0, theta=0;

        const float PI = 3.1415926535897323846264338297;

    public:
        MecanumOdometry(float width, float length, float r){
            lx = width / 2.0;
            ly = length / 2.0;
            lr = pow( pow(lx,2) + pow(ly,2) , 0.5);
            prev_t = pros::millis();
            radius = r;
        }


        /**
          Update robot x, y and theta pose

          If no updates want to be made, use `get_x()`, `get_y()` and `get_theta()` functions instead to retrieve pose

          @param sl Left side displacement in ft, in, m, cm, etc. SINCE LAST ITERATION
          @param sr Right side displacement in ft, in, m, cm, etc. SINCE LAST ITERATION
          @return x, y and theta pose relative to 0,0,0 (starting point and initial heading)
          */
        void calculate_pose(float lf, float lb, float rf, float rb){
            current_t = pros::millis();
            dt = current_t - prev_t;

            float ds[] = {
                (lf - prev_s[0])*(2*PI*radius),
                (lb - prev_s[1])*(2*PI*radius),
                (rf - prev_s[2])*(2*PI*radius),
                (rb - prev_s[3])*(2*PI*radius)
            };

            prev_s[0] = lf;
            prev_s[1] = lb;
            prev_s[2] = rf;
            prev_s[3] = rb;

            // (FrontRight + FrontLeft + BackLeft + BackRight) / 4
            dy_r = (ds[2] + ds[0] + ds[1] + ds[3])/4.0;
            // (-FrontRight + FrontLeft - BackLeft + BackRight) / 4
            dx_r = (-ds[2] + ds[0] - ds[1] + ds[3])/4.0;
            // (FrontRight - FrontLeft - BackLeft + BackRight) / 4 * lr
            delta_theta = (ds[2] - ds[0] - ds[1] + ds[3])/(4*lr);

            delta_x = dx_r * cos(get_theta()) - dy_r * sin(get_theta());
            delta_y = dx_r * sin(get_theta()) + dy_r * cos(get_theta());

            x += delta_x*dt;
            y += delta_y*dt;
            theta = delta_theta*dt;

            prev_t = current_t;
        }

        // Getters and setters
        void set_x(float _x){ x = _x; }
        float get_x(){ return(x); }

        void set_y(float _y){ y = _y; }
        float get_y(){ return(y); }

        void set_theta(float _theta, bool degrees = false){
            theta = degrees ? (PI*180.0)*_theta : _theta;
        }
        float get_theta(bool degrees = false){
            return(degrees ? (180.0/PI)*theta : theta);
        }
};
