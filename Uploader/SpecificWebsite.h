#pragma once

#include <iostream>


class SpecificWebsite
{
private:
	std::string UserAgent;

public:
	SpecificWebsite(std::string userAgent, void(*log)(void* object, std::string message), void(*NotifyUploadResult)(void* object, bool result), void* client);
	~SpecificWebsite();

	std::shared_ptr<std::string> visibility = std::make_shared<std::string>("DEFAULT");

	void(*Log)(void* object, std::string message);
	void(*NotifyUploadResult)(void* object, bool result);
	void* Client;

	void UploadReplay(std::string replayPath, std::string playerId, std::string specificWebsiteURL);
};

