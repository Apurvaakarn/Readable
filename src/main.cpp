#include "Engine/Engine.hpp"

int main(int argc, char** argv) {
	lite2d::EngineConfig config;
	config.windowWidth = 1280;
	config.windowHeight = 720;
	config.windowTitle = "Lite2D Engine Demo";
	config.targetFps = 60;

	lite2d::Engine engine;
	if (!engine.initialize(config)) {
		return 1;
	}
	engine.run();
	engine.shutdown();
	return 0;
}