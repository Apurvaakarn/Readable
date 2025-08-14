#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace lite2d {

struct AnimationClip {
	std::string name;
	int frameWidth{0};
	int frameHeight{0};
	int numFrames{1};
	int startFrame{0};
	float frameDurationSec{0.1f};
	bool loop{true};
};

class AnimationPlayer {
public:
	void addClip(const AnimationClip& clip);
	void play(const std::string& clipName, bool restartIfPlaying = false);
	void stop();
	void update(float dt);

	int getCurrentFrameIndex() const { return currentFrame; }
	const AnimationClip* getCurrentClip() const { return currentClip; }
	bool isPlaying() const { return currentClip != nullptr; }

private:
	std::unordered_map<std::string, AnimationClip> clips;
	const AnimationClip* currentClip{nullptr};
	float timeAccum{0.0f};
	int currentFrame{0};
};

} // namespace lite2d