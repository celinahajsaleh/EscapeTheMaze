#include "audio.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Audio::Audio() : backgroundMusic(nullptr), rollingChannel(-1), movementChannel(-1), isRolling(false) {
}

Audio::~Audio() {
    cleanup();
}

void Audio::initialize() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cerr << "SDL Audio initialization failed: " << SDL_GetError() << endl;
        return;
    }
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer initialization failed: " << Mix_GetError() << endl;
        return;
    }
    
    Mix_AllocateChannels(16);
    cout << "Audio system initialized" << endl;
}

void Audio::cleanup() {
    Mix_HaltChannel(-1);
    Mix_HaltMusic();
    
    for (auto& pair : sounds) {
        if (pair.second) {
            Mix_FreeChunk(pair.second);
        }
    }
    sounds.clear();
    
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
    
    Mix_CloseAudio();
    Mix_Quit();
}

// metal ball rolling
Mix_Chunk* Audio::generateRollingSound() {
    const int sampleRate = 44100;
    const float duration = 0.6f;   
    const int samples = static_cast<int>(sampleRate * duration);
    
    Sint16* buffer = new Sint16[samples];
    
    // Generate metal rolling sound 
    for (int i = 0; i < samples; i++) {
        float t = static_cast<float>(i) / sampleRate;
        
        float sample = 0.0f;
        
        // Metallic resonance (higher, clearer tones)
        sample += 0.4f * sin(2.0f * M_PI * 240.0f * t);   // Clear metallic
        sample += 0.35f * sin(2.0f * M_PI * 180.0f * t);  // Mid metallic
        sample += 0.3f * sin(2.0f * M_PI * 360.0f * t);   // High metallic
        
        // Stone contact (low grinding)
        sample += 0.25f * sin(2.0f * M_PI * 110.0f * t);  // Stone grind
        sample += 0.2f * sin(2.0f * M_PI * 85.0f * t);    // Deep friction
        
        // Rhythmic rolling pattern
        float rhythm = 0.85f + 0.15f * sin(2.0f * M_PI * 4.0f * t);
        
        // Textured noise for surface contact
        float noise = (static_cast<float>(rand()) / RAND_MAX - 0.5f);
        sample += 0.35f * noise * rhythm;
        
        sample *= rhythm;
        
        buffer[i] = static_cast<Sint16>(sample * 15000.0f);  // Moderate volume
    }
    
    Mix_Chunk* chunk = Mix_QuickLoad_RAW(reinterpret_cast<Uint8*>(buffer), samples * sizeof(Sint16));
    return chunk;
}

// metal ball rolling on stone - active sound
Mix_Chunk* Audio::generateMovementSound() {
    const int sampleRate = 44100;
    const float duration = 0.35f;  // Shorter, smoother loop
    const int samples = static_cast<int>(sampleRate * duration);
    
    Sint16* buffer = new Sint16[samples];
    
    // Generate smooth metal ball rolling
    for (int i = 0; i < samples; i++) {
        float t = static_cast<float>(i) / sampleRate;
        
        float sample = 0.0f;
        
        // weight and continuous motion
        sample += 0.6f * sin(2.0f * M_PI * 110.0f * t);   // Main rumble
        sample += 0.5f * sin(2.0f * M_PI * 165.0f * t);   // Mid rumble
        sample += 0.4f * sin(2.0f * M_PI * 85.0f * t);    // Deep bass
        
        // Soft metallic texture 
        sample += 0.3f * sin(2.0f * M_PI * 220.0f * t);   // Soft metallic
        sample += 0.25f * sin(2.0f * M_PI * 330.0f * t);  // Light ring
        
        // Smooth rolling pattern
        float smooth = 0.8f + 0.2f * sin(2.0f * M_PI * 4.0f * t);
        
        // Gentle surface noise
        float noise = (static_cast<float>(rand()) / RAND_MAX - 0.5f);
        sample += 0.25f * noise * smooth;
        
        sample *= smooth;
        
        buffer[i] = static_cast<Sint16>(sample * 14000.0f);  // Moderate volume
    }
    
    Mix_Chunk* chunk = Mix_QuickLoad_RAW(reinterpret_cast<Uint8*>(buffer), samples * sizeof(Sint16));
    return chunk;
}

