#pragma once
// C++
#include <array>
#include <queue>

// Description
// Keyboard class that handles reading char and keycodes.
class Keyboard
{
private:
	// Description
	// Nested Event Class for the KeyBoard class which contains 
	// an enum class of EventType and functions to control these EventType.
	class KeyboardEvent
	{
	public:
		enum class EventType
		{
			Press,
			Release,
			Invalid
		};

		// Constructor & Destructor
		KeyboardEvent();
		KeyboardEvent(const EventType _type, const unsigned char _keycode);
		~KeyboardEvent() = default;

		bool IsEventPressed() const;
		bool IsEventReleased() const;
		bool IsEventValid() const;
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

	KeyboardEvent ReadKeycode();
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
	std::array<bool, 256>keyStates = { { false } };

	// Buffers
	std::queue<KeyboardEvent> keyBuffer; // FIFO First-in, first-out
	std::queue<unsigned char> charBuffer; // FIFO First-in, first-out
};
