#include <last-outpost/audio.h>
#include <iostream>
#include <SDL_mixer.h>

namespace Game
{
	Audio::Audio()
		: initialized(false),
		  frequency(44100),
		  format(MIX_DEFAULT_FORMAT),
		  channels(2),
		  chunkSize(2048),
		  masterVolume(1.0f),
		  soundVolume(1.0f),
		  musicVolume(1.0f),
		  soundEnabled(true),
		  musicEnabled(true),
		  currentMusic(nullptr),
		  currentMusicType(MusicType::Unknown)
	{
	}

	Audio::~Audio()
	{
		cleanup();
	}

	bool Audio::initialize(int frequency, Uint16 format, int channels, int chunkSize)
	{
		if (initialized)
		{
			return true;
		}

		this->frequency = frequency;
		this->format = format;
		this->channels = channels;
		this->chunkSize = chunkSize;

		if (Mix_OpenAudio(frequency, format, channels, chunkSize) < 0)
		{
			return false;
		}

		initialized = true;
		updateVolumes();

		return true;
	}

	void Audio::cleanup()
	{
		if (!initialized)
			return;

		stopMusic();
		stopAllSounds();

		unloadAllSounds();
		unloadAllMusic();

		Mix_CloseAudio();
		initialized = false;
	}

	bool Audio::loadSound(SoundType type, const std::string &filepath)
	{
		if (!initialized)
		{
			return false;
		}

		unloadSound(type);

		Mix_Chunk *chunk = Mix_LoadWAV(filepath.c_str());
		if (chunk == nullptr)
		{
			return false;
		}

		sounds[type] = chunk;
		return true;
	}

	bool Audio::loadMusic(MusicType type, const std::string &filepath)
	{
		if (!initialized)
		{
			return false;
		}

		unloadMusic(type);

		Mix_Music *musicTrack = Mix_LoadMUS(filepath.c_str());
		if (musicTrack == nullptr)
		{
			return false;
		}

		music[type] = musicTrack;
		return true;
	}

	void Audio::unloadSound(SoundType type)
	{
		auto it = sounds.find(type);
		if (it != sounds.end())
		{
			Mix_FreeChunk(it->second);
			sounds.erase(it);
		}
	}

	void Audio::unloadMusic(MusicType type)
	{
		auto it = music.find(type);
		if (it != music.end())
		{
			if (currentMusic == it->second)
			{
				stopMusic();
			}
			Mix_FreeMusic(it->second);
			music.erase(it);
		}
	}

	void Audio::unloadAllSounds()
	{
		for (auto &pair : sounds)
		{
			Mix_FreeChunk(pair.second);
		}
		sounds.clear();
	}

	void Audio::unloadAllMusic()
	{
		stopMusic();
		for (auto &pair : music)
		{
			Mix_FreeMusic(pair.second);
		}
		music.clear();
	}

	void Audio::playSound(SoundType type, int channel)
	{
		if (!soundEnabled || !initialized)
			return;

		auto it = sounds.find(type);
		if (it != sounds.end())
		{
			Mix_PlayChannel(channel, it->second, 0);
		}
	}

	void Audio::playSoundWithVolume(SoundType type, float volume, int channel)
	{
		if (!soundEnabled || !initialized)
			return;

		auto it = sounds.find(type);
		if (it != sounds.end())
		{
			int sdlVolume = volumeToSDL(volume * soundVolume * masterVolume);
			Mix_VolumeChunk(it->second, sdlVolume);
			Mix_PlayChannel(channel, it->second, 0);
		}
	}

	void Audio::stopSound(int channel)
	{
		Mix_HaltChannel(channel);
	}

	void Audio::stopAllSounds()
	{
		Mix_HaltChannel(-1);
	}

	void Audio::playMusic(MusicType type, int loops)
	{
		if (!musicEnabled || !initialized)
			return;

		auto it = music.find(type);
		if (it != music.end())
		{
			if (currentMusic != nullptr && Mix_PlayingMusic())
			{
				stopMusic();
			}

			currentMusic = it->second;
			currentMusicType = type;
			Mix_PlayMusic(currentMusic, loops);
		}
	}

	void Audio::pauseMusic()
	{
		if (Mix_PlayingMusic() && !Mix_PausedMusic())
		{
			Mix_PauseMusic();
		}
	}

	void Audio::resumeMusic()
	{
		if (Mix_PausedMusic())
		{
			Mix_ResumeMusic();
		}
	}

	void Audio::stopMusic()
	{
		Mix_HaltMusic();
		currentMusic = nullptr;
		currentMusicType = MusicType::Unknown;
	}

