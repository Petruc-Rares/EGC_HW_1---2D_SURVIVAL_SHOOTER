#pragma once
class GameElement
{
    protected:
        // Private attribute
        double hitboxWidth;
        double hitboxHeight;
        double oxHitbox;
        double oyHitbox;

        double crtOxHitbox;
        double crtOyHitbox;

        // after translation of pressing W, A, S, D
        double oxTranslate;
        double oyTranslate;

        // translation of current element from center
        double ox;
        double oy;

    public:
        // Setter
        void setHitboxWidth(double width);
        void setHitboxHeight(double height);
        void setOx(double Ox);
        void setOy(double Oy);
        void setCrtOxHitbox(double Ox);
        void setCrtOyHitbox(double Oy);
        void setOxHitbox(double Ox);
        void setOyHitbox(double Oy);
        void setOxTranslate(double Ox);
        void setOyTranslate(double Oy);


        // Getter
        double getHitboxWidth();
        double getHitboxHeight();
        double getOx();
        double getOy();
        double getOxHitbox();
        double getOyHitbox();
        double getOxTranslate();
        double getOyTranslate();
        double getCrtOxHitbox();
        double getCrtOyHitbox();

};

