#pragma once

#include <iostream>
#include <cstdio>

class _Console
{
private:
	template<typename Arg, typename ...Args>
	void Print(std::ostream &os, const Arg& a, Args &&...args) const
	{
		os << a;
		((os << " " << args), ...);
		os << std::endl;
	}

	template<typename ...Args>
	void PrintFormat(FILE *fd, const std::string& fmt, Args &&...args) const
	{
		fprintf(fd, fmt.c_str(), std::forward<Args>(args)...);
	}

public:
	template<typename ...Args>
	void Log(Args &&...args) const
	{
		Print(std::cout, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	void Error(Args &&...args) const
	{
		Print(std::cerr, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	void LogFormat(const std::string& fmt, Args &&...args) const
	{
		PrintFormat(stdout, fmt, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	void ErrorFormat(const std::string& fmt, Args &&...args) const
	{
		PrintFormat(stderr, fmt, std::forward<Args>(args)...);
	}
};

static const _Console Console;