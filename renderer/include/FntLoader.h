/*
 *	MIT License
 *
 *	Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

namespace	ExoRenderer
{

struct CharDescriptor
{
	float x, y;
	unsigned short width, height;
	float xOffset, yOffset;
	float xAdvance;

	float xMaxTextureCoord, yMaxTextureCoord;

	CharDescriptor()
	: x(0), y(0), width(0), height( 0 ), xOffset( 0 ), yOffset( 0 ), xAdvance(0)
	{ }
};

class FntLoader
{
public:
	FntLoader(const std::string& path)
	: _width(0), _height(0)
	{
		std::filebuf fb;
		if (fb.open(path.c_str(), std::ios::in))
		{
			std::istream is(&fb);
			while (is)
				parseFont(is);

			fb.close();
		}
	}

	virtual ~FntLoader(void)
	{ }

	inline CharDescriptor getCharacter(unsigned char character) const { return _chars[character]; }
private:
	void parseFont(std::istream& stream)
	{
		std::string line;
		std::string read, key, value;
		std::size_t i;

		while(!stream.eof())
		{
			std::stringstream lineStream;
			std::getline(stream, line);

			lineStream << line;
			lineStream >> read;

			if(read == "common")
			{
				// Read the line
				while(!lineStream.eof())
				{
					std::stringstream converter;
					lineStream >> read;
					i = read.find( '=' );

					// Split string (key=value)
					key = read.substr( 0, i );
					value = read.substr( i + 1 );

					converter << value;
					if(key == "lineHeight")
						converter >> _lineHeight;
					else if(key == "base")
						converter >> _base;
					else if(key == "scaleW")
						converter >> _width;
					else if(key == "scaleH")
						converter >> _height;
				}
			}
			else if(read == "char")
			{
				unsigned int charId = 0;

				while(!lineStream.eof())
				{
					std::stringstream converter;
					lineStream >> read;
					i = read.find( '=' );

					// Split string (key=value)
					key = read.substr( 0, i );
					value = read.substr( i + 1 );

					converter << value;
					if(key == "id")
						converter >> charId;
					else if(key == "x")
						converter >> _chars[charId].x;
					else if(key == "y")
						converter >> _chars[charId].y;
					else if(key == "width")
						converter >> _chars[charId].width;
					else if(key == "height")
						converter >> _chars[charId].height;
					else if(key == "xoffset")
						converter >> _chars[charId].xOffset;
					else if(key == "yoffset")
						converter >> _chars[charId].yOffset;
					else if(key == "xadvance")
						converter >> _chars[charId].xAdvance;
				}

				_chars[charId].xMaxTextureCoord = (_chars[charId].width + _chars[charId].x) / _width;
				_chars[charId].yMaxTextureCoord = (_chars[charId].height + _chars[charId].y) / _height;
				_chars[charId].x /= _width;
				_chars[charId].y /= _height;
			}
		}
	}
private:
	unsigned short _lineHeight;
	unsigned short _base;
	float _width, _height;
	CharDescriptor _chars[256];
};

}
