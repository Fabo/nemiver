//Author: Fabien Parent
/*
 *This file is part of the Nemiver project
 *
 *Nemiver is free software; you can redistribute
 *it and/or modify it under the terms of
 *the GNU General Public License as published by the
 *Free Software Foundation; either version 2,
 *or (at your option) any later version.
 *
 *Nemiver is distributed in the hope that it will
 *be useful, but WITHOUT ANY WARRANTY;
 *without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *See the GNU General Public License for more details.
 *
 *You should have received a copy of the
 *GNU General Public License along with Nemiver;
 *see the file COPYING.
 *If not, write to the Free Software Foundation,
 *Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *See COPYRIGHT file copyright information.
 */
#ifndef __NMV_DBG_CONSOLE_H__
#define __NMV_DBG_CONSOLE_H__

#include "common/nmv-console.h"

NEMIVER_BEGIN_NAMESPACE(nemiver)

namespace common {
    class UString;
}

class IDebugger;

using common::SafePtr;
using common::UString;

class DBGConsole : public common::Console {
    struct Priv;
    SafePtr<Priv> m_priv;

public:
    DBGConsole (int a_fd, IDebugger &a_debugger);
    ~DBGConsole ();

    void current_file_path (const UString &a_file_path);
    const UString& current_file_path () const;

    sigc::signal<void, UString> file_opened_signal () const;
};

NEMIVER_END_NAMESPACE(nemiver)

#endif /* __NMV_DBG_CONSOLE_H__ */

