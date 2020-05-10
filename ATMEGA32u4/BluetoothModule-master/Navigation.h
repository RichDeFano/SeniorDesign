#ifndef Navigation_h
#define Navigation_h

#include <string.h>

class Navigation {
    private:
        char distance[10], next_distance[10], time[10], next_dir[50], next_road[75];
        bool running;
    public: 
        Navigation(){
            running = false;
        };
        ~Navigation(){};
        char* getDistance(){return distance;}
        char* getNextDistance(){return next_distance;}
        char* getNextDir(){return next_dir;}
        char* getNextRoad(){return next_road;}
        char *getTime(){return time;}
        bool getRunning(){return running;}


        void setDistance(char* d){
            strlcpy(distance,d,strlen(d)+1);
        }
        void setNextDistance(char* nd){
            strlcpy(next_distance,nd,strlen(nd)+1);
        }
        void setNextDir(char* nd){
           strlcpy(next_dir,nd,strlen(nd)+1);
        }
        void setNextRoad(char* nr){
            strlcpy(next_road,nr,strlen(nr)+1);
        }
        void setTime(char* t){
            strlcpy(time,t,strlen(t)+1);
        }
        void setRunning(bool r){running = r;}
        
};

#endif