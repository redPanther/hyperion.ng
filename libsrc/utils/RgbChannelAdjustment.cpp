// STL includes
#include <cmath>
#include <cstdint>
#include <algorithm>

// Utils includes
#include <utils/RgbChannelAdjustment.h>

RgbChannelAdjustment::RgbChannelAdjustment(QString channelName)
	: _channelName(channelName)
	, _log(Logger::getInstance(channelName))
{
	resetInitialized();
}

RgbChannelAdjustment::RgbChannelAdjustment(uint8_t adjustR, uint8_t adjustG, uint8_t adjustB, QString channelName)
	: _channelName(channelName)
	, _log(Logger::getInstance(channelName))
{
	setAdjustment(adjustR, adjustG, adjustB);
}

RgbChannelAdjustment::~RgbChannelAdjustment()
{
}

void RgbChannelAdjustment::resetInitialized()
{
	Debug(_log, "initialize mapping with %d,%d,%d", _adjust[RED], _adjust[GREEN], _adjust[BLUE]);
	for(int idx=0;idx<256;idx++)
	{
		_initialized[idx] = false;
	}
}

void RgbChannelAdjustment::setAdjustment(uint8_t adjustR, uint8_t adjustG, uint8_t adjustB)
{
	_adjust[RED]   = adjustR;
	_adjust[GREEN] = adjustG;
	_adjust[BLUE]  = adjustB;
	resetInitialized();
}

uint8_t RgbChannelAdjustment::getAdjustmentR() const
{
	return _adjust[RED];
}

uint8_t RgbChannelAdjustment::getAdjustmentG() const
{
	return _adjust[GREEN];
}

uint8_t RgbChannelAdjustment::getAdjustmentB() const
{
	return _adjust[BLUE];
}

void RgbChannelAdjustment::apply(uint8_t input, uint8_t & red, uint8_t & green, uint8_t & blue)
{
	if (!_initialized[input])
	{
		_mapping[0][input] = std::min( (((unsigned)input * _adjust[0]) / UINT8_MAX), (unsigned)UINT8_MAX);
		_mapping[1][input] = std::min( (((unsigned)input * _adjust[1]) / UINT8_MAX), (unsigned)UINT8_MAX);
		_mapping[2][input] = std::min( (((unsigned)input * _adjust[2]) / UINT8_MAX), (unsigned)UINT8_MAX);
		_initialized[input] = true;
	}
	red   = _mapping[RED][input];
	green = _mapping[GREEN][input];
	blue  = _mapping[BLUE][input];
}
