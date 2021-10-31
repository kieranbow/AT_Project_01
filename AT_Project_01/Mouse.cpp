#include "Mouse.h"

// ----------MouesEvents----------

Mouse::MouseEvent::MouseEvent() : type(EventType::Invalid), x(0), y(0)
{
}

Mouse::MouseEvent::MouseEvent(const EventType _type, const int _x, const int _y) : type(type), x(_x), y(_y)
{
}

bool Mouse::MouseEvent::IsValid() const
{
	return type != EventType::Invalid;
}

Mouse::MouseEvent::EventType Mouse::MouseEvent::GetType() const
{
	return type;
}

MousePoint Mouse::MouseEvent::GetPosition() const
{
	return {x, y};
}

int Mouse::MouseEvent::GetPosX() const
{
	return x;
}

int Mouse::MouseEvent::GetPosY() const
{
	return y;
}

// ----------MouseClass----------

void Mouse::OnLeftBtnPressed(int x, int y)
{
	leftBtnDown = true;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LBtnPress, x, y));
}

void Mouse::OnLeftBtnReleased(int x, int y)
{
	leftBtnDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LBtnRelease, x, y));
}

void Mouse::OnRightBtnPressed(int x, int y)
{
	rightBtnDown = true;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RBtnPress, x, y));
}

void Mouse::OnRightBtnReleased(int x, int y)
{
	rightBtnDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RBtnRelease, x, y));
}

void Mouse::OnMiddleBtnPressed(int x, int y)
{
	midBtnDown = true;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::MBtnPress, x, y));
}

void Mouse::OnMiddleBtnReleased(int x, int y)
{
	midBtnDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::MBtnRelase, x, y));
}

void Mouse::OnWheelUp(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void Mouse::OnWheelDown(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void Mouse::OnMouseMove(int _x, int _y)
{
	x = _x;
	y = _y;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
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

Mouse::MouseEvent Mouse::ReadEvent()
{
	if (EventBufferIsEmpty())
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
