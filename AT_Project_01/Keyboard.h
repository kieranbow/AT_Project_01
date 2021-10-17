#pragma once
// C++
#include <array>
#include <queue>

class Keyboard
{
private:
	class Event // Nested Class
	{
	public:
		enum EventType
		{
			Press,
			Release,
			Invalid
		};

		// Constructor & Destructor
		Event();
		Event(const EventType _type, const unsigned char _keycode);
		~Event() = default;

		bool IsPressed() const;
		bool IsReleased() const;
		bool IsValid() const;
		unsigned char GetKeyCode() const;


	private:
		EventType type;
		unsigned char keycode;
	};

public:
	// Constructor & Destructor
	Keyboard();
	~Keyboard() = default;

	// Copy/Copy-Assign Operator
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	bool IsKeyPressed(const unsigned char keycode);
	bool IsKeyBufferEmpty();
	bool IsCharBufferEmpty();

	Event ReadKeycode();
	unsigned char ReadChar();

	void OnKeyPressed(const unsigned char keycode);
	void OnKeyReleased(const unsigned char keycode);
	void OnChar(const unsigned char keycode);

	// Setter
	void EnableAutoRepeatKey();
	void DisableAutoRepeatKey();

	void EnableAutoRepeatChar();
	void DisableAutoRepeatChar();

	// Check Auto Repeat
	bool IsAutoRepeatKeyOn();
	bool IsAutoRepeatCharOn();

private:
	// AutoRepeat
	bool autoRepeatedKey = false;
	bool autoRepeatedChar = false;

	// Keystates
	std::array<bool, 256>keyStates;

	// Buffers
	std::queue<Event> keyBuffer; // FIFO First-in, first-out
	std::queue<unsigned char> charBuffer; // FIFO First-in, first-out
};

