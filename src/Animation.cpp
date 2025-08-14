#include "Engine/Animation.hpp"

namespace lite2d {

void AnimationPlayer::addClip(const AnimationClip& clip) {
	clips[clip.name] = clip;
}

void AnimationPlayer::play(const std::string& clipName, bool restartIfPlaying) {
	auto it = clips.find(clipName);
	if (it == clips.end()) return;
	if (!restartIfPlaying && currentClip && currentClip->name == clipName) return;
	currentClip = &it->second;
	timeAccum = 0.0f;
	currentFrame = 0;
}

void AnimationPlayer::stop() {
	currentClip = nullptr;
	timeAccum = 0.0f;
	currentFrame = 0;
}

void AnimationPlayer::update(float dt) {
	if (!currentClip) return;
	timeAccum += dt;
	while (timeAccum >= currentClip->frameDurationSec) {
		timeAccum -= currentClip->frameDurationSec;
		currentFrame++;
		if (currentFrame >= currentClip->numFrames) {
			if (currentClip->loop) currentFrame = 0;
			else { currentFrame = currentClip->numFrames - 1; }
		}
	}
}

} // namespace lite2d