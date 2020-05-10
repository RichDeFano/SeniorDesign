
#ifndef Music_h
#define Music_h

#include <string.h>

class Music {
    private:
        char track[60], artist[60], length[10], position[10];
        int progressBar;
        bool playing;
    public: 
        Music(){
            playing = false;
            progressBar = 0;

        };
        ~Music(){};
        char* getLength(){return length;}
        char* getPosition(){return position;}
        char* getArtist(){return artist;}
        char* getTrack(){return track;}

        bool getPlaying(){return playing;}
        int getProgressBar(){return progressBar;}

        void setLength(char * l){
            strlcpy(length,l,strlen(l)+1);
        }
        void setPosition(char *p){
           strlcpy(position,p,strlen(p)+1);
        }
        void setArtist(char *a){
            strlcpy(artist,a,strlen(a)+1);
        }
        void setTrack(char *t){
            strlcpy(track,t,strlen(t)+1);
        }


        void setPlaying(bool p){playing = p;}
        void setProgressBar(int p){progressBar = p;}
        
};

#endif