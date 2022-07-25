#include <lunchbox_sampler.hpp>

#undef main

int main() {
	LunchboxSampler& sampler = LunchboxSampler::getInstance();
	sampler.run();
}