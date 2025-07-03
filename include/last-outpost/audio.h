#ifndef __LAST_OUTPOST_AUDIO_H__
#define __LAST_OUTPOST_AUDIO_H__

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <memory>

namespace Game
{
	enum class SoundType
	{
		TowerFire,
		ProjectilHit,
		EnemyDeath,
		EnemySpawn,
		TowerPlace,
		GameOver,
		LevelComplete,
		ButtonClick,
		Unknown
	};

	enum class MusicType
	{
		MainMenu,
		GamePlay,
		Victory,
		Defeat,
		Unknown
	};

	class Audio
	{
	private:
		bool initialized;
		int frequency;
		Uint16 format;
		int channels;
		int chunkSize;

		float masterVolume;
		float soundVolume;
		float musicVolume;
		bool soundEnabled;
		bool musicEnabled;

		std::unordered_map<SoundType, Mix_Chunk *> sounds;
		std::unordered_map<MusicType, Mix_Music *> music;

		Mix_Music *currentMusic;
		MusicType currentMusicType;

	public:
		Audio();
		~Audio();

		bool initialize(int frequency = 44100, Uint16 format = MIX_DEFAULT_FORMAT,
						int channels = 2, int chunkSize = 2048);
		void cleanup();

		bool loadSound(SoundType type, const std::string &filepath);
		bool loadMusic(MusicType type, const std::string &filepath);
		void unloadSound(SoundType type);
		void unloadMusic(MusicType type);
		void unloadAllSounds();
		void unloadAllMusic();

		void playSound(SoundType type, int channel = -1);
		void playSoundWithVolume(SoundType type, float volume, int channel = -1);
		void stopSound(int channel);
		void stopAllSounds();

		void playMusic(MusicType type, int loops = -1);
		void pauseMusic();
		void resumeMusic();
		void stopMusic();
		void fadeInMusic(MusicType type, int loops, int fadeTimeMs);
		void fadeOutMusic(int fadeTimeMs);

		void setMasterVolume(float volume);
		void setSoundVolume(float volume);
		void setMusicVolume(float volume);
		float getMasterVolume() const { return masterVolume; }
		float getSoundVolume() const { return soundVolume; }
		float getMusicVolume() const { return musicVolume; }

		void setSoundEnabled(bool enabled);
		void setMusicEnabled(bool enabled);
		bool isSoundEnabled() const { return soundEnabled; }
		bool isMusicEnabled() const { return musicEnabled; }

		bool isInitialized() const { return initialized; }
		bool isMusicPlaying() const;
		bool isMusicPaused() const;
		MusicType getCurrentMusicType() const { return currentMusicType; }

		int reserveChannels(int numChannels);
		int allocateChannels(int numChannels);

		void loadGameSounds();
		void loadGameMusic();

	private:
		void updateVolumes();
		int volumeToSDL(float volume) const;
	};
}

#endif