	void Audio::fadeInMusic(MusicType type, int loops, int fadeTimeMs)
	{
		if (!musicEnabled || !initialized)
			return;

		auto it = music.find(type);
		if (it != music.end())
		{
			if (currentMusic != nullptr && Mix_PlayingMusic())
			{
				stopMusic();
			}

			currentMusic = it->second;
			currentMusicType = type;
			Mix_FadeInMusic(currentMusic, loops, fadeTimeMs);
		}
	}

	void Audio::fadeOutMusic(int fadeTimeMs)
	{
		if (Mix_PlayingMusic())
		{
			Mix_FadeOutMusic(fadeTimeMs);
			currentMusic = nullptr;
			currentMusicType = MusicType::Unknown;
		}
	}

	void Audio::setMasterVolume(float volume)
	{
		masterVolume = std::max(0.0f, std::min(1.0f, volume));
		updateVolumes();
	}

	void Audio::setSoundVolume(float volume)
	{
		soundVolume = std::max(0.0f, std::min(1.0f, volume));
		updateVolumes();
	}

	void Audio::setMusicVolume(float volume)
	{
		musicVolume = std::max(0.0f, std::min(1.0f, volume));
		updateVolumes();
	}

	void Audio::setSoundEnabled(bool enabled)
	{
		soundEnabled = enabled;
		if (!enabled)
		{
			stopAllSounds();
		}
	}

	void Audio::setMusicEnabled(bool enabled)
	{
		musicEnabled = enabled;
		if (!enabled)
		{
			stopMusic();
		}
		else if (enabled && currentMusic != nullptr)
		{
			updateVolumes();
		}
	}

	bool Audio::isMusicPlaying() const
	{
		return Mix_PlayingMusic() != 0;
	}

	bool Audio::isMusicPaused() const
	{
		return Mix_PausedMusic() != 0;
	}

	int Audio::reserveChannels(int numChannels)
	{
		return Mix_ReserveChannels(numChannels);
	}

	int Audio::allocateChannels(int numChannels)
	{
		return Mix_AllocateChannels(numChannels);
	}

	void Audio::loadGameSounds()
	{

		if (!loadSound(SoundType::ProjectilHit, "assets/audios/Projectile1.mp3"))
		{
			std::cerr << "Warning: Failed to load Projectile1.mp3 sound effect" << std::endl;
		}
		if (!loadSound(SoundType::TowerFire, "assets/audios/Projectile1.mp3"))
		{
			std::cerr << "Warning: Failed to load tower fire sound effect" << std::endl;
		}

		if (!loadSound(SoundType::EnemyDeath, "assets/audios/Projectile1.mp3"))
		{
			std::cerr << "Warning: Failed to load enemy death sound effect" << std::endl;
		}

		if (!loadSound(SoundType::EnemySpawn, "assets/audios/Projectile1.mp3"))
		{
			std::cerr << "Warning: Failed to load enemy spawn sound effect" << std::endl;
		}

		if (!loadSound(SoundType::TowerPlace, "assets/audios/Projectile1.mp3"))
		{
			std::cerr << "Warning: Failed to load tower place sound effect" << std::endl;
		}
	}

	void Audio::loadGameMusic()
	{
		if (!loadMusic(MusicType::MainMenu, "assets/audios/GameSong1.mp3"))
		{
			std::cerr << "Warning: Failed to load main menu music (GameSong1.mp3)" << std::endl;
		}

		if (!loadMusic(MusicType::GamePlay, "assets/audios/GameSong2.mp3"))
		{
			std::cerr << "Warning: Failed to load gameplay music (GameSong2.mp3)" << std::endl;
		}

		if (!loadMusic(MusicType::Victory, "assets/audios/GameSong3.mp3"))
		{
			std::cerr << "Warning: Failed to load victory music (GameSong3.mp3)" << std::endl;
		}

		if (!loadMusic(MusicType::Defeat, "assets/audios/GameSong4.mp3"))
		{
			std::cerr << "Warning: Failed to load defeat music (GameSong4.mp3)" << std::endl;
		}
	}

	void Audio::updateVolumes()
	{
		if (!initialized)
			return;

		int musicVol = volumeToSDL(musicVolume * masterVolume);
		Mix_VolumeMusic(musicVol);

		int soundVol = volumeToSDL(soundVolume * masterVolume);
		Mix_Volume(-1, soundVol);
	}

	int Audio::volumeToSDL(float volume) const
	{
		return static_cast<int>(volume * MIX_MAX_VOLUME);
	}
}