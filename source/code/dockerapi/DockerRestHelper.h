#pragma once

#include <stdio.h>
#include <string>

using std::string;

class DockerRestHelper
{
public:
	///
	/// Create the REST request to list images
	///
	/// \returns Request in string format
	///
	static string restDockerImages()
	{
		return "GET /images/json?all=0 HTTP/1.1\r\n\r\n";
	}

	///
	/// Create the REST request to list containers
	///
	/// \returns Request in string format
	///
	static string restDockerPs()
	{
		return "GET /containers/json?all=1 HTTP/1.1\r\n\r\n";
	}

	///
	/// Create the REST request to list running containers
	///
	/// \returns Request in string format
	///
	static string restDockerPsRunning()
	{
		return "GET /containers/json HTTP/1.1\r\n\r\n";
	}

	///
	/// Create the REST request to inspect a container
	///
	/// \param[in] id ID of the container to be inspected
	/// \returns Request in string format
	///
	static string restDockerInspect(string id)
	{
		return "GET /containers/" + id + "/json HTTP/1.1\r\n\r\n";
	}

	///
	/// Create the REST request to list events
	///
	/// \returns Request in string format
	///
	static string restDockerEvents(int start, int end)
	{
		char result[70];
		sprintf(result, "GET /events?since=%d&until=%d HTTP/1.1\r\n\r\n", start, end);
		return string(result);
	}

	///
	/// Create the REST request to get container stats
	///
	/// \param[in] id ID of the container to get stats of
	/// \returns Request in string format
	///
	static string restDockerStats(string id)
	{
		return "GET /containers/" + id + "/stats?stream=false HTTP/1.1\r\n\r\n";
	}
};