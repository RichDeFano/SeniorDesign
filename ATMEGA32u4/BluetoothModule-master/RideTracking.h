#ifndef RideTracking_h
#define RideTracking_h

#include <string.h>

class RideTracking {
    private:
        char time[10], distance[10], speed[10], avg_speed[10], gain[10];
        bool running;
    public: 
        RideTracking(){
            running = true;
        };
        ~RideTracking(){};
        char* getDistance(){return distance;}
        char *getSpeed(){return speed;}
        char *getAvg(){return avg_speed;}
        char *getGain(){return gain;}
        char *getTime(){return time;}
        bool getRunning(){return running;}

        void setDistance(char* d){
            strlcpy(distance,d,strlen(d)+1);
        }
        void setSpeed(char* s){
            strlcpy(speed,s,strlen(s)+1);
        }
        void setAvg(char* a){
            strlcpy(avg_speed,a,strlen(a)+1);
        }
        void setGain(char* g){
            strlcpy(gain,g,strlen(g)+1);
        }
        void setTime(char* t){
            strlcpy(time,t,strlen(t)+1);
        }

        void setRunning(bool r){running = r;}
        
};

#endif