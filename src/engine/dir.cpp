/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#if defined(_MSC_VER) || defined(__MINGW32CE__) || defined(__MINGW32__)
#include <windows.h>
#else
#include <dirent.h>
#endif

#include "system.h"
#include "tools.h"
#include "dir.h"


void ListFiles::Append(const ListFiles & list)
{
    insert(end(), list.begin(), list.end());
}

void ListFiles::ReadDir(const std::string &path, const std::string &filter, bool sensitive)
{
#if defined(_MSC_VER) || defined(__MINGW32CE__) || defined(__MINGW32__)
    std::string pattern(path + "\\*" + filter);
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            push_back(path + "\\" + data.cFileName);
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
#else
    // read directory
    DIR *dp = opendir(path.c_str());

    if(dp)
    {
	struct dirent *ep;
	while(NULL != (ep = readdir(dp)))
	{
	    const std::string fullname = System::ConcatePath(path, ep->d_name);

    	    // if not regular file
    	    if(! System::IsFile(fullname)) continue;

	    if(filter.size())
	    {
    		std::string filename(ep->d_name);

		if(sensitive)
		{
		    if(std::string::npos == filename.find(filter)) continue;
    		}
    		else
    		{
		    if(std::string::npos == StringLower(filename).find(StringLower(filter))) continue;
		}
    	    }

    	    push_back(fullname);
	}
	closedir(dp);
    }
#endif
}

void ListDirs::Append(const std::list<std::string> & dirs)
{
    insert(end(), dirs.begin(), dirs.end());
}
