#include "stdafx.h"
#include "SnakeObjects.h"

namespace SnakeObjects
{
    SinglePartGenerator::SinglePartGenerator()
    {
    }

    SinglePartGenerator::~SinglePartGenerator()
    {
        generated.clear();
    }

    LocationListPointer SinglePartGenerator::Generate()
    {
        LocationPointer lp = nullptr;

        while (generated.empty())
        {
            lp = new Location(rand() % Stage->GetWidth(), rand() % Stage->GetHeight());
            if (Stage->GetSnake()->CanOverlap(*lp))
            {
                delete lp;
            }
            else
            {
                generated.push_front(*lp);
            }
        }

        return &generated;
    }
}