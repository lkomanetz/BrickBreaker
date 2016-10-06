#pragma once
#ifndef _GAMEERROR_H
#define _GAMEERROR_H
#define WIN32_LEAN_AND_MEAN

class GameError;

#include <string>
#include <exception>

namespace GameErrorNS {
	/*
	 * Negative numbers are fatal errors that may require the game to shut down.
	 * Positive numbers are warnings that will not require the game to shut down.
	 */
	const int FATAL_ERROR = -1;
	const int WARNING = 1;
}

class GameError : public std::exception {
private:
	int _errorCode;
	std::string _message;

public:
	GameError() : _errorCode(GameErrorNS::FATAL_ERROR), _message("Undefined error in game.") {}
	GameError(const GameError& err) : std::exception(err), _errorCode(err._errorCode), _message(err._message) {}
	GameError(int code, const std::string &msg) : _errorCode(code), _message(msg) {}

	GameError& operator= (const GameError& err) {
		std::exception::operator=(err);
		this->_errorCode = err._errorCode;
		this->_message = err._message;
	}

	virtual ~GameError() {}
	virtual const char* what() const { return this->getMessage(); }

	const char* getMessage() const { return _message.c_str(); }
	int getErrorCode() const { return _errorCode; }
};
#endif