//collision sound 
Mix_Chunk* Audio::generateCollisionSound() {
    const int sampleRate = 44100;
    const float duration = 0.15f;  
    const int samples = static_cast<int>(sampleRate * duration);
    
    Sint16* buffer = new Sint16[samples];
    
    for (int i = 0; i < samples; i++) {
        float t = static_cast<float>(i) / sampleRate;
        
        // Fast decay for dry stone impact
        float decay = exp(-25.0f * t);
        float sample = 0.0f;
        
        // Heavy stone thud 
        sample += 0.9f * sin(2.0f * M_PI * 150.0f * t);   // Deep stone impact
        sample += 0.8f * sin(2.0f * M_PI * 95.0f * t);    // Very low thud
        sample += 0.7f * sin(2.0f * M_PI * 220.0f * t);   // Stone resonance
        
        // Some metallic component (ball hitting stone)
        sample += 0.4f * sin(2.0f * M_PI * 450.0f * t);   // Light metal
        sample += 0.3f * sin(2.0f * M_PI * 680.0f * t);   // Subtle ring
        
        // Heavy impact noise (stone crunch)
        float noiseIntensity = (t < 0.02f) ? 0.8f : (t < 0.05f ? 0.3f : 0.05f);
        sample += noiseIntensity * (static_cast<float>(rand()) / RAND_MAX - 0.5f);
        
        sample *= decay;
        
        buffer[i] = static_cast<Sint16>(sample * 18000.0f);
    }
    
    Mix_Chunk* chunk = Mix_QuickLoad_RAW(reinterpret_cast<Uint8*>(buffer), samples * sizeof(Sint16));
    return chunk;
}

// ambient drone 
Mix_Chunk* Audio::generateAmbientDrone() {
    const int sampleRate = 44100;
    const float duration = 4.0f;  // 4 second loop
    const int samples = static_cast<int>(sampleRate * duration);
    
    Sint16* buffer = new Sint16[samples];
    
    for (int i = 0; i < samples; i++) {
        float t = static_cast<float>(i) / sampleRate;
        
        // Dark ambient drone
        float sample = 0.0f;
        sample += 0.3f * sin(2.0f * M_PI * 40.0f * t);    // Very low drone
        sample += 0.2f * sin(2.0f * M_PI * 60.0f * t);    // Low harmony
        sample += 0.15f * sin(2.0f * M_PI * 90.0f * t);   // Subtle upper
        sample += 0.1f * sin(2.0f * M_PI * 30.0f * t);    // Sub bass
        
        // subtle noise for atmosphere
        sample += 0.05f * (static_cast<float>(rand()) / RAND_MAX - 0.5f);
        
        // Gentle fade in/out for smooth looping
        float envelope = 1.0f;
        if (t < 0.5f) envelope = t / 0.5f;
        if (t > duration - 0.5f) envelope = (duration - t) / 0.5f;
        
        sample *= envelope;
        
        buffer[i] = static_cast<Sint16>(sample * 6000.0f);  // Quieter ambient
    }
    
    Mix_Chunk* chunk = Mix_QuickLoad_RAW(reinterpret_cast<Uint8*>(buffer), samples * sizeof(Sint16));
    return chunk;
}

