#include "Mouse.h"

// ----------MouesEvents----------

MouseEvent::MouseEvent() : type(EventType::Invalid), x(0), y(0)
{
}

MouseEvent::MouseEvent(const EventType _type, const int _x, const int _y) : type(type), x(_x), y(_y)
{
}

bool MouseEvent::IsValid() const
{
	return type != EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const
{
	return type;
}

MousePoint MouseEvent::GetPosition() const
{
	return {x, y};
}

int MouseEvent::GetPosX() const
{
	return x;
}

int MouseEvent::GetPosY() const
{
	return y;
}

// ----------MouseClass----------

void Mouse::OnLeftBtnPressed(int x, int y)
{
	leftBtnDown = true;
	MouseEvent event(MouseEvent::EventType::LBtnPress, x, y);
	eventBuffer.push(event);
}

void Mouse::OnLeftBtnReleased(int x, int y)
{
	leftBtnDown = false;
	MouseEvent event(MouseEvent::EventType::LBtnRelease, x, y);
	eventBuffer.push(event);
}

void Mouse::OnRightBtnPressed(int x, int y)
{
	rightBtnDown = true;
	MouseEvent event(MouseEvent::EventType::RBtnPress, x, y);
	eventBuffer.push(event);
}

void Mouse::OnRightBtnReleased(int x, int y)
{
	rightBtnDown = false;
	MouseEvent event(MouseEvent::EventType::RBtnRelease, x, y);
	eventBuffer.push(event);
}

void Mouse::OnMiddleBtnPressed(int x, int y)
{
	midBtnDown = true;
	MouseEvent event(MouseEvent::EventType::MBtnPress, x, y);
	eventBuffer.push(event);
}

void Mouse::OnMiddleBtnReleased(int x, int y)
{
	midBtnDown = false;
	MouseEvent event(MouseEvent::EventType::MBtnRelase, x, y);
	eventBuffer.push(event);
}

void Mouse::OnWheelUp(int x, int y)
{
	MouseEvent event(MouseEvent::EventType::WheelUp, x, y);
	eventBuffer.push(event);
}

void Mouse::OnWheelDown(int x, int y)
{
	MouseEvent event(MouseEvent::EventType::WheelDown, x, y);
	eventBuffer.push(event);
}

void Mouse::OnMouseMove(int _x, int _y)
{
	x = _x;
	y = _y;
	MouseEvent event(MouseEvent::EventType::Move, x, y);
	eventBuffer.push(event);
}

void Mouse::OnMouseMoveRaw(int _x, int _y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, _x, _y));
}

bool Mouse::IsLeftBtnDown()
{
	return leftBtnDown;
}

bool Mouse::IsMiddleBtnDown()
{
	return midBtnDown;
}

bool Mouse::IsRightBtnDown()
{
	return rightBtnDown;
}

int Mouse::GetPosX()
{
	return x;
}

int Mouse::GetPosY()
{
	return y;
}

MousePoint Mouse::GetPosition()
{
	return {x, y};
}

bool Mouse::EventBufferIsEmpty()
{
	return eventBuffer.empty();
}

MouseEvent Mouse::ReadEvent()
{
	if (eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent event = eventBuffer.front();
		eventBuffer.pop();
		return event;
	}
}
