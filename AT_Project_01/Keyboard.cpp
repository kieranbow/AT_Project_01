#include "Keyboard.h"

//----------Event----------
//
Keyboard::KeyboardEvent::KeyboardEvent() : type(EventType::Invalid), keycode(0u)
{
}

Keyboard::KeyboardEvent::KeyboardEvent(const EventType _type, const unsigned char _keycode) : type(_type), keycode(_keycode)
{
}

bool Keyboard::KeyboardEvent::IsEventPressed() const
{
	return type == EventType::Press;
}

bool Keyboard::KeyboardEvent::IsEventReleased() const
{
	return type == EventType::Release;
}

bool Keyboard::KeyboardEvent::IsEventValid() const
{
	return type != EventType::Invalid;
}

unsigned char Keyboard::KeyboardEvent::GetKeyCode() const
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

Keyboard::KeyboardEvent Keyboard::ReadKeycode()
{
	if (IsKeyBufferEmpty())
	{
		return Keyboard::KeyboardEvent();
	}

	// Gets the first element in the buffer, removes it and returns buffer
	KeyboardEvent event = keyBuffer.front();
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
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, keycode));
}

void Keyboard::OnKeyReleased(const unsigned char keycode)
{
	keyStates.at(keycode) = false;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, keycode));
}

void Keyboard::OnChar(const unsigned char keycode)
{
	charBuffer.push(keycode);
}

//----------Auto Repeats----------
//
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
