#include "Keyboard.h"

//----------Event----------
//
//Default Constructor
Keyboard::Event::Event() : type(EventType::Invalid), keycode(0u)
{
}

Keyboard::Event::Event(const EventType _type, const unsigned char _keycode) : type(_type), keycode(_keycode)
{
}

bool Keyboard::Event::IsPressed() const
{
	return type == EventType::Press;
}

bool Keyboard::Event::IsReleased() const
{
	return type == EventType::Release;
}

bool Keyboard::Event::IsValid() const
{
	return type != EventType::Invalid;
}

unsigned char Keyboard::Event::GetKeyCode() const
{
	return keycode;
}

//----------Keyboard----------
//
Keyboard::Keyboard()
{
	// Range based loop to set keystates to off (false)
	for (auto& keys : keyStates)
	{
		keys = false;
	}
}

bool Keyboard::IsKeyPressed(const unsigned char keycode)
{
	return keyStates.at(keycode);
}

bool Keyboard::IsKeyBufferEmpty()
{
	return keyBuffer.empty();
}

bool Keyboard::IsCharBufferEmpty()
{
	return charBuffer.empty();
}

Keyboard::Event Keyboard::ReadKeycode()
{
	if (IsKeyBufferEmpty())
	{
		return Keyboard::Event();
	}

	// Gets the first element in the buffer, removes it and returns buffer
	Event event = keyBuffer.front();
	keyBuffer.pop();
	return event;
}

unsigned char Keyboard::ReadChar()
{
	if (IsCharBufferEmpty())
	{
		return 0u;
	}

	// Gets the first element in the buffer, removes it and returns buffer
	unsigned char temp = charBuffer.front();
	charBuffer.pop();
	return temp;
}

void Keyboard::OnKeyPressed(const unsigned char keycode)
{
	keyStates.at(keycode) = true;
	keyBuffer.push(Event(Event::EventType::Press, keycode));
}

void Keyboard::OnKeyReleased(const unsigned char keycode)
{
	keyStates.at(keycode) = false;
	keyBuffer.push(Event(Event::EventType::Release, keycode));
}

void Keyboard::OnChar(const unsigned char keycode)
{
	charBuffer.push(keycode);
}

void Keyboard::EnableAutoRepeatKey()
{
	autoRepeatedKey = true;
}

void Keyboard::DisableAutoRepeatKey()
{
	autoRepeatedKey = false;
}

void Keyboard::EnableAutoRepeatChar()
{
	autoRepeatedChar = true;
}

void Keyboard::DisableAutoRepeatChar()
{
	autoRepeatedChar = false;
}

bool Keyboard::IsAutoRepeatKeyOn()
{
	return autoRepeatedKey;
}

bool Keyboard::IsAutoRepeatCharOn()
{
	return autoRepeatedChar;
}
