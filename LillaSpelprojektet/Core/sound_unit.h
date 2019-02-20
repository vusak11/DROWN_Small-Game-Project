#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
// This class is a sound class. It is included as a member variable for each sound related variable.
// Use this by creating a SoundUnit, load a file and finally play the SoundUnit.
// Example:
//    in player class:
//		  int pos_x_;
//		  int pos_y_;
//		  SoundUnit attack_sound_;

// OBS! SFML supports the audio file formats WAV, OGG/Vorbis and FLAC.
// Due to licensing issues MP3 is not supported.
// Never exceed 256 sounds.
// Music is not copyable.


class SoundUnit
{
public:
	SoundUnit();	// Sets default sound and music file.
	~SoundUnit();

	bool LoadSound(char* file_name);
	void SetMusicFile(char* file_name);	// This simulates loading the song because the song is loaded when played later.

	void PlaySound();	// Play also act as Resume.
	void StopSound();

	bool PlayMusic();
	void StopMusic();

private:
	sf::SoundBuffer buffer_;
	sf::Sound sound_;
	sf::Music music_;
	char* music_file_name_;

};
