/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef TIME_PROFILER_H
#define TIME_PROFILER_H

#include <string>
#include <map>
#include <boost/noncopyable.hpp>
#include <cstring>

#include "System/float3.h"

// disable this if you want minimal profiling
// (sim time is still measured because of game slowdown)
#define SCOPED_TIMER(name) ScopedTimer myScopedTimerFromMakro(name);


class BasicTimer : public boost::noncopyable
{
public:
	BasicTimer(const char* const name);

protected:
	const std::string name;
	const unsigned starttime;
};


/**
 * @brief Time profiling helper class
 *
 * Construct an instance of this class where you want to begin time measuring,
 * and destruct it at the end (or let it be autodestructed).
 */
class ScopedTimer : public BasicTimer
{
public:
	ScopedTimer(const char* const name, bool autoShow = false): BasicTimer(name) {
		autoShowGraph = autoShow;
	}
	/**
	 * @brief destroy and add time to profiler
	 */
	~ScopedTimer();

private:
	bool autoShowGraph;
};


class ScopedOnceTimer : public BasicTimer
{
public:
	ScopedOnceTimer(const char* const name): BasicTimer(name) {}
	ScopedOnceTimer(const std::string& name): BasicTimer(name.c_str()) {}
	/**
	 * @brief destroy and print passed time to infolog
	 */
	~ScopedOnceTimer();
};



class CTimeProfiler
{
public:
	struct TimeRecord {
		TimeRecord() : total(0), current(0), percent(0), color(0,0,0), showGraph(false), peak(0), newpeak(false) { 
			memset(frames, 0, sizeof(frames));
		}
		unsigned total;
		unsigned current;
		static const unsigned frames_size = 128;
		unsigned frames[frames_size];
		float percent;
		float3 color;
		bool showGraph;
		float peak;
		bool newpeak;
	};

	CTimeProfiler();
	~CTimeProfiler();

	float GetPercent(const char *name);
	void AddTime(const std::string& name, unsigned time, bool showGraph = false);
	void Update();

	void PrintProfilingInfo() const;

	std::map<std::string,TimeRecord> profile;

private:
	unsigned lastBigUpdate;
	/// increases each update, from 0 to (frames_size-1)
	unsigned currentPosition;
};

extern CTimeProfiler profiler;

#endif // TIME_PROFILER_H
