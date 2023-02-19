#include "pch.h"
#include <string>

extern "C" __declspec(dllexport) std::string getButton(int amount) {
	std::string html = "<button'>" + std::to_string(amount) + " kg CO2 offset</button>";
	return html;
}
