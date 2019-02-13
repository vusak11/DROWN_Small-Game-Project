#include "sound_unit.h"

SoundUnit::SoundUnit() {
	buffer_.loadFromFile((char*)"../Resources/Audio/default_sound.wav");
	sound_.setBuffer(buffer_);

	music_file_name_ = ((char*)"../Resources/Audio/default_sound.wav");
}

SoundUnit::~SoundUnit() {
}

bool SoundUnit::LoadSound(char* file_name) {
	
	if (!buffer_.loadFromFile(file_name))
		return false;
	sound_ = sf::Sound(buffer_);
	sound_.setBuffer(buffer_);
	return true;
}

void SoundUnit::SetMusicFile(char * file_name) {
	music_file_name_ = file_name;
}

void SoundUnit::PlaySound() {
	sound_.play();
}

void SoundUnit::StopSound() {
	sound_.stop();
}

bool SoundUnit::PlayMusic() {

	if (!music_.openFromFile(music_file_name_))
		return false; // error
	music_.play();

	return true;
}

void SoundUnit::StopMusic() {
	music_.stop();
}
