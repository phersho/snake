#include "stdafx.h"

namespace SnakeObjects
{
    /*
     * Creates an instance of Snake.
     */
    Snake::Snake(int length, Location initial)
    {
        if (length < SNAKE_MIN_LENGTH)
        {
            throw new std::logic_error("Longitud inicial");
        }

        this->length = length;
        direction = SNAKE_DIRECTION_DEFAULT;
        body = Location::BuildList(initial, length, DirectionWest);

        RefreshLimits();
    }

    /*
     * Unloads an instance of Snake.
     */
    Snake::~Snake()
    {
        delete body;

        length = 0;
        direction = DirectionNone;
    }

    /*
     * Gets current direction.
     */
    Direction Snake::GetDirection() const
    {
        return direction;
    }

    /*
     * Sets a new direction.
     */
    void Snake::SetDirection(Direction newDirection)
    {
        direction = newDirection;
    }

    /*
     * Gets access to body inside.
     */
    LocationListPointer Snake::GetBody() const
    {
        return body;
    }

    /*
     * Indicates if 'location' can overlap this snake.
     */
    bool Snake::CanOverlap(const Location& location) const
    {
        LocationPointer previous = &body->front();
        for (auto current = body->begin(); current != body->end(); ++current)
        {
            if (location.IsBetween(*previous, *current, true))
            {
                return true;
            }
            previous = &current._Ptr->_Myval;
        }
        return false;
    }

    /*
     * Indicates if 'location' is target of this snake.
     */
    bool Snake::IsTarget(const Location& location) const
    {
        return body->front().IsTarget(direction, location);
    }

    /*
     * Indicates if 'location' is the head of this snake.
     */
    bool Snake::IsHead(const Location& other) const
    {
        return other == body->front();
    }

    /*
     * Indicates if this snake is collisioning with 'location'.
     */
    bool Snake::IsCollided(const Location& location) const
    {
        return IsTarget(location) && body->front().GetDistance(location) == 0;
    }

    /*
     * Refreshes limits of snake's area.
     */
    void Snake::RefreshLimits()
    {
        minHeight = INT_MAX;
        minWidth = INT_MAX;
        maxHeight = INT_MIN;
        maxWidth = INT_MIN;

        for (auto current = body->begin(); current != body->end(); ++current)
        {
            minHeight = min(minHeight, current->GetY());
            minWidth = min(minWidth, current->GetX());
            maxHeight = max(maxHeight, current->GetY());
            maxWidth = max(maxWidth, current->GetX());
        }
    }

    /*
     * Advances this snake by its state.
     */
    void Snake::Advance(LocationListPointer newParts)
    {
        Location& head = body->front();
        bool withoutNews = true;
        LocationPointer partAdded;

        // evaluates if snake will have new parts
        for (auto i = newParts->begin(); i != newParts->end(); ++i)
        {
            if (head.IsTarget(direction, *i) && head.GetDistance(*i) == SNAKE_MOV)
            {
                if (head.IsBetween(*(++body->begin()), *i, false))
                {
                    head.Move(SNAKE_MOV, direction);
                }
                else
                {
                    body->push_front(*i);
                }

                withoutNews = false;
                partAdded = &*i;
                break;
            }
        }

        if (withoutNews)
        {
            auto newHead = head.GetDestiny(direction, SNAKE_MOV);
            if (head.IsBetween(*newHead, *(++body->begin()), false))
            {
                head.Set(*newHead);
                delete newHead;
            }
            else
            {
                body->push_front(*newHead);
            }

            auto final = --body->end();
            auto before = ----body->end();
            
            auto newTail = final->GetDestiny(final->GetDirection(*before), SNAKE_MOV);
            if (*newTail == *before)
            {
                body->pop_back();
            }
            else
            {
                final->Set(*newTail);
            }
            delete newTail;
        }
        else
        {
            newParts->remove(*partAdded);
        }

        RefreshLimits();
    }

    /*
     * Indicates if this snake is inside the coordinates indicated.
     */
    bool Snake::IsInside(const int x1, const int y1, const int x2, const int y2) const
    {
        return x1 <= minWidth && y1 <= minHeight 
            && maxWidth <= x2 && maxHeight <= y2;
    }

    /*
     * Indicates if this snake has collided with itself.
     */
    bool Snake::IsCollidedInside() const
    {
        LocationPointer last, head = &body->front();
        for (auto current = body->begin(); current != body->end(); ++current)
        {
            if (!(&*current == head))
            {
                if (!(&*last == head))
                {
                    if (head->IsBetween(*current, *last, true))
                    {
                        return true;
                    }
                }
            }

            last = &*current;
        }

        return false;
    }

    bool Snake::CheckBody() const
    {
        LocationPointer last = nullptr, head = &body->front();
        for (auto current = body->begin(); current != body->end(); ++current)
        {
            if (!(*current == *head))
            {
                if (current->GetDirection(*last) == DirectionNone)
                {
                    return false;
                }
            }

            last = &*current;
        }

        return true;
    }
}