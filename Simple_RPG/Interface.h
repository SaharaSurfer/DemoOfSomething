#pragma once
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <iterator>
#include "json.hpp"

using json = nlohmann::json;

class Interface
{
private:
	const int ASCII_ART_WIDTH = 50, FRAME_WIDTH = 60;
	int ASCII_ART_HEIGHT = 0, FRAME_HEIGHT = 0;

	std::vector<std::string> ascii_art;
	std::vector<std::string> framed_art;

	void CreateFrame();

public:
	void LoadLocation(const std::string& LocationName);
	void RenderLocation();

	std::string LoadText(const std::string& LocationName);
	void RenderText(const std::string& Text);

	json LoadJSON(const std::string& LocationName);

	size_t CollectPlayerChoice(size_t upper_border);
};