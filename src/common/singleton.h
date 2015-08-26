// Pożyczone z https://github.com/colobot/colobot/blob/dev/src/common/singleton.h ;)
/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2015, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsitec.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

#pragma once

/**
 * \class CSingleton
 * \brief CSingleton base class for singletons
 */
template<typename T> class CSingleton
{
protected:
    static T* m_instance;

public:
    static T& GetInstance()
    {
        return *m_instance;
    }

    static T* GetInstancePointer()
    {
        return m_instance;
    }

    static bool IsCreated()
    {
        return m_instance != nullptr;
    }

    CSingleton()
    {
        m_instance = static_cast<T *>(this);
    }

    ~CSingleton()
    {
        m_instance = nullptr;
    }

    CSingleton& operator=(const CSingleton<T> &) = delete;
    CSingleton(const CSingleton<T> &) = delete;
};
