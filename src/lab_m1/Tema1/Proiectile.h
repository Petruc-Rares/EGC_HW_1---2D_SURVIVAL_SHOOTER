#include "GameElement.h"

class Proiectile : public GameElement {
	private:
		float radius;
        double thetaCursor;
        double speed;
        float distance;

    public:
        // Setter
        void setRadius(float radius);
        void setThetaCursor(double thetaCursor);
        void setSpeed(double speed);
        void setDistance(double distance);

        // Getter
        float getRadius();
        double getThetaCursor();
        double getSpeed();
        double getDistance();

};