//victory sound
Mix_Chunk* Audio::generateVictorySound() {
    const int sampleRate = 44100;
    const float duration = 1.2f;  // Longer lasting
    const int samples = static_cast<int>(sampleRate * duration);
    
    Sint16* buffer = new Sint16[samples];
    

    float freq1 = 1046.5f;  // C6 (high C)
    float freq2 = 1567.98f; // G6 (perfect fifth above)
    
    for (int i = 0; i < samples; i++) {
        float t = static_cast<float>(i) / sampleRate;
        
        // Two frequencies for richer sound
        float sample = sin(2.0f * M_PI * freq1 * t) * 0.7f;
        sample += sin(2.0f * M_PI * freq2 * t) * 0.3f;
        
        // Slower decay for longer sound
        float envelope = exp(-3.5f * t);
        sample *= envelope;
        
        buffer[i] = static_cast<Sint16>(sample * 20000.0f);
    }
    
    Mix_Chunk* chunk = Mix_QuickLoad_RAW(reinterpret_cast<Uint8*>(buffer), samples * sizeof(Sint16));
    return chunk;
}

void Audio::generateSounds() {
    cout << "Generating procedural sounds..." << endl;
    
    // Generate procedural sounds
    sounds["rolling"] = generateRollingSound();
    sounds["collision"] = generateCollisionSound();
    sounds["ambient"] = generateAmbientDrone();
    sounds["victory"] = generateVictorySound();
    
    // Try to load movement sound from file
    Mix_Chunk* movementFile = Mix_LoadWAV("../assets/sounds/movement.wav");
    if (movementFile) {
        cout << "Loaded movement sound from file" << endl;
        sounds["movement"] = movementFile;
    } else {
        cout << "Could not load movement.wav, generating procedural sound: " << Mix_GetError() << endl;
        sounds["movement"] = generateMovementSound();  // Fallback to procedural
    }
    
    cout << "Sounds loaded successfully" << endl;
}

void Audio::playSound(const string& name, int loops) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        Mix_PlayChannel(-1, it->second, loops);
    }
}

void Audio::playMusic(int loops) {
    // Play ambient as looping sound instead of music
    auto it = sounds.find("ambient");
    if (it != sounds.end()) {
        Mix_PlayChannel(-1, it->second, loops);
    }
}

void Audio::stopMusic() {
    Mix_HaltMusic();
}

void Audio::setSoundVolume(int volume) {
    Mix_Volume(-1, volume);
}

void Audio::setMusicVolume(int volume) {
    Mix_VolumeMusic(volume);
}

void Audio::startRolling() {
    if (!isRolling) {
        auto it = sounds.find("rolling");
        if (it != sounds.end()) {
            rollingChannel = Mix_PlayChannel(-1, it->second, -1);  
            isRolling = true;
        }
    }
}

void Audio::stopRolling() {
    if (isRolling && rollingChannel != -1) {
        Mix_HaltChannel(rollingChannel);
        isRolling = false;
        rollingChannel = -1;
    }
}

void Audio::updateRollingVolume(float speed) {
    if (isRolling && rollingChannel != -1) {
        // Volume based on speed (0.0 to 1.0) -> (0 to 64)
        int volume = static_cast<int>(speed * 64.0f);
        volume = min(64, max(0, volume));
        Mix_Volume(rollingChannel, volume);
    }
}

void Audio::startMovement() {
    if (movementChannel == -1 && sounds.find("movement") != sounds.end()) {
        movementChannel = Mix_PlayChannel(-1, sounds["movement"], -1);  // Loop
    }
}

void Audio::stopMovement() {
    if (movementChannel != -1) {
        Mix_HaltChannel(movementChannel);
        movementChannel = -1;
    }
}

void Audio::updateMovementVolume(float speed) {
    if (movementChannel != -1) {
        // Volume based on speed
        int volume = static_cast<int>(speed * 128.0f);
        volume = min(128, max(0, volume));
        Mix_Volume(movementChannel, volume);
    }
}

void Audio::playVictory() {
    auto it = sounds.find("victory");
    if (it != sounds.end()) {
        Mix_PlayChannel(-1, it->second, 0);
    }
}
