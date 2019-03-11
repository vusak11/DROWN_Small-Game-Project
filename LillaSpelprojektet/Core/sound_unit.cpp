#include "sound_unit.h"

SoundUnit::SoundUnit() {
	buffer_.loadFromFile((char*)"../Resources/Audio/default_sound.wav");
	sound_.setBuffer(buffer_);

	music_file_name_ = ((char*)"../Resources/Audio/default_sound.wav");
	music_.setVolume(100);
	music_.setLoop(true);
	music_.setRelativeToListener(true);
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

void SoundUnit::SetVolumeMusic(int volume) {
	music_.setVolume(volume);
}

//void SoundUnit::SetAttenuation(float attenuation) {
//	music_.setAttenuation(attenuation);
//}
//
//void SoundUnit::SetMinDistance(float distance) {
//	music_.setMinDistance(distance);
//}
//
//void SoundUnit::SetPosition(float x, float y, float z) {
//	music_.setPosition(x, y, z);
//}
