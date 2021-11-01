#pragma once
#include <queue>

struct MousePoint
{
	int x;
	int y;
};

class MouseEvent
{
public:
	enum class EventType
	{
		LBtnPress,
		LBtnRelease,
		RBtnPress,
		RBtnRelease,
		MBtnPress,
		MBtnRelase,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid,
	};

	MouseEvent();
	MouseEvent(const EventType _type, const int _x, const int _y);
	~MouseEvent() = default;

	bool IsValid() const;

	// Getters
	EventType GetType() const;
	MousePoint GetPosition() const;
	int GetPosX() const;
	int GetPosY() const;

private:
	EventType type;
	int x;
	int y;
};


class Mouse
{
	public:
		Mouse() = default;
		~Mouse() = default;

		// Copy/Copy-Assign Operator
		//Mouse(const Mouse&) = delete;
		//Mouse& operator=(const Mouse&) = delete;

		void OnLeftBtnPressed(int x, int y);
		void OnLeftBtnReleased(int x, int y);
		
		void OnRightBtnPressed(int x, int y);
		void OnRightBtnReleased(int x, int y);
		
		void OnMiddleBtnPressed(int x, int y);
		void OnMiddleBtnReleased(int x, int y);
		
		void OnWheelUp(int x, int y);
		void OnWheelDown(int x, int y);
		
		void OnMouseMove(int _x, int _y);
		void OnMouseMoveRaw(int _x, int _y);

		bool IsLeftBtnDown();
		bool IsMiddleBtnDown();
		bool IsRightBtnDown();

		// Getters
		int GetPosX();
		int GetPosY();
		MousePoint GetPosition();

		bool EventBufferIsEmpty();
		MouseEvent ReadEvent();

		

	private:
		std::queue<MouseEvent> eventBuffer;
		bool leftBtnDown	= false;
		bool rightBtnDown	= false;
		bool midBtnDown	= false;
		int x = 0;
		int y = 0;
};
