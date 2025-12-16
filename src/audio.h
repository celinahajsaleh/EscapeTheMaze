#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

using namespace std;

class Audio {
private:
    map<string, Mix_Chunk*> sounds;
    Mix_Music* backgroundMusic;
    int rollingChannel;
    int movementChannel;
    bool isRolling;
    
    Mix_Chunk* generateRollingSound();
    Mix_Chunk* generateCollisionSound();
    Mix_Chunk* generateAmbientDrone();
    Mix_Chunk* generateMovementSound();
    Mix_Chunk* generateVictorySound();

public:
    Audio();
    ~Audio();
    
    void initialize();
    void cleanup();
    void generateSounds();
    
    void playSound(const string& name, int loops = 0);
    void playMusic(int loops = -1);
    void stopMusic();
    
    void setSoundVolume(int volume);
    void setMusicVolume(int volume);
    
    void startRolling();
    void stopRolling();
    void updateRollingVolume(float speed);
    
    void startMovement();
    void stopMovement();
    void updateMovementVolume(float speed);
    
    void playCollision();
    void playVictory();
};

#endif
