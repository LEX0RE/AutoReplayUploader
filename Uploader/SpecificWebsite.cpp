#include "SpecificWebsite.h"

#include "HttpClient.h"


SpecificWebsite::SpecificWebsite(std::string userAgent, void(*log)(void* object, std::string message), void(*NotifyUploadResult)(void* object, bool result), void* client)
{
	this->UserAgent = userAgent;
	this->Log = log;
	this->NotifyUploadResult = NotifyUploadResult;
	this->Client = client;
}

void SpecificWebsiteRequestComplete(PostFileRequest* ctx)
{
	auto specificWebsite = (SpecificWebsite*)ctx->Requester;

	specificWebsite->Log(specificWebsite->Client, "SpecificWebsite::UploadCompleted with status: " + std::to_string(ctx->Status));
	if (ctx->Message.size() > 0)
	{
		specificWebsite->Log(specificWebsite->Client, ctx->Message);
	}
	if (ctx->ResponseBody.size() > 0)
	{
		specificWebsite->Log(specificWebsite->Client, ctx->ResponseBody);
	}
	specificWebsite->NotifyUploadResult(specificWebsite->Client, (ctx->Status >= 200 && ctx->Status < 300));

	DeleteFile(ctx->FilePath.c_str());

	delete ctx;
}

/**
* Posts the replay file to Specific Website
*/
void SpecificWebsite::UploadReplay(std::string replayPath, std::string playerId, std::string specificWebsiteURL)
{
	if (UserAgent.empty() || replayPath.empty() || specificWebsiteURL.empty())
	{
		Log(Client, "SpecificWebsite::UploadReplay Parameters were empty.");
		Log(Client, "UserAgent: " + UserAgent);
		Log(Client, "ReplayPath: " + replayPath);
		Log(Client, "SpecificWebsiteURL: " + specificWebsiteURL);
		return;
	}

	std::string path = AppendGetParams(specificWebsiteURL, { {"player_id", playerId}, {"visibility", *visibility} });

	std::string destPath = "./bakkesmod/data/specific_webssite/temp.replay";
	CreateDirectory("./bakkesmod/data/specific_webssite", NULL);
	bool resultOfCopy = CopyFile(replayPath.c_str(), destPath.c_str(), FALSE);

	Log(Client, "ReplayPath: " + replayPath);
	Log(Client, "DestPath: " + destPath);
	Log(Client, "File copy success: " + std::string(resultOfCopy ? "true" : "false"));

	PostFileRequest* request = new PostFileRequest();
	request->Url = path;
	request->FilePath = destPath;
	request->ParamName = "replays";
	request->Headers.push_back("UserAgent: " + UserAgent);
	request->RequestComplete = &SpecificWebsiteRequestComplete;
	request->RequestId = 1;
	request->Requester = this;
	request->Message = "";

	PostFileAsync(request);
}

SpecificWebsite::~SpecificWebsite()
{
}
